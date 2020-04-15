//****************************************************
//* Quaternion.c++                                   *
//*                                                  *
//* Implementaion for a generalized Quaternion class *   
//*                                                  *
//* Written 1.25.00 by Angela Bennett                *
//****************************************************

#include <math.h>
#include "vcl/math/vec/vec.hpp"
#include "Quaternion.hpp"

namespace vcl {

//Quaternion
// -default constructor
// -creates a new Quaternion with all parts equal to zero
Quaternion::Quaternion() {
    w = 0;
    x = 0;
    y = 0;
    z = 0;
}


//Quaternion
// -constructor
// -parametes : x, y, z, w elements of the Quaternion
// -creates a new Quaternion based on the elements passed in
Quaternion::Quaternion(float wi, float xi, float yi, float zi) {
    w = wi;
    x = xi;
    y = yi;
    z = zi;
}


//Quaternion
// -copy constructor
// -parameters : const Quaternion q
// -creates a new Quaternion based on the Quaternion passed in
Quaternion::Quaternion(const Quaternion &q) {
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
}

//Quaternion
// -constructor
// -parameters : roll, pitch, and yaw of an Euler angle
// -creates a new Quaternion based on the Euler elements passed in
Quaternion::Quaternion(vec3 &e) {
    x = sin(e.x / 2) * cos(e.y / 2) * cos(e.z / 2) - cos(e.x / 2) * sin(e.y / 2) * sin(e.z / 2);
    y = cos(e.x / 2) * sin(e.y / 2) * cos(e.z / 2) + sin(e.x / 2) * cos(e.y / 2) * sin(e.z / 2);
    z = cos(e.x / 2) * cos(e.y / 2) * sin(e.z / 2) - sin(e.x / 2) * sin(e.y / 2) * cos(e.z / 2);
    w = cos(e.x / 2) * cos(e.y / 2) * cos(e.z / 2) + sin(e.x / 2) * sin(e.y / 2) * sin(e.z / 2);
}

Quaternion::Quaternion(vec3 &v, float angle) {
    x = v.x * sin(angle / 2);
    y = v.y * sin(angle / 2);
    z = v.z * sin(angle / 2);
    w = cos(angle / 2);
}

//~Quaternion
// -destructor
// -deleted dynamically allocated memory
Quaternion::~Quaternion() = default;


//operator=
// -parameters : q1 - Quaternion object
// -return value : Quaternion
// -when called on Quaternion q2 sets q2 to be an object of  q3 
Quaternion Quaternion::operator=(const Quaternion &q) {
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;

    return *this;
}

//operator+
// -parameters : q1 - Quaternion object
// -return value : Quaternion 
// -when called on Quaternion q2 adds q1 + q2 and returns the sum in a new Quaternion
Quaternion Quaternion::operator+(const Quaternion &q) {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

//operator-
// -parameters : q1- Quaternion object
// -return values : Quaternion 
// -when called on q1 subtracts q1 - q2 and returns the difference as a new Quaternion
Quaternion Quaternion::operator-(const Quaternion &q) {
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}


//operator*
// -parameters : q1 - Quaternion object
// -return values : Quaternion 
// -when called on a Quaternion q2, multiplies q2 *q1  and returns the product in a new Quaternion 
Quaternion Quaternion::operator*(const Quaternion &q) {
    return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y + y * q.w + z * q.x - x * q.z,
            w * q.z + z * q.w + x * q.y - y * q.x);
}

//operator/
// -parameters : q1 and q2- Quaternion objects
// -return values : Quaternion 
// -divide q1 by q2 and returns the quotient q1
Quaternion Quaternion::operator/(Quaternion &q) {
    return ((*this) * (q.inverse()));
}


//operator+=
// -parameters : q1- Quaternion object
// -return values : Quaternion 
// -when called on Quaternion q3, adds q1 and q3 and returns the sum as q3
Quaternion &Quaternion::operator+=(const Quaternion &q) {
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;

    return (*this);
}


//operator-=
// -parameters : q1- Quaternion object
// -return values : Quaternion 
// -when called on Quaternion q3, subtracts q1 from q3 and returns the difference as q3
Quaternion &Quaternion::operator-=(const Quaternion &q) {
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;

    return (*this);
}


//operator*=
// -parameters : q1- Quaternion object
// -return values : Quaternion 
// -when called on Quaternion q3, multiplies q3 by q1 and returns the product as q3
Quaternion &Quaternion::operator*=(const Quaternion &q) {
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
// -parameters : q1- Quaternion object
// -return values : Quaternion
// -when called on Quaternion q3, divides q3 by q1 and returns the quotient as q3
Quaternion &Quaternion::operator/=(Quaternion &q) {
    (*this) = (*this) * q.inverse();
    return (*this);
}


//operator!=
// -parameters : q1 and q2- Quaternion objects
// -return value : bool
// -determines if q1 and q2 are not equal
bool Quaternion::operator!=(const Quaternion &q) {
    return w != q.w || x != q.x || y != q.y || z != q.z;
}

//operator==
// -parameters : q1 and q2- Quaternion objects
// -return value : bool
// -determines if q1 and q2 are equal
bool Quaternion::operator==(const Quaternion &q) {
    return w == q.w && x == q.x && y == q.y && z == q.z;
}

//norm
// -parameters : none
// -return value : float
// -when called on a Quaternion object q, returns the norm of q
float Quaternion::norm() {
    return (w * w + x * x + y * y + z * z);
}

//scale
// -parameters :  s- a value to scale q1 by
// -return value: Quaternion
// -returns the original Quaternion with each part, w,x,y,z, multiplied by some scalar s
Quaternion Quaternion::scale(float s) {
    return Quaternion(w * s, x * s, y * s, z * s);
}

// -parameters : none
// -return value : Quaternion
// -when called on a Quaternion object q, returns the inverse of q
Quaternion Quaternion::inverse() {
    return conjugate().scale(1 / norm());
}

//conjugate
// -parameters : none
// -return value : Quaternion
// -when called on a Quaternion object q, returns the conjugate of q
Quaternion Quaternion::conjugate() {
    return Quaternion(w, -x, -y, -z);
}

//UnitQuaternion
// -parameters : none
// -return value : Quaternion
// -when called on quaterion q, takes q and returns the unit Quaternion of q
Quaternion Quaternion::normalized() {
    return (*this).scale(1 / norm());
}

// -parameters : vector of type float
// -return value : void
// -when given a 3D vector, v, rotates v by this Quaternion
vec3 Quaternion::rotated(vec3 &v) {
    Quaternion qv(0, v.x, v.y, v.z);
    Quaternion qm = (*this) * qv * (*this).inverse();

    v.x = qm.x;
    v.y = qm.y;
    v.z = qm.z;
    return v;
}

vec3 Quaternion::toEuler() {
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

}
