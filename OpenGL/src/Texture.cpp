#include "Texture.h"

#include "stb_image/stb_image.h"
#include "Renderer.h"

Texture::Texture(const std::string& filepath) : rendererId(0), filepath(filepath), localBuffer(nullptr), width(0), height(0), BPP(0) {
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(filepath.c_str(), &width, &height, &BPP, 4);

	GLCall(glGenTextures(1, &rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if(localBuffer) {
		stbi_image_free(localBuffer);
	}
}


Texture::~Texture() {
	GLCall(glDeleteTextures(1, &rendererId));
}

void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererId));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture::GetWidth() const {
	return width;
}

int Texture::GetHeight() const {
	return height;
}
