#pragma once

#include <type_traits>

namespace krimo {


template<class T, bool Readonly = false>
class Property {
public:
	using QualifiedRefT = std::conditional_t<Readonly, const T&, T&>;
private:
	QualifiedRefT value;

public:
	Property(QualifiedRefT value) : value{value}
	{}

	operator const QualifiedRefT() const {
		return value;
	}

	Property<T, Readonly>& operator=(T value) {
#ifdef KRIMO_READONLY_PROPERTY_SILENTLY_FAIL
		if constexpr (!Readonly){
#endif
			this->value = value;
#ifdef KRIMO_READONLY_PROPERTY_SILENTLY_FAIL
		}
#else
		static_assert(!Readonly, "Cannot assign to a readonly property");
#endif

		return *this;
	}
};

template<class T>
using ReadonlyProperty = Property<T, true>;

}