#include <krimo/krimo.hpp>
#include <krimo/System.hpp>

void krimo::System::Init(Krimo* krimo) {
	this->krimo = krimo;
	OnAttach();
}