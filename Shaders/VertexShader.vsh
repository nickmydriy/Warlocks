#version 120

attribute vec3 vertexPositionModelSpace;
attribute vec2 vertexUV;
attribute vec3 vertexNormalModelSpace;

varying vec2 UV;
varying vec3 positionWorldSpace;
varying vec3 normalCameraSpace;
varying vec3 eyeDirectionCameraSpace;
varying vec3 lightDirectionCameraSpace;

uniform mat4 mvpMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPositionWorldSpace;


void main(){
	gl_Position =  mvpMatrix * vec4(vertexPositionModelSpace,1);
	positionWorldSpace = (modelMatrix * vec4(vertexPositionModelSpace,1)).xyz;
	vec3 vertexPositionCameraSpace = (viewMatrix * modelMatrix * vec4(vertexPositionModelSpace,1)).xyz;
	eyeDirectionCameraSpace = vec3(0, 0, 0) - vertexPositionCameraSpace;
	vec3 lightPositionCameraSpace = (viewMatrix * vec4(lightPositionWorldSpace,1)).xyz;
	lightDirectionCameraSpace = lightPositionCameraSpace + eyeDirectionCameraSpace;
	normalCameraSpace = (viewMatrix * modelMatrix * vec4(vertexNormalModelSpace,0)).xyz;
	UV = vertexUV;
}

