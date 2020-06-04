#include "special_transform.hpp"
#include "vcl/math/math.hpp"

namespace vcl
{

mat3 rotation_from_axis_angle_mat3(const vec3& axis, float angle)
{
    const vec3 u = normalize(axis);
    const float x = u.x;
    const float y = u.y;
    const float z = u.z;
    const float c = std::cos(angle);
    const float s = std::sin(angle);

    return mat3 {c+x*x*(1-c)  , x*y*(1-c)-z*s, x*z*(1-c)+y*s,
                 y*x*(1-c)+z*s, c+y*y*(1-c)  , y*z*(1-c)-x*s,
                 z*x*(1-c)-y*s, z*y*(1-c)+x*s, c+z*z*(1-c)   };
}

mat3 rotation_between_vector_mat3(const vec3& a, const vec3& b)
{
    const vec3 u0 = normalize(a);
    const vec3 u1 = normalize(b);

    if( norm(u0-u1)<1e-4f )
        return mat3::identity();
    if( norm(u0+u1)<1e-4f )
        return -mat3::identity();

    const float d = dot(u0,u1);
    const float angle = std::acos( d );

    vec3 axis = normalize(cross(u0,u1));

    return rotation_from_axis_angle_mat3(axis,angle);
}

mat3 rotation_euler(vcl::vec3& orientation, float rotation){
    // Rotation using euler angles and basis on y
    vcl::vec3 projDpXY = {orientation.x, orientation.y, 0};

    float alpha;
    vcl::mat3 aRotation;
    if (projDpXY.norm() < 1e-6)
        aRotation = {{1, 0, 0},{0, 1, 0},{0, 0, 1}};
    else{
        if (vcl::cross(projDpXY, {0, 1, 0}).z > 0)
            alpha = -projDpXY.angle({0, 1, 0});
        else alpha = projDpXY.angle({0, 1, 0});
        aRotation = vcl::rotation_from_axis_angle_mat3({0, 0, 1}, alpha);
    }

    vcl::vec3 bAxis = vcl::cross(orientation, {0, 0, 1});
    float beta = -orientation.angle({0, 0, 1})+M_PI_2;
    vcl::mat3 bRotation = vcl::rotation_from_axis_angle_mat3(bAxis, beta);

    vcl::mat3 cRotation = vcl::rotation_from_axis_angle_mat3(orientation, rotation);

    return cRotation*bRotation*aRotation;
}

}
