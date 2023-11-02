#include <iostream>
#include <krimo/krimo.hpp>
#include <krimo/SceneTree.hpp>
#include <krimo/EventSystem.hpp>
#include <krimo/RenderingSystem.hpp>

using namespace krimo;

class TestComponent : Component {
	// KRIMO_IMPL_REFLECTION(TestComponent, Component);
};


#define KRIMO_COUT(x) std::cout << x << std::endl;

int main(){
	Transform node;
	Krimo{}.WithSystems<EventSystem, RenderingSystem>();

	return 0;
}