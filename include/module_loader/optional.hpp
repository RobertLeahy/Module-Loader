/**
 *	\file
 */

#pragma once

#if __has_include(<experimental/optional>)
#include <experimental/optional>
namespace module_loader {
using std::experimental::bad_optional_access;
using std::experimental::in_place;
using std::experimental::in_place_t;
using std::experimental::nullopt;
using std::experimental::nullopt_t;
using std::experimental::optional;
}
#endif
