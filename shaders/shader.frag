/*
// #version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normalOutput;
in vec3 posOutput;

uniform Material m;
uniform vec3 lightAttr1;
uniform vec3 viewPos;
uniform int mode;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
	if (mode == 1) {
		vec3 lightPos = vec3(-1.0f, 1.0f, 2.0f); // light source

		 // ambient
		vec3 ambient = lightAttr1 * m.ambient;
  	
		// diffuse 
		vec3 norm = normalize(normalOutput);
		vec3 lightDir = normalize(lightPos - posOutput);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = lightAttr1 * (diff * m.diffuse);
    
		// specular
		vec3 viewDir = normalize(viewPos - posOutput);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), m.shininess);
		vec3 specular = lightAttr1 * (spec * m.specular);  
        
		vec3 result = ambient + diffuse + specular ;
		fragColor = vec4(result, 1.0);
	} else {
		fragColor = vec4((normalOutput + 1) / 2, 1);
	}
}
*/

#version 330 core
out vec4 fragColor;

in vec3 normalOutput;
in vec3 posOutput;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(posOutput - viewPos);
    vec3 R = reflect(I, normalize(normalOutput));
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}
