#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
// Normals
layout (location = 3) in vec3 aNormal;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

out vec3 normal;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;

uniform mat4 modelMatrix;


void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aPos;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
	normal = aNormal;
}

