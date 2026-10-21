#include <catch2/catch.hpp>
#include "adm/private/rapidxml_utils.hpp"
#include "adm/private/rapidxml_wrapper.hpp"
#include "adm/errors.hpp"
#include "adm/parse.hpp"
#include "adm/xml.hpp"

TEST_CASE("generic XML siblings and generated nodes") {
  adm::xml::DomDocument document;
  document.parse(
      "<format><audioFormatExtended/>"
      "<audioFormatCustom><presetInformation name=\"A &amp; B\"/>"
      "</audioFormatCustom></format>");
  auto custom = document.root().findNode("audioFormatCustom");
  REQUIRE(custom);
  REQUIRE(custom.firstNode("presetInformation").attribute("name") == "A & B");
  REQUIRE(document.root()
              .firstNode("format")
              .firstNode("audioFormatExtended")
              .nextSibling("audioFormatCustom"));
  REQUIRE_FALSE(custom.findNode("missing"));
  auto info = custom.firstNode("presetInformation");
  info.setAttribute("name", "\"<new>&");
  info.setAttribute("name", "updated & escaped");
  info.addNode("description").setValue("A < B & C");
  adm::xml::DomDocument roundTrip;
  roundTrip.parse(document.toString());
  auto parsed = roundTrip.root().findNode("presetInformation");
  REQUIRE(parsed.attribute("name") == "updated & escaped");
  REQUIRE(parsed.firstNode("description").value() == "A < B & C");
}

TEST_CASE("custom XML closing text is not markup inside XML constructs") {
  const std::string xml =
      "<format><audioFormatCustom>"
      "<!-- </audioFormatCustom> -->"
      "<![CDATA[</audioFormatCustom>]]>"
      "<?custom </audioFormatCustom> ?>"
      "<presetInformation text=\"&lt;/audioFormatCustom&gt;\"/>"
      "</audioFormatCustom></format>";
  adm::xml::DomDocument document;
  std::istringstream input(xml);
  document.parse(input);
  REQUIRE(document.root().findNode("presetInformation"));
  adm::xml::DomDocument roundTrip;
  roundTrip.parse(document.toString());
  REQUIRE(roundTrip.root().findNode("presetInformation"));
}

TEST_CASE("generic XML skips DTD markup during depth validation") {
  adm::xml::DomDocument document;
  REQUIRE_NOTHROW(document.parse(
      "<!DOCTYPE format [<!ELEMENT format ANY><!-- <ignored> -->]>"
      "<format><audioFormatCustom/></format>"));
  REQUIRE(document.root().findNode("audioFormatCustom"));
}

TEST_CASE("failed generic XML parsing preserves the previous document") {
  adm::xml::DomDocument document;
  document.parse("<presetInformation/>");
  REQUIRE_THROWS(document.parse("<broken>"));
  REQUIRE(document.root().findNode("presetInformation"));
  std::istringstream input("<new/>");
  input.setstate(std::ios::failbit);
  REQUIRE_THROWS(document.parse(input));
  REQUIRE_THROWS(document.parse(""));
  REQUIRE(document.root().findNode("presetInformation"));
  REQUIRE_THROWS_AS(document.root().addNode(""), std::invalid_argument);
  REQUIRE_THROWS_AS(document.root().setAttribute("", ""),
                    std::invalid_argument);
}

