#version 450

#extension GL_ARB_shading_language_include : require

layout(set = 0, binding = 0) uniform Variables
{
	vec3 transformation;
} variables;

layout(location = 0) out vec4 pixelColor;

const vec2 resolution = vec2(1600, 900);
const float aspect = resolution.x / resolution.y;

int maxIterations = 90;
//float zoom = 1.0;
//vec2 center = vec2(-0.5, 0.0);

float Manderbrot(vec2 uv)
{
	float x = (uv.x - 0.5) * 2.0 * aspect / variables.transformation.z + variables.transformation.x;
	float y = (uv.y - 0.5) * 2.0 / variables.transformation.z + variables.transformation.y;
	vec2 c = vec2(x, y);
	vec2 z = vec2(0.0);

	int iterations;
	for (iterations = 0; iterations < maxIterations; iterations++)
	{
		float tmpX = (z.x * z.x - z.y * z.y) + c.x;
		z.y = 2.0 * z.x * z.y + c.y;
		z.x = tmpX;

		if (dot(z, z) > 4.0)
		{
			break;
		}
	}

	return (float(iterations) / float(maxIterations));
}

void main()
{
	//pixelColor = vec4(gl_FragCoord.x / 800.0, gl_FragCoord.y / 800.0, 1.0, 1.0);

	vec2 uv = gl_FragCoord.xy / resolution.xy;
	float result = Manderbrot(uv);
	pixelColor = vec4(vec3(result), 1.0);
}