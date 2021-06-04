

// SKIN_COLOR - Fragment Shader

#version 400 core

// Not allowed in 400, only 420
// layout (binding = 0) uniform sampler2D tex_object;



// Varying
	in VS_OUT
	{
		vec4 vFragColor;
	} fs_in;

	out vec4 color;

void main(void)
{
    vec4 baseColor = vec4(0.0f,1.0f,0.0f,1.0f);
	color = fs_in.vFragColor * baseColor ;
}

// --- End of File ---
