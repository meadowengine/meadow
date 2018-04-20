#ifndef MEADOW_TEXTURE_H_
#define MEADOW_TEXTURE_H_

namespace Meadow {

	class Texture {
		unsigned int glId;
	public:
		Texture();
		Texture(int width, int height, unsigned char rgba[]);

		void bind();

		static void bindNone();
	};

}

#endif