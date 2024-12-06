#version 330 core

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} fs_in;

uniform sampler2D tex;

out vec4 frag_color;

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct Spot_angle {
    float inner;
    float outer;
};

struct Light {
    int type;   // 0 = Directional, 1 = Point, 2 = Spot

    // These define the position and direction of the light
    vec3 direction; // Directional light is only defined by a direction. In spot loght, the direction is the direction of the spot light's cone axis.
    vec3 position;  // Point light is defined only by a position. In spot light, the position is the position of the light source.

    // These define the colors and intensities of the light
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;

    // The attenuation is used to control how the light dims out as we go further from it.
    Attenuation attenuation;  

    // The angles define the spot light cone shape.
    // Inside the inner cone, the light intensity is full. Outside the outer angle, the light intensity is 0.
    // In between we use a smooth step to compute the light intensity.
    Spot_angle spot_angle;
};

struct Material {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

// Receive the material and the light as uniforms.
uniform Light lights[10];    // Maximum of 10 lights
uniform Material material;
uniform int lightCount;

// This will be used to compute the diffuse factor.
float calculate_lambert(vec3 normal, vec3 light_direction){
    return max(0.0f, dot(normal, light_direction));
}

// This will be used to compute the phong specular.
float calculate_phong(vec3 normal, vec3 light_direction, vec3 view, float shininess){
    vec3 reflected = reflect(-light_direction, normal);
    return pow(max(0.0f, dot(view, reflected)), shininess);
}

void main(){
    vec3 light_direction = normalize(vec3(0.0, 1.0, 0.0)); 
    vec3 view = normalize(fs_in.view);
    vec3 normal = normalize(fs_in.normal);

    // Diffuse Component [Diffuse = Kd * Id * Max(0,l.n) ]
    float lambert = max(0.0,dot(normal, light_direction)); // Lambert's Cosine Law
    vec3 light_diffuse = vec3(1.0, 1.0, 1.0); // White Light
    vec3 material_diffuse = vec3(0.5, 0.5, 0.5); // Diffuse Color of the Material
    vec3 diffuse = light_diffuse * material_diffuse * lambert;

    // Ambient Component [Ambient = Ka * Ia]
    vec3 light_ambient = vec3(0.1, 0.1, 0.1); // Ambient Light Intensity
    vec3 material_ambient = vec3(0.5, 0.5, 0.5); // Ambient Color of the Material
    vec3 ambient = light_ambient * material_ambient;

    // Specular Component [Specular = Ks * Is * Max(0, (r.v))^alpha]
    vec3 r = reflect(-light_direction, normal);
    vec3 light_specular = vec3(1.0, 1.0, 1.0); // White Light
    vec3 material_specular = vec3(0.5, 0.5, 0.5); // Specular Color of the Material
    float alpha = 10.0; // Shininess of the Material
    float phong = pow(max(0.0, dot(r,view)), alpha);
    vec3 specular = light_specular * material_specular * phong;

    frag_color = texture(tex, fs_in.tex_coord) * vec4(diffuse + ambient + specular, 1.0);
}