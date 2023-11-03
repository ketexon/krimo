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
	impl::WindowClassInternals* internals;

public:
	Event<WindowEvent> windowEvent;

	void OnAttach() override;
	void OnUpdate() override;
	void OnDetach() override;
};

}