#pragma once

#include <algorithm>

namespace krimo {

template<size_t TSize>
struct CXString {
	char data[TSize];
	constexpr CXString(const char (&init)[TSize]) { std::copy_n(init, TSize, data); }
	constexpr size_t Size() const { return TSize - 1; }
};

}