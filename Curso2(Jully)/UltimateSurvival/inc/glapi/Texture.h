#pragma once

#include <glapi/GLApi.h>

class Texture {
public:
	std::string path;
	Texture(const std::string& path, aiTextureType type);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
private:
	unsigned int bufferId;
	unsigned char* buffer;
	aiTextureType type;
	int width;
	int height;
	int bpp;
	void Load();
};

