#include <algorithm>
#include <cstddef>
namespace aleph::core {

template <size_t N> struct TString {
  constexpr TString(const char (&str)[N]) { std::copy_n(str, N, value); }
  char value[N] = {};
};
} // namespace aleph::core