//****************************************************
//* quaternion.c++                                   *
//*                                                  *
//* Implementaion for a generalized quaternion class *
//*                                                  *
//* Written 1.25.00 by Angela Bennett                *
//****************************************************

#include <math.h>
#include "vcl/math/vec/vec.hpp"
#include "vcl/math/mat/mat.hpp"
#include "quaternion.hpp"

namespace vcl {

//quaternion
// -default constructor
// -creates a new quaternion with all parts equal to zero
quaternion::quaternion() {
    w = 0;
    x = 0;
    y = 0;
    z = 0;
}


//quaternion
// -constructor
// -parametes : x, y, z, w elements of the quaternion
// -creates a new quaternion based on the elements passed in
quaternion::quaternion(float wi, float xi, float yi, float zi) {
    w = wi;
    x = xi;
    y = yi;
    z = zi;
}


//quaternion
// -copy constructor
// -parameters : const quaternion q
// -creates a new quaternion based on the quaternion passed in
quaternion::quaternion(const quaternion &q) {
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
}

//quaternion
// -constructor
// -parameters : roll, pitch, and yaw of an Euler angle
// -creates a new quaternion based on the Euler elements passed in
quaternion::quaternion(vec3 &e) {
    x = sin(e.x / 2) * cos(e.y / 2) * cos(e.z / 2) - cos(e.x / 2) * sin(e.y / 2) * sin(e.z / 2);
    y = cos(e.x / 2) * sin(e.y / 2) * cos(e.z / 2) + sin(e.x / 2) * cos(e.y / 2) * sin(e.z / 2);
    z = cos(e.x / 2) * cos(e.y / 2) * sin(e.z / 2) - sin(e.x / 2) * sin(e.y / 2) * cos(e.z / 2);
    w = cos(e.x / 2) * cos(e.y / 2) * cos(e.z / 2) + sin(e.x / 2) * sin(e.y / 2) * sin(e.z / 2);
}

quaternion::quaternion(vec3 &v, float angle) {
    x = v.x * sin(angle / 2);
    y = v.y * sin(angle / 2);
    z = v.z * sin(angle / 2);
    w = cos(angle / 2);
}

//quaternion
// -destructor
// -deleted dynamically allocated memory
quaternion::~quaternion() = default;


//operator=
// -parameters : q1 - quaternion object
// -return value : quaternion
// -when called on quaternion q2 sets q2 to be an object of  q3
quaternion quaternion::operator=(const quaternion &q) {
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;

    return *this;
}

//operator+
// -parameters : q1 - quaternion object
// -return value : quaternion
// -when called on quaternion q2 adds q1 + q2 and returns the sum in a new quaternion
quaternion quaternion::operator+(const quaternion &q) {
    return quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

//operator-
// -parameters : q1- quaternion object
// -return values : quaternion
// -when called on q1 subtracts q1 - q2 and returns the difference as a new quaternion
quaternion quaternion::operator-(const quaternion &q) {
    return quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}


//operator*
// -parameters : q1 - quaternion object
// -return values : quaternion
// -when called on a quaternion q2, multiplies q2 *q1  and returns the product in a new quaternion
quaternion quaternion::operator*(const quaternion &q) {
    return quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y + y * q.w + z * q.x - x * q.z,
            w * q.z + z * q.w + x * q.y - y * q.x);
}

//operator/
// -parameters : q1 and q2- quaternion objects
// -return values : quaternion
// -divide q1 by q2 and returns the quotient q1
quaternion quaternion::operator/(quaternion &q) {
    return ((*this) * (q.inverse()));
}


//operator+=
// -parameters : q1- quaternion object
// -return values : quaternion
// -when called on quaternion q3, adds q1 and q3 and returns the sum as q3
quaternion &quaternion::operator+=(const quaternion &q) {
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;

    return (*this);
}


//operator-=
// -parameters : q1- quaternion object
// -return values : quaternion
// -when called on quaternion q3, subtracts q1 from q3 and returns the difference as q3
quaternion &quaternion::operator-=(const quaternion &q) {
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;

    return (*this);
}


//operator*=
// -parameters : q1- quaternion object
// -return values : quaternion
// -when called on quaternion q3, multiplies q3 by q1 and returns the product as q3
quaternion &quaternion::operator*=(const quaternion &q) {
    float w_val = w * q.w - x * q.x - y * q.y - z * q.z;
    float x_val = w * q.x + x * q.w + y * q.z - z * q.y;
    float y_val = w * q.y + y * q.w + z * q.x - x * q.z;
    float z_val = w * q.z + z * q.w + x * q.y - y * q.x;

    w = w_val;
    x = x_val;
    y = y_val;
    z = z_val;

    return (*this);
}


//operator/=
// -parameters : q1- quaternion object
// -return values : quaternion
// -when called on quaternion q3, divides q3 by q1 and returns the quotient as q3
quaternion &quaternion::operator/=(quaternion &q) {
    (*this) = (*this) * q.inverse();
    return (*this);
}


//operator!=
// -parameters : q1 and q2- quaternion objects
// -return value : bool
// -determines if q1 and q2 are not equal
bool quaternion::operator!=(const quaternion &q) {
    return w != q.w || x != q.x || y != q.y || z != q.z;
}

//operator==
// -parameters : q1 and q2- quaternion objects
// -return value : bool
// -determines if q1 and q2 are equal
bool quaternion::operator==(const quaternion &q) {
    return w == q.w && x == q.x && y == q.y && z == q.z;
}

//norm
// -parameters : none
// -return value : float
// -when called on a quaternion object q, returns the norm of q
float quaternion::norm() {
    return (w * w + x * x + y * y + z * z);
}

//scale
// -parameters :  s- a value to scale q1 by
// -return value: quaternion
// -returns the original quaternion with each part, w,x,y,z, multiplied by some scalar s
quaternion quaternion::scale(float s) {
    return quaternion(w * s, x * s, y * s, z * s);
}

// -parameters : none
// -return value : quaternion
// -when called on a quaternion object q, returns the inverse of q
quaternion quaternion::inverse() {
    return conjugate().scale(1 / norm());
}

//conjugate
// -parameters : none
// -return value : quaternion
// -when called on a quaternion object q, returns the conjugate of q
quaternion quaternion::conjugate() {
    return quaternion(w, -x, -y, -z);
}

//UnitQuaternion
// -parameters : none
// -return value : quaternion
// -when called on quaterion q, takes q and returns the unit quaternion of q
quaternion quaternion::normalized() {
    return (*this).scale(1 / norm());
}

// -parameters : vector of type float
// -return value : void
// -when given a 3D vector, v, rotates v by this quaternion
vec3 quaternion::rotated(vec3 &v) {
    quaternion qv(0, v.x, v.y, v.z);
    quaternion qm = (*this) * qv * (*this).inverse();

    v.x = qm.x;
    v.y = qm.y;
    v.z = qm.z;
    return v;
}

vec3 quaternion::toEuler() {
    float t0 = +2.0f * (w * x + y * z);
    float t1 = +1.0f - 2.0f * (x * x + y * y);
    float roll = atan2(t0, t1);

    float t2 = +2.0f * (w * y - z * x);
    t2 = clamp(t2, -1.0f, 1.0f);
    float pitch = asin(t2);

    float t3 = +2.0f * (w * z + x * y);
    float t4 = +1.0f - 2.0f * (y * y + z * z);
    float yaw = atan2(t3, t4);

    return {roll, pitch, yaw};
}

mat3 quaternion::getRotationMatrix() {
        float norm = this->norm();
        float x = this->x/norm;
        float y = this->y/norm;
        float z = this->z/norm;
        return {w*w+x*x-y*y-z*z, 2*x*y-2*w*z, 2*x*z+2*w*y,
                2*x*y+2*w*z, w*w+y*y-x*x-z*z, 2*y*z-2*w*x,
                2*x*z-2*w*y, 2*y*z+2*w*x, w*w+z*z-x*x-y*y};
    }

}
