#version 330 core

precision highp float;

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
// Uniform to switch between texture and solid color
uniform bool useTexture;

void main()
{
    if (useTexture) {
        // Sample the texture if useTexture is true
        
        vec4 textureColor = texture(tex0, texCoord);

        // Discard should never be used in the fragment shader

        if (textureColor.a < 0.2) {
            discard;
        }
        
        FragColor = textureColor;

    } else {
        // Output the solid color if useTexture is false
		FragColor = vec4(color, 1.0); //texture(tex0, texCoord);
    }
}