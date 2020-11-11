#version 330 core
out vec4 fragmentColor;
in vec2 fragmentTexture;

// texture sampler
// Read in from glBindTexture
uniform sampler2D texture1;
uniform vec4 Colour;

void main()
{
	vec4 fragColor = texture(texture1, fragmentTexture);
	fragmentColor = fragColor * vec4(Colour)*fragColor.a;
}
