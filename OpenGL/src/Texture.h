#pragma once

#include <string>

class Texture {
private:
	unsigned int rendererId;
	std::string filepath;
	unsigned char* localBuffer;
	int width, height, BPP;

public:
	Texture(const std::string& filepath);
	~Texture();

public:
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const;
	inline int GetHeight() const;
	
};
