#pragma once

#include <glapi/GLApi.h>

class Texture {
private:
	unsigned int idTextureBuffer;
	unsigned char* buffer;
	int width;
	int height;
	int bpp;
public:
	const std::string path;
	Texture(const std::string& path);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};

