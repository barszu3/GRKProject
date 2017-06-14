#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D objectTex;

in vec3 interpNormal;
in vec2 texPosition;

void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	vec4 textureColor = texture2D(objectTex, vec2(texPosition.x, 1 - texPosition.y));
	gl_FragColor = vec4(textureColor.rgb * diffuse, 1.0);
}
