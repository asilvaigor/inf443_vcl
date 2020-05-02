#version 330 core

in struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
    vec4 light_ref_pos;
    vec4 light_ref_pos_2;
    vec4 light_ref_pos_3;
} fragment;

uniform sampler2D texture_sampler;

uniform int shadow_map_id;
uniform sampler2D shadow_map_still_1;
uniform sampler2D shadow_map_movable_1;
uniform sampler2D shadow_map_still_2;
uniform sampler2D shadow_map_movable_2;
uniform sampler2D shadow_map_still_3;
uniform sampler2D shadow_map_movable_3;

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color     = vec3(1.0, 1.0, 1.0);
uniform float color_alpha = 1.0;
uniform float ambiant  = 0.2;
uniform float diffuse  = 0.8;
uniform float specular = 0.5;
uniform int specular_exponent = 128;

uniform mat4 light_matrix;
uniform mat4 light_matrix_2;
uniform mat4 light_matrix_3;
uniform vec3 light_pos;

float shadowCalc(vec4 light_ref_pos_1, float epsilon_1, vec4 light_ref_pos_2,
                 float epsilon_2, vec4 light_ref_pos_3, float epsilon_3) {
    vec3 pos_1 = light_ref_pos_1.xyz / light_ref_pos_1.w;
    pos_1 = pos_1 * 0.5 + 0.5;
    vec3 pos_2 = light_ref_pos_2.xyz / light_ref_pos_2.w;
    pos_2 = pos_2 * 0.5 + 0.5;
    vec3 pos_3 = light_ref_pos_3.xyz / light_ref_pos_3.w;
    pos_3 = pos_3 * 0.5 + 0.5;

    float current_depth_1 = pos_1.z;
    vec2 texel_size_still_1 = 1.0 / textureSize(shadow_map_still_1, 0);
    vec2 texel_size_movable_1 = 1.0 / textureSize(shadow_map_movable_1, 0);
    float current_depth_2 = pos_2.z;
    vec2 texel_size_still_2 = 1.0 / textureSize(shadow_map_still_2, 0);
    vec2 texel_size_movable_2 = 1.0 / textureSize(shadow_map_movable_2, 0);
    float current_depth_3 = pos_3.z;
    vec2 texel_size_still_3 = 1.0 / textureSize(shadow_map_still_3, 0);
    vec2 texel_size_movable_3 = 1.0 / textureSize(shadow_map_movable_3, 0);

    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcf_depth_still_1 = texture(shadow_map_still_1, pos_1.xy + vec2(x, y) * texel_size_still_1).r;
            float pcf_depth_movable_1 = texture(shadow_map_movable_1, pos_1.xy + vec2(x, y) * texel_size_movable_1).r;
            float pcf_depth_still_2 = texture(shadow_map_still_2, pos_2.xy + vec2(x, y) * texel_size_still_2).r;
            float pcf_depth_movable_2 = texture(shadow_map_movable_2, pos_2.xy + vec2(x, y) * texel_size_movable_2).r;
            float pcf_depth_still_3 = texture(shadow_map_still_3, pos_3.xy + vec2(x, y) * texel_size_still_3).r;
            float pcf_depth_movable_3 = texture(shadow_map_movable_3, pos_3.xy + vec2(x, y) * texel_size_movable_3).r;

            shadow += (current_depth_1 <= 1.0 &&
                       (current_depth_1 - epsilon_1 > pcf_depth_still_1 ||
                        current_depth_1 - epsilon_1 > pcf_depth_movable_1) ||
                       current_depth_2 <= 1.0 &&
                       (current_depth_2 - epsilon_2 > pcf_depth_still_2 ||
                        current_depth_2 - epsilon_2 > pcf_depth_movable_2) ||
                       current_depth_3 <= 1.0 &&
                       (current_depth_3 - epsilon_3 > pcf_depth_still_3 ||
                        current_depth_3 - epsilon_3 > pcf_depth_movable_3)) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
    vec3 n = normalize(fragment.normal.xyz);
    vec3 u = normalize(light_pos-fragment.position.xyz);
    vec3 r = reflect(u, n);
    vec3 t = normalize(fragment.position.xyz - camera_position);

    float un = dot(u, n);

    float diffuse_value  = diffuse * clamp(un, 0.0, 1.0);
    float specular_value = specular * pow(clamp(dot(r, t), 0.0, 1.0), specular_exponent);

    // Calculating shadow
    float a = 0.01;
    float light_dist = length(fragment.light_ref_pos);
    float epsilon = max((a / light_dist) * (1.0 - un), a / (10 * light_dist));
    float light_dist_2 = length(fragment.light_ref_pos_2);
    float epsilon_2 = max((a / light_dist_2) * (1.0 - un), a / (10 * light_dist_2));
    float light_dist_3 = length(fragment.light_ref_pos_3);
    float epsilon_3 = max((a / light_dist_3) * (1.0 - un), a / (10 * light_dist_3));
    float shadow = shadowCalc(fragment.light_ref_pos, epsilon, fragment.light_ref_pos_2,
                              epsilon_2, fragment.light_ref_pos_3, epsilon_3);

    // Setting color
    vec3 white = vec3(1.0);
    vec4 color_texture = texture(texture_sampler, fragment.texture_uv);
    vec3 color = color.rgb * fragment.color.rgb * color_texture.rgb;
    vec3 c = (ambiant + (1 - shadow) * (diffuse_value + specular_value * white)) * color;
//    vec3 c = (ambiant + diffuse_value) * color.rgb * fragment.color.rgb * color_texture.rgb + specular_value * white;

    FragColor = vec4(c, color_texture.a * fragment.color.a * color_alpha);
}
