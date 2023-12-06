#include "color.hpp"

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  // need to ensure valid color has been constructed...
 if (r < 0 || r > kColorValueMax || g < 0 || g > kColorValueMax || b < 0 || b > kColorValueMax) {
    throw std::runtime_error("invalid color");
 }
}


// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}