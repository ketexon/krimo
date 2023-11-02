#pragma once

#include <krimo/System.hpp>
#include <memory>
#include <vector>
#include <krimo/types/Delegate.hpp>

namespace krimo {

namespace impl {

class WindowClassInternals;

}

class WindowEvent {
public:
	bool preventDefault = false;
};

class WindowSystem : public System {
	KRIMO_IMPL_ITYPEID(WindowSystem);
private:
	std::unique_ptr<impl::WindowClassInternals> internals;

public:


	void OnAttach() override;
	// void OnDetach() override;
};

}