TEST_CASE("all XML entry points reject excessive nesting and mismatched tags") {
  const auto rejects = [](const std::string& text) {
    adm::xml::DomDocument document;
    REQUIRE_THROWS(document.parse(text));
    std::istringstream admInput(text);
    REQUIRE_THROWS(adm::parseXml(
        admInput, adm::xml::ParserOptions::recursive_node_search));
    std::istringstream frameInput(text);
    REQUIRE_THROWS(adm::parseFrameHeader(frameInput));
  };
  rejects("<format><audioFormatCustom><presetInformation/></wrong></format>");
  rejects("<format><audioFormatExtended></wrong></format>");
  rejects("<frame><frameHeader></wrong></frame>");
  std::string nested;
  for (std::size_t i = 0; i < adm::xml::maxXmlDepth; ++i) nested += "<n>";
  auto atLimit = nested;
  for (std::size_t i = 0; i < adm::xml::maxXmlDepth; ++i) atLimit += "</n>";
  adm::xml::DomDocument document;
  REQUIRE_NOTHROW(document.parse(atLimit));
  rejects(nested + "<n/>" + atLimit.substr(nested.size()));
  std::string deep;
  for (int i = 0; i < 100000; ++i) deep += "<n>";
  rejects(deep);
  rejects("<n>unterminated");
  rejects(std::string("<n/>\0<n/>", 9));
}

TEST_CASE("RapidXML bounds recursion even without the XML preflight") {
  std::string text;
  for (int i = 0; i < 10000; ++i) text += "<n>";
  rapidxml::xml_document<> document;
  REQUIRE_THROWS_AS(
      document.parse<rapidxml::parse_validate_closing_tags>(text.data()),
      rapidxml::parse_error);
  std::string shallow = "<n/>";
  REQUIRE_NOTHROW(
      document.parse<rapidxml::parse_validate_closing_tags>(shallow.data()));
}

TEST_CASE("custom XML size bound includes complete and self closing elements") {
  const std::string opening = "<audioFormatCustom>";
  const std::string closing = "</audioFormatCustom>";
  const auto paddingSize =
      adm::xml::maxCustomElementBytes - opening.size() - closing.size();
  const auto atLimit = opening + std::string(paddingSize, 'x') + closing;
  adm::xml::DomDocument document;
  REQUIRE_NOTHROW(document.parse(atLimit));
  const auto oversized = opening + std::string(paddingSize + 1, 'x') + closing;
  REQUIRE_THROWS(document.parse(oversized));
  std::istringstream input(oversized);
  REQUIRE_THROWS(document.parse(input));
  std::istringstream admInput("<format><audioFormatExtended/>" + oversized +
                              "</format>");
  REQUIRE_THROWS(
      adm::parseXml(admInput, adm::xml::ParserOptions::recursive_node_search));
  REQUIRE_THROWS(document.parse(
      "<audioFormatCustom value=\"" +
      std::string(adm::xml::maxCustomElementBytes, 'x') + "\"/>"));
  REQUIRE_THROWS(document.parse(
      opening + std::string(adm::xml::maxCustomElementBytes, 'x')));
}

TEST_CASE("line_number_calculation") {
  using namespace adm;
  {
    std::istringstream admStream(
        "<audioFormatExtended>\n"
        "<audioChannelFormat "
        "audioChannelFormatID=\"AC_00031002\" "
        "audioChannelFormatName=\"MyChannelFormat\" "
        "typeLabel=\"0003\""
        ">\n"
        "<frequency \n typeDefinition=\"lowPass\">120</frequency>"
        "</audioChannelFormat>"
        "</audioFormatExtended>");

    rapidxml::file<> xmlFile(admStream);
    rapidxml::xml_document<> xmlDocument;
    xmlDocument.parse<0>(xmlFile.data());
    auto root = xmlDocument.first_node();
    auto freq = root->first_node()->first_node();
    auto p = adm::xml::getDocumentLine(freq);
    REQUIRE(p == 2);
    p = adm::xml::getDocumentLine(freq->first_attribute());
    REQUIRE(p == 3);
  }
}

TEST_CASE("empty document") {
  using namespace adm;

  SECTION("ebucore") {
    std::istringstream admStream("");
    REQUIRE_THROWS_AS(parseXml(admStream), error::XmlParsingError);
  }

  SECTION("recursive") {
    std::istringstream admStream("");
    REQUIRE_THROWS_AS(
        parseXml(admStream, xml::ParserOptions::recursive_node_search),
        error::XmlParsingError);
  }
}
