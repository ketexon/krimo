#pragma once

#include <string>
#include <algorithm>
#include <string_view>

#include <krimo/util/Hash.hpp>
#include <krimo/types/CXString.hpp>

namespace krimo {

constexpr uint32_t HashString(std::string_view sv){
	return crc::CRC32C{}(sv.data(), sv.size());
}

template<CXString s>
constexpr uint32_t HashString(){
	return crc::CRC32C{}(s.data, s.Size());
}

template<CXString s>
constexpr uint32_t operator ""_khs() {
	return crc::CRC32C{}(s.data, s.Size());
}

}