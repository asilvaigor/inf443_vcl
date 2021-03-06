#include "mat4.hpp"

#include "../../mat/mat3/mat3.hpp"
#include "../../vec/vec3/vec3.hpp"

#include <iostream>

namespace vcl {



mat4::mat()
    :xx(),xy(),xz(),xw(),
      yx(),yy(),yz(),yw(),
      zx(),zy(),zz(),zw(),
      wx(),wy(),wz(),ww()
{}
mat4::mat(float xx_arg,float xy_arg,float xz_arg,float xw_arg,
          float yx_arg,float yy_arg,float yz_arg,float yw_arg,
          float zx_arg,float zy_arg,float zz_arg,float zw_arg,
          float wx_arg,float wy_arg,float wz_arg,float ww_arg)
    :xx(xx_arg),xy(xy_arg),xz(xz_arg),xw(xw_arg),
      yx(yx_arg),yy(yy_arg),yz(yz_arg),yw(yw_arg),
      zx(zx_arg),zy(zy_arg),zz(zz_arg),zw(zw_arg),
      wx(wx_arg),wy(wy_arg),wz(wz_arg),ww(ww_arg)
{}

mat4::mat(const vcl::mat3& R, const vcl::vec3& t)
    :xx(R(0,0)),xy(R(0,1)),xz(R(0,2)),xw(t.x),
      yx(R(1,0)),yy(R(1,1)),yz(R(1,2)),yw(t.y),
      zx(R(2,0)),zy(R(2,1)),zz(R(2,2)),zw(t.z),
      wx(0),wy(0),wz(0),ww(1.0f)
{
}

mat4 mat4::identity()
{
    return mat4(1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1);
}

mat4 mat4::zero()
{
    return mat4(0,0,0,0,
                0,0,0,0,
                0,0,0,0,
                0,0,0,0);
}

mat4 mat4::perspective(float angle_of_view, float image_aspect, float z_near, float z_far)
{
    const float fy = 1/std::tan(angle_of_view/2);
    const float fx = fy/image_aspect;
    const float L = z_near-z_far;

    const float C = (z_far+z_near)/L;
    const float D = (2*z_far*z_near)/L;

    return {
        fx,0,0,0,
        0,fy,0,0,
        0,0,C,D,
        0,0,-1,0
    };
}

mat4 mat4::from_scaling(float s)
{
    return {
        s,0,0,0,
        0,s,0,0,
        0,0,s,0,
        0,0,0,1
    };
}
mat4 mat4::from_scaling(const vcl::vec3& s)
{
    return {
        s.x,0  ,0  ,0,
        0  ,s.y,0  ,0,
        0  ,0  ,s.z,0,
        0  ,0  ,0  ,1
    };
}

mat4 mat4::from_mat3(const vcl::mat3& m)
{
    return {
        m.xx, m.xy, m.xz, 0,
        m.yx, m.yy, m.yz, 0,
        m.zx, m.zy, m.zz, 0,
          0 ,  0  ,  0  , 1
    };
}
mat4 mat4::from_translation(const vcl::vec3& t)
{
    return {
        1,0,0,t.x,
        0,1,0,t.y,
        0,0,1,t.z,
        0,0,0,1
    };
}
mat4 mat4::from_assimp(const aiMatrix4x4& m) {
    return {
        m.a1, m.a2, m.a3, m.a4,
        m.b1, m.b2, m.b3, m.b4,
        m.c1, m.c2, m.c3, m.c4,
        m.d1, m.d2, m.d3, m.d4
    };
}



vec4 mat4::row(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return {xx,xy,xz,xw};
    case 1:
        return {yx,yy,yz,yw};
    case 2:
        return {zx,zy,zz,zw};
    case 3:
        return {wx,wy,wz,ww};
    default:
        std::cerr<<"Error: Try to access mat4.row("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
vec4 mat4::col(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return {xx,yx,zx,wx};
    case 1:
        return {xy,yy,zy,wy};
    case 2:
        return {xz,yz,zz,wz};
    case 3:
        return {xw,yw,zw,ww};
    default:
        std::cerr<<"Error: Try to access mat4.col("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
mat4& mat4::set_row(std::size_t offset, const vec4& v)
{
    switch(offset) {
    case 0:
        xx=v.x; xy=v.y; xz=v.z; wz=v.w; break;
    case 1:
        yx=v.x; yy=v.y; yz=v.z; yw=v.w; break;
    case 2:
        zx=v.x; zy=v.y; zz=v.z; zw=v.w; break;
    case 3:
        wx=v.x; wy=v.y; wz=v.z; ww=v.w; break;
    default:
        std::cerr<<"Error: Try to set mat4.row("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}
mat4& mat4::set_col(std::size_t offset, const vec4& v)
{
    switch(offset) {
    case 0:
        xx=v.x; yx=v.y; zx=v.z; wx=v.w; break;
    case 1:
        xy=v.x; yy=v.y; zy=v.z; wy=v.w; break;
    case 2:
        xz=v.x; yz=v.y; zz=v.z; wz=v.w; break;
    case 3:
        xw=v.x; yw=v.y; zw=v.z; ww=v.w; break;
    default:
        std::cerr<<"Error: Try to set mat4.col("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}

const float& mat4::operator[](std::size_t offset) const
{
    switch(offset) {

    case 0: return xx;
    case 1: return xy;
    case 2: return xz;
    case 3: return xw;

    case 4: return yx;
    case 5: return yy;
    case 6: return yz;
    case 7: return yw;

    case 8: return zx;
    case 9: return zy;
    case 10: return zz;
    case 11: return zw;

    case 12: return wx;
    case 13: return wy;
    case 14: return wz;
    case 15: return ww;

    default:
        std::cerr<<"Error: Try to access mat4["<<offset<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat4::operator[](std::size_t offset)
{
    switch(offset) {

    case 0: return xx;
    case 1: return xy;
    case 2: return xz;
    case 3: return xw;

    case 4: return yx;
    case 5: return yy;
    case 6: return yz;
    case 7: return yw;

    case 8: return zx;
    case 9: return zy;
    case 10: return zz;
    case 11: return zw;

    case 12: return wx;
    case 13: return wy;
    case 14: return wz;
    case 15: return ww;

    default:
        std::cerr<<"Error: Try to access mat4["<<offset<<"]"<<std::endl;
        assert(false);
    }
	abort();
}

const float& mat4::operator()(std::size_t index1, std::size_t index2) const
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        case 2: return xz;
        case 3: return xw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        case 2: return yz;
        case 3: return yw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 2:
        switch(index2) {
        case 0: return zx;
        case 1: return zy;
        case 2: return zz;
        case 3: return zw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 3:
        switch(index2) {
        case 0: return wx;
        case 1: return wy;
        case 2: return wz;
        case 3: return ww;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat4::operator()(std::size_t index1, std::size_t index2)
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        case 2: return xz;
        case 3: return xw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        case 2: return yz;
        case 3: return yw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 2:
        switch(index2) {
        case 0: return zx;
        case 1: return zy;
        case 2: return zz;
        case 3: return zw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 3:
        switch(index2) {
        case 0: return wx;
        case 1: return wy;
        case 2: return wz;
        case 3: return ww;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}

aiMatrix4x4 mat4::operator*(aiMatrix4x4 &m) const {
    return {
            m.a1 * xx + m.b1 * xy + m.c1 * xz + m.d1 * xw,
            m.a2 * xx + m.b2 * xy + m.c2 * xz + m.d2 * xw,
            m.a3 * xx + m.b3 * xy + m.c3 * xz + m.d3 * xw,
            m.a4 * xx + m.b4 * xy + m.c4 * xz + m.d4 * xw,
            m.a1 * yx + m.b1 * yy + m.c1 * yz + m.d1 * yw,
            m.a2 * yx + m.b2 * yy + m.c2 * yz + m.d2 * yw,
            m.a3 * yx + m.b3 * yy + m.c3 * yz + m.d3 * yw,
            m.a4 * yx + m.b4 * yy + m.c4 * yz + m.d4 * yw,
            m.a1 * zx + m.b1 * zy + m.c1 * zz + m.d1 * zw,
            m.a2 * zx + m.b2 * zy + m.c2 * zz + m.d2 * zw,
            m.a3 * zx + m.b3 * zy + m.c3 * zz + m.d3 * zw,
            m.a4 * zx + m.b4 * zy + m.c4 * zz + m.d4 * zw,
            m.a1 * wx + m.b1 * wy + m.c1 * wz + m.d1 * ww,
            m.a2 * wx + m.b2 * wy + m.c2 * wz + m.d2 * ww,
            m.a3 * wx + m.b3 * wy + m.c3 * wz + m.d3 * ww,
            m.a4 * wx + m.b4 * wy + m.c4 * wz + m.d4 * ww
    };
}

vcl::mat3 mat4::mat3() const
{
    return submat<3,3>(*this,0,0);
}
mat4& mat4::set_mat3(const vcl::mat3& m)
{
    set_submat(*this,0,0,m);
    return *this;
}
vcl::vec3 mat4::vec3() const
{
    return mat2vec(submat<3,1>(*this,0,3));
}
mat4& mat4::set_vec3(const vcl::vec3& tr)
{
    set_submat(*this,0,3,vec2mat(tr));
    return *this;

}

mat4 mat4::from_mat3_vec3(const vcl::mat3& linear_block, const vcl::vec3& translation_block)
{
    mat4 M = mat4::identity();
    M.set_mat3(linear_block);
    M.set_vec3(translation_block);
    return M;
}

void mat4::rotate_x(const float ang) {
    float sin = std::sin(ang);
    float cos = std::cos(ang);
    mat4 r(1, 0, 0, 0, 0, cos, -sin, 0, 0, sin, cos, 0, 0, 0, 0, 1);
    (*this) = r * (*this);
}
void mat4::rotate_y(const float ang) {
    float sin = std::sin(ang);
    float cos = std::cos(ang);
    mat4 r(cos, 0, -sin, 0, 0, 1, 0, 0, sin, 0, cos, 0, 0, 0, 0, 1);
    (*this) = r * (*this);
}
void mat4::rotate_z(const float ang) {
    float sin = std::sin(ang);
    float cos = std::cos(ang);
    mat4 r(cos, -sin, 0, 0, sin, cos, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1);
    (*this) = r * (*this);
}
void mat4::translate(const vcl::vec3 &v) {
    mat4 t(1, 0, 0, v.x, 0, 1, 0, v.y, 0, 0, 1, v.z, 0, 0, 0, 1);
    (*this) = (*this) * t;
}

float det(const mat4& m) {
    return m.ww*m.xx*m.yy*m.zz - m.ww*m.xx*m.yz*m.zy - m.ww*m.xy*m.yx*m.zz +
           m.ww*m.xy*m.yz*m.zx + m.ww*m.xz*m.yx*m.zy - m.ww*m.xz*m.yy*m.zx -
           m.wx*m.xw*m.yy*m.zz + m.wx*m.xw*m.yz*m.zy + m.wx*m.xy*m.yw*m.zz -
           m.wx*m.xy*m.yz*m.zw - m.wx*m.xz*m.yw*m.zy + m.wx*m.xz*m.yy*m.zw +
           m.wy*m.xw*m.yx*m.zz - m.wy*m.xw*m.yz*m.zx - m.wy*m.xx*m.yw*m.zz +
           m.wy*m.xx*m.yz*m.zw + m.wy*m.xz*m.yw*m.zx - m.wy*m.xz*m.yx*m.zw -
           m.wz*m.xw*m.yx*m.zy + m.wz*m.xw*m.yy*m.zx + m.wz*m.xx*m.yw*m.zy -
           m.wz*m.xx*m.yy*m.zw - m.wz*m.xy*m.yw*m.zx + m.wz*m.xy*m.yx*m.zw;
}
mat4 inverse(const mat4& m) {
    float d = det(m);
    assert(std::abs(d)>1e-6f);

    float xx = (m.ww*m.yy*m.zz - m.ww*m.yz*m.zy - m.wy*m.yw*m.zz + m.wy*m.yz*m.zw + m.wz*m.yw*m.zy - m.wz*m.yy*m.zw) / d;
    float xy = -(m.ww*m.xy*m.zz - m.ww*m.xz*m.zy - m.wy*m.xw*m.zz + m.wy*m.xz*m.zw + m.wz*m.xw*m.zy - m.wz*m.xy*m.zw) / d;
    float xz = (m.ww*m.xy*m.yz - m.ww*m.xz*m.yy - m.wy*m.xw*m.yz + m.wy*m.xz*m.yw + m.wz*m.xw*m.yy - m.wz*m.xy*m.yw) / d;
    float xw = -(m.xw*m.yy*m.zz - m.xw*m.yz*m.zy - m.xy*m.yw*m.zz + m.xy*m.yz*m.zw + m.xz*m.yw*m.zy - m.xz*m.yy*m.zw) / d;

    float yx = -(m.ww*m.yx*m.zz - m.ww*m.yz*m.zx - m.wx*m.yw*m.zz + m.wx*m.yz*m.zw + m.wz*m.yw*m.zx - m.wz*m.yx*m.zw) /d;
    float yy = (m.ww*m.xx*m.zz - m.ww*m.xz*m.zx - m.wx*m.xw*m.zz + m.wx*m.xz*m.zw + m.wz*m.xw*m.zx - m.wz*m.xx*m.zw) / d;
    float yz = -(m.ww*m.xx*m.yz - m.ww*m.xz*m.yx - m.wx*m.xw*m.yz + m.wx*m.xz*m.yw + m.wz*m.xw*m.yx - m.wz*m.xx*m.yw) / d;
    float yw = (m.xw*m.yx*m.zz - m.xw*m.yz*m.zx - m.xx*m.yw*m.zz + m.xx*m.yz*m.zw + m.xz*m.yw*m.zx - m.xz*m.yx*m.zw) / d;

    float zx = (m.ww*m.yx*m.zy - m.ww*m.yy*m.zx - m.wx*m.yw*m.zy + m.wx*m.yy*m.zw + m.wy*m.yw*m.zx - m.wy*m.yx*m.zw) / d;
    float zy = (m.ww*m.xy*m.zx - m.ww*m.xx*m.zy + m.wx*m.xw*m.zy - m.wx*m.xy*m.zw - m.wy*m.xw*m.zx + m.wy*m.xx*m.zw) / d;
    float zz = (m.ww*m.xx*m.yy - m.ww*m.xy*m.yx - m.wx*m.xw*m.yy + m.wx*m.xy*m.yw + m.wy*m.xw*m.yx - m.wy*m.xx*m.yw) / d;
    float zw = (m.xw*m.yy*m.zx - m.xw*m.yx*m.zy + m.xx*m.yw*m.zy - m.xx*m.yy*m.zw - m.xy*m.yw*m.zx + m.xy*m.yx*m.zw) / d;

    float wx = (m.wx*m.yz*m.zy - m.wx*m.yy*m.zz + m.wy*m.yx*m.zz - m.wy*m.yz*m.zx - m.wz*m.yx*m.zy + m.wz*m.yy*m.zx) / d;
    float wy = (m.wx*m.xy*m.zz - m.wx*m.xz*m.zy - m.wy*m.xx*m.zz + m.wy*m.xz*m.zx + m.wz*m.xx*m.zy - m.wz*m.xy*m.zx) / d;
    float wz = (m.wx*m.xz*m.yy - m.wx*m.xy*m.yz + m.wy*m.xx*m.yz - m.wy*m.xz*m.yx - m.wz*m.xx*m.yy + m.wz*m.xy*m.yx) / d;
    float ww = (m.xx*m.yy*m.zz - m.xx*m.yz*m.zy - m.xy*m.yx*m.zz + m.xy*m.yz*m.zx + m.xz*m.yx*m.zy - m.xz*m.yy*m.zx) / d;

    return {xx, xy, xz, xw,
            yx, yy, yz, yw,
            zx, zy, zz, zw,
            wx, wy, wz, ww};
}

}
