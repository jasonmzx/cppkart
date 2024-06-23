#include"Texture.h"

Texture::Texture(std::string image, GLenum texType, GLenum slot, GLenum pixelType, bool rgbAlpha)
{
	static int debug = 0;
	type = texType;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	
	//Equivalent to a UV Flip
	stbi_set_flip_vertically_on_load(false);

	GLenum format = GL_RGB;

	if(rgbAlpha)
		format = GL_RGBA;

	printf("FORMAT IN HEX: %x\n", format);

	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image.c_str(), &widthImg, &heightImg, &numColCh, 0);

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	//* Configures the type of algorithm that is used to make the image smaller or bigger for mipmapping
	// glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	// glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	
	// GL Mipmapping
	glGenerateMipmap(texType);
	
	float lodBias = -1.5f; // Adjust this value as needed
    glTexParameterf(texType, GL_TEXTURE_LOD_BIAS, lodBias);



	float anisotropyLevel = 16.0f;
	    // Check for anisotropic filtering support and set it
    // Check for anisotropic filtering support and set it
    if (GLAD_GL_EXT_texture_filter_anisotropic) {
        if (anisotropyLevel > 1.0f) {
            GLfloat maxAniso = 0.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
            anisotropyLevel = std::min(anisotropyLevel, maxAniso);
            glTexParameterf(texType, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
        }
    } else {
        printf("Anisotropic filtering not supported.\n");
    }

	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(texType, 0);

    hashID = createHashedID(image);
}

void Texture::texUnit(std::shared_ptr<Shader>& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader->ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader->Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);

	printf("Successfully loaded TexUnit");
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}