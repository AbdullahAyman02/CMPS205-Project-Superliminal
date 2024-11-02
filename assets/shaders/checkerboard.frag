#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // The position of the tile in the grid.
    ivec2 position = ivec2(gl_FragCoord.xy) / size;

    // The color of the tile is determined by the sum of the x and y position of the tile. Odd sum means color[1], even means color[0].
    vec3 color = colors[(position.x + position.y) % 2];

    frag_color = vec4(color, 1.0);
}