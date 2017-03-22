#version 120

attribute vec2 vertexPositionScreenSpace;
attribute vec2 vertexUV;

varying vec2 UV;

uniform vec2 screen;

void main(){
	vec2 vertexPosition = vertexPositionScreenSpace - screen;
	vertexPosition /= screen;
	gl_Position =  vec4(vertexPosition, 0, 1);	
	UV = vertexUV;
}

