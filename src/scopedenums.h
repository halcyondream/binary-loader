#ifndef SCOPED_ENUMS_H
#define SCOPED_ENUMS_H
#include <type_traits>

// Get a scoped enum member's underlying value. Typically, this is an
// integer.
template <typename E> decltype(auto) getValue(E const value) {
  return static_cast<typename std::underlying_type<E>::type>(value);
}

// Overload a class enum's Bitwise AND operator based on its underlying
// value.
template <typename E>
auto operator&(E const x, E const y) -> typename std::underlying_type<E>::type {
  return getValue(x) & getValue(y);
}

#endif