#pragma once

#include <krimo/types/Property.hpp>
#include <krimo/types/Math.hpp>
#include <krimo/types/TypeID.hpp>
#include <map>
#include <memory>

namespace krimo {

class Transform;
class Entity;
class Component;

class Transform {
private:
	Transform* parent_ = nullptr;
public:
	ReadonlyProperty<Transform*> parent = parent_;

	FVec3 position;
	FVec3 scale;
	FQuat rotation;


};

class Component {

};

class Entity {
private:
	std::map<TID, std::shared_ptr<Component>> components;

public:
	template<class T>
	std::weak_ptr<Component> GetComponent(){
		auto it = components.find(TypeID<Component>());
		if(it != components.end()){
			return *it;
		}
		return std::weak_ptr<Component>();
	}

	// template<class T>
	// std::weak
};

} // namespace krimo