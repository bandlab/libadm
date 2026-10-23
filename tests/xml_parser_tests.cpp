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

TEST_CASE("ADM parsing reuses a generic DOM without consuming custom nodes") {
  adm::xml::DomDocument xml;
  xml.parse(
      "<?xml version=\"1.0\"?><!-- metadata -->"
      "<format><audioFormatExtended>"
      "<audioProgramme audioProgrammeID=\"APR_1001\" "
      "audioProgrammeName=\"test\"/>"
      "</audioFormatExtended><audioFormatCustom id=\"preset\"/></format>");
  auto custom = xml.root().findNode("audioFormatCustom");
  auto document =
      adm::parseXml(xml, adm::xml::ParserOptions::recursive_node_search);
  REQUIRE(document->getElements<adm::AudioProgramme>().size() == 1);
  REQUIRE(custom.attribute("id") == "preset");
  REQUIRE_NOTHROW(
      adm::parseXml(xml, adm::xml::ParserOptions::recursive_node_search));
  REQUIRE_THROWS(adm::parseXml(xml));
  xml.parse("<other/>");
  REQUIRE(document->getElements<adm::AudioProgramme>()
              .front()
              ->get<adm::AudioProgrammeName>()
              .get() == "test");
}

TEST_CASE("generic XML local name lookup preserves exact name lookup") {
  adm::xml::DomDocument document;
  document.parse(
      "<root><!-- <preset/> --><x:preset x:id=\"A &amp; B\"/>"
      "<!-- preset --><other/><preset id=\"second\"/>"
      "<y:preset id=\"third\"/></root>");
  auto root = document.root().firstNode("root");
  auto preset = root.childElement("preset");
  REQUIRE(preset.name() == "x:preset");
  REQUIRE(preset.attributeValue("id") == "A & B");
  REQUIRE_FALSE(preset.attribute("id"));
  REQUIRE(root.firstNode("preset").attribute("id") == "second");
  auto second = preset.nextSiblingElement("preset");
  REQUIRE(second.attributeValue("id") == "second");
  auto third = second.nextSiblingElement("preset");
  REQUIRE(third.name() == "y:preset");
  REQUIRE(third.attributeValue("id") == "third");
  REQUIRE_FALSE(third.nextSiblingElement("preset"));
  REQUIRE_FALSE(preset.nextSiblingElement("missing"));
  REQUIRE_THROWS_AS(preset.nextSiblingElement(""), std::invalid_argument);
  REQUIRE_FALSE(root.childElement("missing"));
  REQUIRE_FALSE(preset.attributeValue("missing"));
  REQUIRE_THROWS_AS(root.childElement(""), std::invalid_argument);
  REQUIRE_THROWS_AS(preset.attributeValue(""), std::invalid_argument);
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
  for (std::size_t i = 0; i < 128; ++i) nested += "<n>";
  auto atLimit = nested;
  for (std::size_t i = 0; i < 128; ++i) atLimit += "</n>";
  adm::xml::DomDocument document;
  REQUIRE_NOTHROW(document.parse(atLimit));
  rejects(nested + "<n/>" + atLimit.substr(nested.size()));
  std::string deep;
  for (int i = 0; i < 100000; ++i) deep += "<n>";
  rejects(deep);
  rejects("<n>unterminated");
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
