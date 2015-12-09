#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextSampler;
uniform vec3 textColor;
uniform vec4 backGroundColor;

void main(){
	color = texture2D(myTextSampler, UV);
	color.rgb = color.rgb * textColor;
	color.rgb = color.rgb * color.a - ((color.a - 1) * backGroundColor.rgb);
	color.a = color.a - ((color.a - 1) * backGroundColor.a);
}