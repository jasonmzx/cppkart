#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include<memory>
#include<string>

#include "shaderClass.h"
#include "core/ID.hpp"

class Texture : public ID
{
public:
	GLuint ID;
	GLenum type;
	Texture(std::string image, GLenum texType, GLenum slot, GLenum pixelType, bool rgbAlpha);

	// Assigns a texture unit to a texture
	void texUnit(std::shared_ptr<Shader>& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif