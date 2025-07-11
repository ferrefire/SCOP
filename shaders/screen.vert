#version 450

#extension GL_ARB_shading_language_include : require

layout(location = 0) in vec3 localPosition;

void main()
{
	gl_Position = vec4(localPosition.xy * 2.0, 0.0, 1.0);
}