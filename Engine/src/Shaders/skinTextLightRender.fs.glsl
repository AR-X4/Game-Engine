

// SKIN_TEXTURE - Fragment Shader

#version 400 core

// Not allowed in 400, only 420
// layout (binding = 0) uniform sampler2D tex_object;

// Uniforms
	uniform sampler2D tex_object;

// Varying
	in VS_OUT
	{
		vec4 vFragColor;
		vec2 textCoordinate;
	} fs_in;

	out vec4 color;

void main(void)
{
    color = fs_in.vFragColor * texture(tex_object, fs_in.textCoordinate );
}

// --- End of File ---
