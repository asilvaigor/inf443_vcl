//****************************************************
//* Quaternion.h                                     *
//*                                                  *
//* Implementaion for a generalized Quaternion class *
//*                                                  *
//* Written 1.25.00 by Angela Bennett                *
//****************************************************

#ifndef PGM_QUATERNION_H
#define PGM_QUATERNION_H

#include <iostream>
#include <math.h>

namespace vcl {

class Quaternion {
public:

    //Quaternion
    // -default constructor
    // -creates a new Quaternion with all parts equal to zero
    Quaternion();

    //Quaternion
    // -constructor
    // -parametes : w, x, y, z elements of the Quaternion
    // -creates a new Quaternion based on the elements passed in
    Quaternion(float wi, float xi, float yi, float zi);

    //Quaternion
    // -copy constructor
    // -parameters : const Quaternion q
    // -creates a new Quaternion based on the Quaternion passed in
    Quaternion(const Quaternion &q);

    //Quaternion
    // -constructor
    // -parameters : yaw, pitch, and roll of an Euler angle
    // -creates a new Quaternion based on the Euler elements passed in
    explicit Quaternion(vec3 &e);

    //Quaternion
    // -constructor
    // -parameters : x, y, z elements of a vector, angle
    // -creates a new Quaternion based on the axis given, rotated by an angle
    Quaternion(vec3 &v, float angle);

    //Quaternion
    // -default destructor
    ~Quaternion();

    //operator=
    // -parameters : q1- Quaternion object
    // -return values : Quaternion
    // -when called on Quaternion q2 sets q2 to be an object of  q3
    Quaternion operator=(const Quaternion &q);

    //operator+
    // -parameters : q1 - Quaternion object
    // -return value : Quaternion
    // -when called on Quaternion q2 adds q1 + q2 and returns the sum in a new Quaternion
    Quaternion operator+(const Quaternion &q);

    //operator-
    // -parameters : q1- Quaternion object
    // -return values : Quaternion
    // -when called on q1 subtracts q1 - q2 and returns the difference as a new Quaternion
    Quaternion operator-(const Quaternion &q);

    //operator*
    // -parameters : q1 - Quaternion object
    // -return values : Quaternion
    // -when called on a Quaternion q2, multiplies q2 *q1  and returns the product in a new Quaternion
    Quaternion operator*(const Quaternion &q);

    //operator/
    // -parameters : q1 and q2- Quaternion objects
    // -return values : Quaternion
    // -divide q1 by q2 and returns the quotient as q1
    Quaternion operator/(Quaternion &q);

    //operator+=
    // -parameters : q1- Quaternion object
    // -return values : Quaternion
    // -when called on Quaternion q3 adds q1 and q3 and returns the sum as q3
    Quaternion &operator+=(const Quaternion &q);

    //operator-=
    // -parameters : q1- Quaternion object
    // -return values : Quaternion
    // -when called on Quaternion q3, subtracts q1 from q3 and returns the difference as q3
    Quaternion &operator-=(const Quaternion &q);

    //operator*=
    // -parameters : q1- Quaternion object
    // -return values : Quaternion
    // -when called on Quaternion q3, multiplies q3 by q1 and returns the product as q3
    Quaternion &operator*=(const Quaternion &q);

    //operator/=
    // -parameters : q1- Quaternion object
    // -return values : Quaternion
    // -when called on Quaternion q3, divides q3 by q1 and returns the quotient as q3
    Quaternion &operator/=(Quaternion &q);

    //operator!=
    // -parameters : q1 and q2- Quaternion objects
    // -return value : bool
    // -determines if q1 and q2 and equal
    bool operator!=(const Quaternion &q);

    //operator==
    // -parameters : q1 and q2- Quaternion objects
    // -return value : bool
    // -determines if q1 and q2 and equal
    bool operator==(const Quaternion &q);

    //operator<<
    // -parameters : ostream o, Quaternion q
    // -return values :
    // -prints out a Quaternion by it's components
    friend inline std::ostream &operator<<(std::ostream &output, const Quaternion &q) {
        output << "[" << q.w << ", " << "(" << q.x << ", " << q.y << ", " << q.z << ")]";
        return output;
    }

    //other methods: norm, inverse, conjugate, toEuler

    //norm
    // -parameters : none
    // -return value : float
    // -when called on a Quaternion object q, returns the norm of q
    float norm();

    //scale
    // -parameters :  s- a value to scale q1 by
    // -return value: Quaternion
    // -returns the original Quaternion with each part, w,x,y,z, multiplied by some scalar s
    Quaternion scale(float s);

    //inverse
    // -parameters : none
    // -return value : Quaternion
    // -when called on a Quaternion object q, returns the inverse of q
    Quaternion inverse();

    //conjugate
    // -parameters : none
    // -return value : Quaternion
    // -when called on a Quaternion object q, returns the conjugate of q
    Quaternion conjugate();

    //Unitquaternion
    // -parameters : none
    // -return value : Quaternion
    // -when called on quaterion q, takes q and returns the unit Quaternion of q
    Quaternion normalized();

    // -parameters : 3D vector of type float
    // -return value : void
    // -when given a  3D vector, v, rotates v by the Quaternion
    vec3 rotated(vec3 &v);

    vec3 toEuler();

private:
    // [w, (x, y, z)]
    float w, x, y, z;
};

}

#endif //PGM_QUATERNION_H