#pragma once

#include <type_traits>

namespace krimo {

using TID = size_t;

namespace impl {
	TID TypeIDIncrement(){
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
	return impl::TypeIDImpl<T>();
};

}