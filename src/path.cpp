#include "adm/path.hpp"

namespace adm {

  struct AddElementVisitor {
    explicit AddElementVisitor(Path* path) : path_(path) {}

    template <typename T>
    void operator()(T v) const {
      path_->add(v);
    }

   private:
    Path* path_;
  };

  struct AddIdVisitor {
    explicit AddIdVisitor(Path* path) : path_(path) {}

    template <typename T>
    void operator()(T v) const {
      path_->add(v);
    }

   private:
    Path* path_;
  };

  void Path::add(ElementConstVariant elementVariant) {
    std::visit(AddElementVisitor(this), elementVariant);
  }

  void Path::add(ElementIdVariant elementIdVariant) {
    std::visit(AddIdVisitor(this), elementIdVariant);
  }

}  // namespace adm
