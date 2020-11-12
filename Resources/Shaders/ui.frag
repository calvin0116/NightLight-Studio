#version 330 core

in vec2 texCoords;
out vec4 fragColor;

// Fragment material
uniform sampler2D diffuse_texture; // used for taking in diffuse texture
uniform vec4 colour;

void main()
{
	vec4 image = texture(diffuse_texture, texCoords);
	fragColor = image * colour *image.a;
}
