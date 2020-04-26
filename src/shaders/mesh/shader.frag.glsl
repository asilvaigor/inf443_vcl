#version 330 core

in struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
    vec4 light_ref_pos;
} fragment;

uniform sampler2D texture_sampler;
uniform sampler2D shadow_map;

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color     = vec3(1.0, 1.0, 1.0);
uniform float color_alpha = 1.0;
uniform float ambiant  = 0.2;
uniform float diffuse  = 0.8;
uniform float specular = 0.5;
uniform int specular_exponent = 128;

uniform mat4 light_matrix;
uniform vec3 light_pos;

float shadowCalc(vec4 light_ref_pos, float epsilon) {
    vec3 pos = light_ref_pos.xyz / light_ref_pos.w;
    pos = pos * 0.5 + 0.5;
    if (pos.z > 1.0)
        return 0.0;

    float closest_depth = texture(shadow_map, pos.xy).r;
    float current_depth = pos.z;

    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcf_depth = texture(shadow_map, pos.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - epsilon > pcf_depth ? 1.0 : 0.0;
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

    float epsilon = max(0.01 * (1.0 - un), 0.001);
    float shadow = shadowCalc(fragment.light_ref_pos, epsilon);

    vec3 white = vec3(1.0);
    vec4 color_texture = texture(texture_sampler, fragment.texture_uv);
    vec3 color = color.rgb * fragment.color.rgb * color_texture.rgb;
    vec3 c = (ambiant + (1 - shadow) * (diffuse_value + specular_value * white)) * color;
//    vec3 c = (ambiant + diffuse_value) * color.rgb * fragment.color.rgb * color_texture.rgb + specular_value * white;

    FragColor = vec4(c, color_texture.a * fragment.color.a * color_alpha);
}
