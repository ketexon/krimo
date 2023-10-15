#pragma once

#include <functional>
#include <map>
#include <compare>
#include <list>

namespace krimo {

template<class F>
class Function;

/// @brief Function is a comparable version of std::function that accepts static, member, capturing lambda, and non-capturing lambda functions.
/// @tparam Ret
/// @tparam ...Params
template<class Ret, class... Params>
class Function<Ret(Params...)> {
public:
	using Signature = Ret(Params...);
	using PSignature = Ret(*)(Params...);
	template<class T>
	using MemberSignature = Ret(T::*)(Params...);

	Function() {}
	Function(const Function<Signature>& other)
		: fnPtr{other.fnPtr},
		instancePtr{other.instancePtr},
		mfnPtr{other.mfnPtr},
		fn{other.fn}
	{}
	Function(Function<Signature>&& other)
		: fnPtr{other->fnPtr},
		instancePtr{other->instancePtr},
		mfnPtr{other->mfnPtr}
	{
		std::swap(fn, other.fn);
	}
	Function(PSignature&& fn) : fnPtr{&fn}, fn{fn} {}

	template<class T>
	Function(T& instance, MemberSignature<T>&& mfn) : instancePtr{static_cast<void*>(&instance)}, mfnPtr{&mfn} {
		fn = [&instance, &mfn](Params... params) { return (instance.*mfn)(std::forward<Params>(params)...); };
	}

	template<class T>
	Function(T c) {
		static_assert(
			std::is_same_v<
				decltype(c(std::declval<Params>()...)),
				Ret
			>,
			"Argument is not callable with arguments Params... returning Ret"
		);
		fn = c;
		fnPtr = static_cast<void*>(&c);
	}

	Function<Signature>& operator=(const Function<Signature>& other){
		if(&other != this){
			instancePtr = other.instancePtr;
			fn = other.fn;
			fnPtr = other.fnPtr;
		}
		return *this;
	}

	Function<Signature>& operator=(Function<Signature>&& other){
		if(&other != this){
			instancePtr = other.instancePtr;
			fnPtr = other.fnPtr;
			std::swap(fn, other.fn);
		}
		return *this;
	}

	Ret operator()(Params... params) const {
		return fn(params...);
	}

	std::strong_ordering operator<=>(const Function<Signature>& other) const {
		// static functions are greater than member functions
		// for member functions, first compare instancePtr, then compare mfnPtr
		return (fnPtr == nullptr && other.fnPtr == nullptr)
				? instancePtr == other.instancePtr
					? mfnPtr <=> other.mfnPtr
					: instancePtr <=> other.instancePtr
				: fnPtr <=> other.fnPtr;
	}

	bool operator==(const Function<Signature>& other) const {
		return (*this) <=> other == 0;
	}
private:
	void* fnPtr = nullptr;
	void* instancePtr = nullptr;
	void* mfnPtr = nullptr;
	std::function<Signature> fn;
};

template<class... Params>
using Action = Function<void(Params...)>;

template<class... Params>
class Event {
public:
	using Signature = void(Params...);
	using FType = Action<Params...>;

	Event() {}

	void Add(FType f){
		fns.push_back(f);
	}


	/// @brief removes one callback at a time from the callback list. Does nothing if callback not registered
	/// @param f the function to be unregistered
	void Remove(FType f){
		auto it = std::find(fns.rbegin(), fns.rend(), f);
		if(it != fns.rend()){
			++it; // because it.base() is shifted by 1
			fns.erase(it.base());
		}
	}

	/// @brief removes one callback at a time from the callback list. Does nothing if callback not registered
	/// @param f the function to be unregistered
	void RemoveAll(FType f){
		fns.erase(f);
	}

	void Clear(){
		fns.clear();
	}

	void Invoke(Params... p) const {
		for(const FType& fn : fns){
			fn(p...);
		}
	}

	void operator()(Params... p) const {
		Invoke(p...);
	}

	void operator+=(FType other){
		Add(other);
	}

	void operator-=(FType other){
		Remove(other);
	}
private:
	struct FnPtrCompare {
		bool operator()(const FType* lhs, const FType* rhs) const {
			return (*lhs) < (*rhs);
		}
	};

	std::list<FType> fns;
};

}