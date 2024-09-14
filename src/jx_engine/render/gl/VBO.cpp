#include"VBO.h"

//* ==> Geometry Buffer

// VBO stores vertex data (e.g., positions, normals, colors, texture coordinates) in GPU memory. 
//This data is used by the vertex shader to render objects. (hence name: geom buffer)
//VBOs can significantly improve performance by reducing the need to send vertex data to the GPU multiple times.

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(const GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::VBO(std::vector<GLfloat> verticesVector)
{

	GLfloat* vertices = verticesVector.data();
	GLsizeiptr size = sizeof(GLfloat) * verticesVector.size();

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}