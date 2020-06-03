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

uniform int shadow_map_id;
uniform sampler2D shadow_map_0;
uniform sampler2D shadow_map_1;
uniform sampler2D shadow_map_2;
//uniform sampler2D shadow_map_3;
//uniform sampler2D shadow_map_4;
//uniform sampler2D shadow_map_5;

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color     = vec3(1.0, 1.0, 1.0);
uniform float color_alpha = 1.0;
uniform float ambiant  = 0.2;
uniform float diffuse  = 0.8;
uniform float specular = 0.5;
uniform int specular_exponent = 128;

uniform vec3 light_pos;
uniform vec3 light_color;

float shadowCalc(vec4 light_ref_pos, float epsilon) {
    vec3 pos = light_ref_pos.xyz / light_ref_pos.w;
    pos = pos * 0.5 + 0.5;
    if (pos.z > 1.0)
        return 0.0;

    float current_depth = pos.z;
    vec2 texel_size;
    switch(shadow_map_id) {
        case 0:
            texel_size = 1.0 / textureSize(shadow_map_0, 0);
            break;
        case 1:
            texel_size = 1.0 / textureSize(shadow_map_1, 0);
            break;
        case 2:
            texel_size = 1.0 / textureSize(shadow_map_2, 0);
            break;
        default:
            texel_size = vec2(0, 0);
            break;
    }
    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcf_depth;
            switch (shadow_map_id) {
                case 0:
                    pcf_depth = texture(shadow_map_0, pos.xy + vec2(x, y) * texel_size).r;
                    break;
                case 1:
                    pcf_depth = texture(shadow_map_1, pos.xy + vec2(x, y) * texel_size).r;
                    break;
                case 2:
                    pcf_depth = texture(shadow_map_2, pos.xy + vec2(x, y) * texel_size).r;
                    break;
                default:
                    pcf_depth = 0;
                    break;
            }

            shadow += (current_depth - epsilon > pcf_depth) ? 1.0 : 0.0;
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

    float light_dist = length(fragment.light_ref_pos);
    float a = 0.02;
    float epsilon = max((a / light_dist) * (1.0 - un), a / (10.0 * light_dist));
    float shadow = shadowCalc(fragment.light_ref_pos, epsilon);

    vec4 color_texture = texture(texture_sampler, fragment.texture_uv);
    vec3 color = color.rgb * fragment.color.rgb * color_texture.rgb;
    vec3 c = (ambiant + (1 - shadow) * light_color * (diffuse_value + specular_value)) * color;
//    vec3 c = (ambiant + diffuse_value) * color.rgb * fragment.color.rgb * color_texture.rgb + specular_value * white;

    FragColor = vec4(c, color_texture.a * fragment.color.a * color_alpha);
}
