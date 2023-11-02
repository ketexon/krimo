#pragma once

#include <type_traits>

namespace krimo {

using TID = size_t;

namespace impl {
	inline TID TypeIDIncrement(){
		static TID i = 0;
		return ++i;
	}

	template<class T>
	TID TypeIDImpl(){
		static TID id = TypeIDIncrement();
		return id;
	}
}


template<class T>
TID TypeID() {
	return impl::TypeIDImpl<std::decay_t<T>>();
};

class ITypeID {
public:
	virtual TID TypeID() = 0;
};

#define KRIMO_IMPL_ITYPEID(ClassName) public: TID TypeID() override { return ::krimo::TypeID<ClassName>(); }

}