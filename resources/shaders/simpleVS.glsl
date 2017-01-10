#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 mvpMatrix;
uniform mat4 worldMatrix;
uniform vec3 directionalLight;
uniform vec4 polyColor;

out vec2 uvCoords;
out vec4 diffColor;
out float diffIntensity;

void main(void)
{
	gl_Position = mvpMatrix * position;
	
	uvCoords = uv;
	uvCoords.y = 1 - uvCoords.y;
	
	mat3 normalMatrix = transpose(inverse(mat3(worldMatrix)));
	vec3 transformedNormal = normalize(normalMatrix * normal);

	// For some reason, I need to normalize for myself, 
	// when using the normalize() function from GLSL, it does not work... 
	// Update: the problem was that I used a homogenous vec4 here. 
	// With a vec3, it works quite well :) 
	//vec4 dirLightNorm = vec4((directionalLight.xyz / length(directionalLight.xyz)), 1);
	vec3 dirLightNorm = normalize(directionalLight);
	
	/*diffIntensity = transformedNormal.x * dirLightNorm.x + 
					transformedNormal.y * dirLightNorm.y + 
					transformedNormal.z * dirLightNorm.z;
					*/
					
    diffIntensity = dot(transformedNormal, dirLightNorm);					
					
	diffColor = vec4(0, diffIntensity, 0, 1);
	
}