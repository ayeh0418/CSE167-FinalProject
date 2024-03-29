
#version 330 core

// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.

//out vec3 normalOutput;
//out vec3 posOutput;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * model * vec4(position, 1.0);
//	posOutput = vec3(model * vec4(position, 1.0f));
//	normalOutput = mat3(transpose(inverse(model))) * normal;
	vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}