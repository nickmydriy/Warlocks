#version 120

varying vec2 UV;
varying vec3 positionWorldSpace;
varying vec3 normalCameraSpace;
varying vec3 eyeDirectionCameraSpace;
varying vec3 lightDirectionCameraSpace;

uniform sampler2D myTextureSampler;
uniform vec3 lightPositionWorldSpace;
uniform float bright;
uniform float alpha;
uniform vec3 myFilter;
uniform vec3 myColor;


void main(){

	vec3 lightColor = vec3(1, 1, 1);
	float lightPower = 9.0f;
	
	vec3 materialDiffuseColor = texture2D(myTextureSampler, UV).rgb;
	vec3 materialAmbientColor = vec3(0.15, 0.15, 0.15) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(0.25, 0.25, 0.25);
	float distance = length(lightPositionWorldSpace - positionWorldSpace);

	vec3 normal = normalize(normalCameraSpace);
	vec3 light = normalize(lightDirectionCameraSpace);
	float cosTheta = clamp(dot(normal, light), 0, 1);
	
	vec3 E = normalize(eyeDirectionCameraSpace);
	vec3 R = reflect(-light, normal);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	gl_FragColor.rgb = materialAmbientColor * bright
	 + materialDiffuseColor * lightColor * lightPower * cosTheta / distance * bright
	 + materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / distance;
	gl_FragColor.rgb = gl_FragColor.rgb * myFilter;
	gl_FragColor.rgb = gl_FragColor.rgb + myColor;
	gl_FragColor.a = alpha * texture2D(myTextureSampler, UV).a;
}
