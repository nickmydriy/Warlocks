#version 330 core

layout(location = 0) in vec3 vertexPositionModelSpace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormalModelSpace;

out vec2 UV;
out vec3 positionWorldSpace;
out vec3 normalCameraSpace;
out vec3 eyeDirectionCameraSpace;
out vec3 lightDirectionCameraSpace;

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

