#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include <vector>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;

	// Constructors that generates a Vertex Buffer Object and links it to vertices
	VBO(const GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<GLfloat> verticesVector);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif