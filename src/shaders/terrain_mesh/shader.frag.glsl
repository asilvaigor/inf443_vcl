#version 330 core

in struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
    vec4 light_ref_pos[3];
} fragment;

uniform sampler2D texture_sampler;

uniform sampler2D shadow_map_0;
uniform sampler2D shadow_map_1;
uniform sampler2D shadow_map_2;

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

float shadowCalc(float un) {
    // Calculating shadow
    float a = 0.02;
    float light_dist[3];
    float epsilon[3];
    vec3 pos[3];
    vec2 texel_size[3];
    float current_depth[3];

    for (int i = 0; i < 6; i++) {
        light_dist[i] = length(fragment.light_ref_pos[i]);
        epsilon[i] = max((a / light_dist[i]) * (1.0 - un), a / (10 * light_dist[i]));
        pos[i] = fragment.light_ref_pos[i].xyz / fragment.light_ref_pos[i].w;
        pos[i] = pos[i] * 0.5 + 0.5;
        current_depth[i] = pos[i].z;
    }
    texel_size[0] = 1.0 / textureSize(shadow_map_0, 0);
    texel_size[1] = 1.0 / textureSize(shadow_map_1, 0);
    texel_size[2] = 1.0 / textureSize(shadow_map_2, 0);

    float shadow = 0.0;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcf_depth_0 = texture(shadow_map_0, pos[0].xy + vec2(x, y) * texel_size[0]).r;
            float pcf_depth_1 = texture(shadow_map_1, pos[1].xy + vec2(x, y) * texel_size[1]).r;
            float pcf_depth_2 = texture(shadow_map_2, pos[2].xy + vec2(x, y) * texel_size[2]).r;

            shadow += (current_depth[0] <= 1.0 && current_depth[0] - epsilon[0] > pcf_depth_0 ||
                       current_depth[1] <= 1.0 && current_depth[1] - epsilon[1] > pcf_depth_1 ||
                       current_depth[2] <= 1.0 && current_depth[2] - epsilon[2] > pcf_depth_2) ? 1.0 : 0.0;
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

    float shadow = shadowCalc(un);

    // Setting color
    vec4 color_texture = texture(texture_sampler, fragment.texture_uv);
    vec3 color = color.rgb * fragment.color.rgb * color_texture.rgb;
    vec3 c = (ambiant + (1 - shadow) * light_color * (diffuse_value + specular_value)) * color;
    //    vec3 c = (ambiant + diffuse_value) * color.rgb * fragment.color.rgb * color_texture.rgb + specular_value * white;

    FragColor = vec4(c, color_texture.a * fragment.color.a * color_alpha);
}
