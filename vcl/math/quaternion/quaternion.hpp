//****************************************************
//* quaternion.h                                     *
//*                                                  *
//* Implementaion for a generalized quaternion class *
//*                                                  *
//* Written 1.25.00 by Angela Bennett                *
//****************************************************

#ifndef PGM_QUATERNION_H
#define PGM_QUATERNION_H

#include <iostream>
#include <math.h>

namespace vcl {

class quaternion {
public:

    //quaternion
    // -default constructor
    // -creates a new quaternion with all parts equal to zero
    quaternion();

    //quaternion
    // -constructor
    // -parametes : w, x, y, z elements of the quaternion
    // -creates a new quaternion based on the elements passed in
    quaternion(float wi, float xi, float yi, float zi);

    //quaternion
    // -copy constructor
    // -parameters : const quaternion q
    // -creates a new quaternion based on the quaternion passed in
    quaternion(const quaternion &q);

    //quaternion
    // -constructor
    // -parameters : yaw, pitch, and roll of an Euler angle
    // -creates a new quaternion based on the Euler elements passed in
    explicit quaternion(vec3 &e);

    //quaternion
    // -constructor
    // -parameters : x, y, z elements of a vector, angle
    // -creates a new quaternion based on the axis given, rotated by an angle
    quaternion(vec3 &v, float angle);

    //quaternion
    // -default destructor
    ~quaternion();

    //operator=
    // -parameters : q1- quaternion object
    // -return values : quaternion
    // -when called on quaternion q2 sets q2 to be an object of  q3
    quaternion operator=(const quaternion &q);

    //operator+
    // -parameters : q1 - quaternion object
    // -return value : quaternion
    // -when called on quaternion q2 adds q1 + q2 and returns the sum in a new quaternion
    quaternion operator+(const quaternion &q);

    //operator-
    // -parameters : q1- quaternion object
    // -return values : quaternion
    // -when called on q1 subtracts q1 - q2 and returns the difference as a new quaternion
    quaternion operator-(const quaternion &q);

    //operator*
    // -parameters : q1 - quaternion object
    // -return values : quaternion
    // -when called on a quaternion q2, multiplies q2 *q1  and returns the product in a new quaternion
    quaternion operator*(const quaternion &q);

    //operator/
    // -parameters : q1 and q2- quaternion objects
    // -return values : quaternion
    // -divide q1 by q2 and returns the quotient as q1
    quaternion operator/(quaternion &q);

    //operator+=
    // -parameters : q1- quaternion object
    // -return values : quaternion
    // -when called on quaternion q3 adds q1 and q3 and returns the sum as q3
    quaternion &operator+=(const quaternion &q);

    //operator-=
    // -parameters : q1- quaternion object
    // -return values : quaternion
    // -when called on quaternion q3, subtracts q1 from q3 and returns the difference as q3
    quaternion &operator-=(const quaternion &q);

    //operator*=
    // -parameters : q1- quaternion object
    // -return values : quaternion
    // -when called on quaternion q3, multiplies q3 by q1 and returns the product as q3
    quaternion &operator*=(const quaternion &q);

    //operator/=
    // -parameters : q1- quaternion object
    // -return values : quaternion
    // -when called on quaternion q3, divides q3 by q1 and returns the quotient as q3
    quaternion &operator/=(quaternion &q);

    //operator!=
    // -parameters : q1 and q2- quaternion objects
    // -return value : bool
    // -determines if q1 and q2 and equal
    bool operator!=(const quaternion &q);

    //operator==
    // -parameters : q1 and q2- quaternion objects
    // -return value : bool
    // -determines if q1 and q2 and equal
    bool operator==(const quaternion &q);

    //operator<<
    // -parameters : ostream o, quaternion q
    // -return values :
    // -prints out a quaternion by it's components
    friend inline std::ostream &operator<<(std::ostream &output, const quaternion &q) {
        output << "[" << q.w << ", " << "(" << q.x << ", " << q.y << ", " << q.z << ")]";
        return output;
    }

    //other methods: norm, inverse, conjugate, toEuler

    //norm
    // -parameters : none
    // -return value : float
    // -when called on a quaternion object q, returns the norm of q
    float norm();

    //scale
    // -parameters :  s- a value to scale q1 by
    // -return value: quaternion
    // -returns the original quaternion with each part, w,x,y,z, multiplied by some scalar s
    quaternion scale(float s);

    //inverse
    // -parameters : none
    // -return value : quaternion
    // -when called on a quaternion object q, returns the inverse of q
    quaternion inverse();

    //conjugate
    // -parameters : none
    // -return value : quaternion
    // -when called on a quaternion object q, returns the conjugate of q
    quaternion conjugate();

    //Unitquaternion
    // -parameters : none
    // -return value : quaternion
    // -when called on quaterion q, takes q and returns the unit quaternion of q
    quaternion normalized();

    // -parameters : 3D vector of type float
    // -return value : void
    // -when given a  3D vector, v, rotates v by the quaternion
    vec3 rotated(vec3 &v);

    vec3 toEuler();

private:
    // [w, (x, y, z)]
    float w, x, y, z;
};

}

#endif //PGM_QUATERNION_H