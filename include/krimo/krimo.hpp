#pragma once

#include <krimo/Macro.hpp>
#include <krimo/EventSystem.hpp>
#include <krimo/EventEmitter.hpp>
#include <krimo/System.hpp>
#include <concepts>
#include <vector>
#include <map>
#include <krimo/types/TypeID.hpp>

static_assert(KRIMO_CXX_VERSION > 202000L);

namespace krimo {

enum class KrimoEventType {
	Update
};

struct KrimoEvent {
	KrimoEventType type;
};

class Krimo : public EventEmitter<KrimoEvent> {
private:
	std::vector<System*> systems;
	std::map<TID, System*> systemTIDMap;

	bool shouldQuit = false;

public:
	Krimo();

	template<std::derived_from<System> S>
	Krimo& WithSystem() {
		System* s = new S;
		systems.push_back(s);
		systemTIDMap[s->TypeID()] = s;
		s->Init(this);
		return *this;
	}

	template<std::derived_from<System>... Ss>
	Krimo& WithSystems() {
		([=](){this->WithSystem<Ss>();}(), ...);
		return *this;
	}

	void Run();

	void Quit();

	~Krimo();
};

}