#pragma once

#include <krimo/types/TypeID.hpp>

namespace krimo {

class Krimo;

class System : public ITypeID {
public:
	Krimo* krimo;

	System() = default;

	System(System&) = delete;
	System(System&&) = delete;

	virtual ~System(){};

	void Init(Krimo*);
	virtual void OnAttach() {}
	virtual void OnUpdate() {}
	virtual void OnDetach() {}
};

}