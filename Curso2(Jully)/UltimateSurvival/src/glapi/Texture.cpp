#include <glapi/GLApi.h>

Texture::Texture(const std::string& path, aiTextureType type)
	: bufferId(0), path(path), type(type), buffer(nullptr), width(0), height(0), bpp(0)
{
	Load();
}

Texture::~Texture()
{
	GLCall(glDeleteBuffers(1, &bufferId));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, bufferId));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Load()
{
	stbi_set_flip_vertically_on_load(true);
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

		GLCall(glGenTextures(1, &bufferId));
		GLCall(glBindTexture(GL_TEXTURE_2D, bufferId));
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

