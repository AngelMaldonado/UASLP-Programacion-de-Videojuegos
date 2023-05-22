#include <glapi/GLApi.h>

Texture::Texture(const std::string& path)
	: idTextureBuffer(0), path(path), buffer(nullptr), width(0), height(0), bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(path.c_str(), &width, &height, &bpp, 0);

	if (buffer)
	{
		GLenum format;
		if (bpp == 1)
			format = GL_RED;
		else if (bpp == 3)
			format = GL_RGB;
		else if (bpp == 4)
			format = GL_RGBA;

		GLCall(glGenTextures(1, &idTextureBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, idTextureBuffer));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		stbi_image_free(buffer);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(buffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteBuffers(1, &idTextureBuffer));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, idTextureBuffer));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

