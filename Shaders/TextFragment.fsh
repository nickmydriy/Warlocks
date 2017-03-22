#version 120

varying vec2 UV;

uniform sampler2D myTextSampler;
uniform vec3 textColor;
uniform vec4 backGroundColor;

void main(){
	gl_FragColor = texture2D(myTextSampler, UV);
	gl_FragColor.rgb = gl_FragColor.rgb * textColor;
	gl_FragColor.rgb = gl_FragColor.rgb * gl_FragColor.a - ((gl_FragColor.a - 1) * backGroundColor.rgb);
	gl_FragColor.a = gl_FragColor.a - ((gl_FragColor.a - 1) * backGroundColor.a);
}
