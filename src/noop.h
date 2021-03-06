#ifndef NOOP_H
#define NOOP_H

#include <cassert>
#include <codecvt>
#include <iostream>
#include <locale>

#define NOOP_VERSION_MAJOR 0
#define NOOP_VERSION_MINOR 0
#define NOOP_VERSION_PATCH 1

namespace noop {

extern bool noop_debug_;
extern bool noop_disable_gc_;
extern bool noop_report_pool_usage_;
#define DEBUG if (noop::noop_debug_) \
  std::cout << "\033[1m\033[32m[" << __FILE__ << ":" << __LINE__ << "]\033[0m "
#define STDOUT (noop::noop_debug_ ? \
  std::cout << "\033[1m\033[34m[" << __FILE__ << ":" << __LINE__ << "]\033[0m " : \
  std::cout << "")

typedef std::u32string String;
typedef char32_t Char;
typedef double Number;

} // namespace noop

#endif // NOOP_H
