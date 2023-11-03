#include <krimo/krimo.hpp>

#include <iostream>

krimo::Krimo::Krimo(){

}

krimo::Krimo::~Krimo(){
	for(auto rit = systems.rbegin(); rit != systems.rend(); ++rit){
		delete *rit;
	}
}

void krimo::Krimo::Run(){
	while(!shouldQuit){
		for(auto system : systems) {
			system->OnUpdate();
		}
	}
	for(auto rit = systems.rbegin(); rit != systems.rend(); ++rit){
		(*rit)->OnDetach();
	}
}

void krimo::Krimo::Quit(){
	shouldQuit = true;
}