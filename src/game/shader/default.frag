#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

in vec3 normal;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
// Uniform to switch between texture and solid color
uniform bool useTexture;

uniform vec3 lightDir;
uniform vec3 lightColor;

void main()
{
    if (useTexture) {
        // Sample the texture if useTexture is true
        FragColor = texture(tex0, texCoord) * vec4(vec3(1.0,1.0,1.0) * max(dot(normalize(lightDir), normal), 0.0), 1.0);
    } else {
        // Output the solid color if useTexture is false
		FragColor = vec4(color, 1.0); //texture(tex0, texCoord);
    }
}