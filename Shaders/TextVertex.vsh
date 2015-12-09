#version 330 core

layout(location = 0) in vec2 vertexPositionScreenSpace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

uniform vec2 screen;

void main(){
	vec2 vertexPosition = vertexPositionScreenSpace - screen;
	vertexPosition /= screen;
	gl_Position =  vec4(vertexPosition, 0, 1);	
	UV = vertexUV;
}

