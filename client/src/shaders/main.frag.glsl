#version 330 core
in vec2 texCoords;
in vec3 normal;
uniform sampler2D texture0;

void main()
{
	gl_FragColor = texture(texture0, texCoords);
	//gl_FragColor = vec4(normal,1.0);
}
