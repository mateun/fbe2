#version 430 core

in vec2 uvCoords;
in vec4 diffColor;
in float diffIntensity;
out vec4 color;

uniform sampler2D sampler;

void main(void)
{
	color = texture(sampler, uvCoords);
	color = color * diffIntensity;
	//color = diffColor;
}