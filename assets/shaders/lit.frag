#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
} fs_in;

uniform sampler2D tex;

out vec4 frag_color;

void main(){
    vec3 light_direction = normalize(vec3(0.0, 1.0, 0.0)); 

    // Diffuse Component [Diffuse = Kd * Id * Max(0,l.n) ]
    float lambert = max(0.0,dot(fs_in.normal, light_direction)); // Lambert's Cosine Law
    vec3 light_diffuse = vec3(1.0, 1.0, 1.0); // White Light
    vec3 material_diffuse = vec3(0.5, 0.5, 0.5); // Diffuse Color of the Material
    vec3 diffuse = light_diffuse * material_diffuse * lambert;

    // Ambient Component [Ambient = Ka * Ia]
    vec3 light_ambient = vec3(0.1, 0.1, 0.1); // Ambient Light Intensity
    vec3 material_ambient = vec3(0.5, 0.5, 0.5); // Ambient Color of the Material
    vec3 ambient = light_ambient * material_ambient;

    // Specular Component [Specular = Ks * Is * Max(0, (r.v))^alpha]
    vec3 r = reflect(-light_direction, fs_in.normal);
    vec3 light_specular = vec3(1.0, 1.0, 1.0); // White Light
    vec3 material_specular = vec3(0.5, 0.5, 0.5); // Specular Color of the Material
    float alpha = 1.0; // Shininess of the Material
    float phong = pow(max(0.0, dot(r,fs_in.view)), alpha);
    vec3 specular = light_specular * material_specular * phong;

    frag_color = texture(tex, fs_in.tex_coord) * vec4(diffuse + ambient + specular, 1.0);
}