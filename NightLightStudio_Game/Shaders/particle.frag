#version 330 core

in vec2 texCoords;
in vec4 particleColour;

out vec4 fragColor;

// Fragment material
uniform sampler2D diffuse_texture; // used for taking in diffuse texture

void main()
{
	if(particleColour.a < 0.01f)
	{
		discard;
	}

	vec4 image = texture(diffuse_texture, texCoords);
	fragColor = image * particleColour;
	//fragColor = particleColour;
}
