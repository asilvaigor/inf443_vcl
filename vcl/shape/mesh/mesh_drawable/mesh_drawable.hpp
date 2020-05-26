#pragma once

#include "vcl/shape/mesh/mesh_structure/mesh.hpp"
#include "vcl/math/math.hpp"
#include "vcl/interaction/camera/camera.hpp"

#include "mesh_drawable_gpu_data/mesh_drawable_gpu_data.hpp"
#include "mesh_drawable_uniform/mesh_drawable_uniform.hpp"


namespace vcl
{




/** High level OpenGL drawable structure.
 * mesh_drawable contains gpu data (mesh_drawable_gpu_data: VAO, VBO, etc.), and a set of uniform parameter (mesh_drawable_uniform)*/
struct mesh_drawable
{
public:

    mesh_drawable();
    /** Initialize VAO and VBO from the mesh */
    mesh_drawable(const mesh& mesh_cpu, GLuint shader = 0, GLuint texture_id = 0);


    /** Clear buffers (VBO, VAO, etc) */
    void clear();

    /** Dynamically update the VBO with the new vector of position
     * Warning: new_position is expected to have the same size (or less) than the initialized one */
    void update_position(const vcl::buffer<vec3>& new_position);

    /** Dynamically update the VBO with the new vector of normal
     * Warning: new_normal is expected to have the same size (or less) than the initialized one */
    void update_normal(const vcl::buffer<vec3>& new_normal);

    /**
     * Overrides all mesh_drawable's shader with this given shader. If 0, the shaders will not be overriden.
     * @param override Shader id to override. If 0, the shades will not be overriden.
     */
    static void overrideShader(GLuint override);


    /** Data attributes: VAO and VBO as well as the number of triangle */
    mesh_drawable_gpu_data data;
    mesh_drawable_uniform uniform;
    GLuint shader;
    GLuint texture_id;
    static GLuint shaderOverride;
};

void draw(const mesh_drawable& drawable, const camera_scene& camera);
void draw(const mesh_drawable& drawable, const camera_scene& camera, GLuint shader);
void draw(const mesh_drawable& drawable, const camera_scene& camera, GLuint shader, GLuint texture_id);
void load_data(const mesh_drawable& drawable, const camera_scene& camera, GLuint shader, GLuint texture_id);


}
