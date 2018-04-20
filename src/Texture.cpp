#include "Texture.h"

#ifndef __EMSCRIPTEN__
	#include "GL/glew.h"
	#include "stb_image.h"
#endif

namespace Meadow {

	GLuint boundTexture = 0;

	Texture::Texture() {
		glGenTextures(1, &glId);
		bind();
	}

	Texture::Texture(int width, int height, unsigned char rgba[]) {
		glGenTextures(1, &glId);
		bind();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void Texture::bind() {
		if (boundTexture != glId) {
			glBindTexture(GL_TEXTURE_2D, glId);
			boundTexture = glId;
		}
	}

	void Texture::bindNone() {
		if (boundTexture != 0) {
			glBindTexture(GL_TEXTURE_2D, 0);
			boundTexture = 0;
		}
	}

}