#pragma once

#include "vcl/containers/buffer_stack/buffer_stack.hpp"
#include <iostream>

namespace vcl {

/** vec3 is an alias on a generic buffer_stack<float, 3> */
using vec3 = buffer_stack<float, 3>;

/** vec3 models a lightweight (x,y,z) 3D coordinates vector or point.
 * vec3 is a specialized-template class from a generic buffer_stack<type,N>, with type=float and N=3 */
template <> struct buffer_stack<float, 3> {

    // Public attributes:
    float x; /**< x-coordinate or vec3[0]  */
    float y; /**< y-coordinate or vec3[1] */
    float z; /**< z-coordinate or vec3[2] */

    /** \name Constructors */
    ///@{
    /** Empty constructor initialize vec3=(0,0,0) */
    buffer_stack<float, 3>();
    /** Direct constructor.
     * vec3(x,y,z), or vec3{x,y,z}, or vec3 p = {x,y,z}; */
	buffer_stack<float, 3>(float x,float y,float z);
    ///@}

    /** Return 3 */
    size_t size() const;

    /** \name Element access
     * \brief  Allow vec3[0/1/2], or vec3(0/1/2), or vec3.at(0/1/2) */
    ///@{
    const float& operator[](std::size_t index) const;
    float& operator[](std::size_t index);

    const float& operator()(std::size_t index) const;
    float& operator()(std::size_t index);

    float const& at(std::size_t index) const;
    float& at(std::size_t index);
    ///@}


    /** \name Iterators
     * Iterators are direct pointers on data */
    ///@{
    float* begin();
    float* end();
    float const* begin() const;
    float const* end() const;
    float const* cbegin() const;
    float const* cend() const;
    ///@}

    /**
     * Norm of the vec3.
     */
    float norm();

    /**
     * Normalized vec3, its norm will now be 1.
     */
    vec3 normalized();

    /**
     * Distance to another vec3.
     */
    float dist(vec3 &v);

    friend inline std::ostream &operator<<(std::ostream &output, const vec3 &v) {
        output << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return output;
    }
};

/** Cross product between two vec3.
 * \relates buffer_stack<float,3> */
vec3 cross(const vec3& a,const vec3& b);

/**
 * Random normalized vec3.
 */
vec3 random();

}
