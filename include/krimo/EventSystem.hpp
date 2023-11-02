#pragma once

#include <krimo/System.hpp>

namespace krimo {

class EventSystem : public System {
	KRIMO_IMPL_ITYPEID(EventSystem);
public:
	void OnAttach() override;
};

}