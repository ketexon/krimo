#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace krimo {

template<size_t N, class T = float>
using Vec = glm::vec<N, T>;

template<class T = float>
using Vec1 = Vec<1, T>;
using FVec1 = Vec1<float>;
using DVec1 = Vec1<double>;

template<class T = float>
using Vec2 = Vec<2, T>;
using FVec2 = Vec2<float>;
using DVec2 = Vec2<double>;

template<class T = float>
using Vec3 = Vec<3, T>;
using FVec3 = Vec3<float>;
using DVec3 = Vec3<double>;

template<class T = float>
using Vec4 = Vec<4, T>;
using FVec4 = Vec4<float>;
using DVec4 = Vec4<double>;

template<class T = float>
using Quat = glm::qua<T>;
using FQuat = Quat<float>;
using DQuat = Quat<double>;

}