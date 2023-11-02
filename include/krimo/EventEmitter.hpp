#pragma once

#include <krimo/types/CXString.hpp>
#include <krimo/types/Delegate.hpp>
#include <krimo/types/HashString.hpp>
#include <krimo/util/Hash.hpp>
#include <map>

namespace krimo {

template<class TEvent>
class EventEmitter {
protected:
	std::map<uint32_t, Event<const TEvent&>> events;

public:

	template<CXString s, class F>
	void On(F callback) {
		uint32_t hash = HashString<s>();
		events[hash] += callback;
	}

	template<class F>
	void On(std::string_view sv, F callback) {
		uint32_t hash = HashString(sv);
		events[hash] += callback;
	}

	template<class F>
	void On(uint32_t hash, F callback) {
		events[hash] += callback;
	}

	template<CXString s>
	void Emit(TEvent evt) {
		uint32_t hash = HashString<s>();
		(events[hash])(evt);
	}

	void Emit(std::string_view sv, TEvent evt) {
		uint32_t hash = HashString(sv);
		(events[hash])(evt);
	}

	void Emit(uint32_t hash, TEvent evt) {
		(events[hash])(evt);
	}
};

}