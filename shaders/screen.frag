#version 450

#extension GL_ARB_shading_language_include : require

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(gl_FragCoord.x / 400.0, gl_FragCoord.y / 400.0, 1.0, 1.0);
	//outColor = vec4(1.0);
}