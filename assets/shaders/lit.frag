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
    vec3 direction; // Directional light is only defined by a direction. In spot light, the direction is the direction of the spot light's cone axis.
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
    return max(0.0f, dot(normal, -light_direction));
}

// This will be used to compute the phong specular.
float calculate_phong(vec3 normal, vec3 light_direction, vec3 view, float shininess){
    vec3 reflected = reflect(light_direction, normal);
    return pow(max(0.0f, dot(view, reflected)), shininess);
}

void main(){
    // Normalize the normal and view since it can change during interpolation.
    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);

    vec3 accumulated_light = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < lightCount; i++) {

        vec3 light_direction;
        vec3 diffuse;
        vec3 ambient;
        vec3 specular;
        float attenuation = 1.0f;

        if(lights[i].type == DIRECTIONAL){
            light_direction = lights[i].direction;
        } else {
            // Get the light direction and distance relative to the pixel location in the world space.
            light_direction = fs_in.world - lights[i].position;
            float distance = length(light_direction);
            light_direction /= distance;

            // Calculate the attenuation factor based on the light distance from the pixel.
            attenuation *= 1.0f / (lights[i].attenuation.constant + lights[i].attenuation.linear * distance + lights[i].attenuation.quadratic * distance * distance);

            if(lights[i].type == SPOT){
                // Calculate the angle between the pixel and the cone axis.
                float angle = acos(dot(lights[i].direction, light_direction));

                // Calculate the attenuation based on the angle.
                float angle_attenuation = smoothstep(lights[i].spot_angle.outer, lights[i].spot_angle.inner, angle);

                // Combine the attenuation and the angle attenuation.
                attenuation *= angle_attenuation;
            }
        }

        vec3 reflected = reflect(light_direction, normal);
        float lambert = calculate_lambert(normal, light_direction);
        float phong = calculate_phong(normal, light_direction, view, material.shininess);

        diffuse = lights[i].diffuse * material.diffuse * lambert;
        ambient = lights[i].ambient * material.ambient;
        specular = lights[i].specular * material.specular * phong;

        accumulated_light += attenuation * (diffuse + specular) + ambient;

        // vec3 light_direction = vec3(0.0, -1.0, 0.0);
        // // Diffuse Component [Diffuse = Kd * Id * Max(0,l.n) ]
        // float lambert = max(0.0,dot(fs_in.normal, light_direction)); // Lambert's Cosine Law
        // vec3 light_diffuse = vec3(1.0, 1.0, 1.0); // White Light
        // vec3 material_diffuse = vec3(0.2, 0.2, 0.2); // Diffuse Color of the Material
        // vec3 diffuse = light_diffuse * material_diffuse * lambert;

        // // Ambient Component [Ambient = Ka * Ia]
        // vec3 light_ambient = vec3(0.1, 0.1, 0.1); // Ambient Light Intensity
        // vec3 material_ambient = vec3(0.2, 0.2, 0.2); // Ambient Color of the Material
        // vec3 ambient = light_ambient * material_ambient;

        // // Specular Component [Specular = Ks * Is * Max(0, (r.v))^alpha]
        // vec3 r = reflect(-light_direction, fs_in.normal);
        // vec3 light_specular = vec3(1.0, 1.0, 1.0); // White Light
        // vec3 material_specular = vec3(0.2, 0.2, 0.2); // Specular Color of the Material
        // float alpha = 10.0; // Shininess of the Material
        // float phong = pow(max(0.0, dot(r,fs_in.view)), alpha);
        // vec3 specular = light_specular * material_specular * phong;

        // vec4 light_color = vec4((diffuse + specular + ambient), 1.0);
        frag_color = texture(tex, fs_in.tex_coord) * vec4(accumulated_light, 1.0f);
        // frag_color = fs_in.color * vec4(accumulated_light, 1.0f);
        // frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f) * vec4(accumulated_light, 1.0f);
    }
}