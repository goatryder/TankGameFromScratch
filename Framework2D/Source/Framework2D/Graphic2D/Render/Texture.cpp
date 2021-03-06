#include <PCH_Framework.h>
#include "Texture.h"

#include <GL/glew.h>
#include <stb_image/stb_image.h>

namespace Framework2D {

	Texture::Texture(const std::string& Path)
		: RendererID(0), FilePath(Path), ImageCache(nullptr), Width(0), Height(0), BPP(0), ActiveSlot(0)
	{
		// load image data
		stbi_set_flip_vertically_on_load(1);
		ImageCache = stbi_load(FilePath.c_str(), &Width, &Height, &BPP, 4);

		// validate
		bSuccessfullyCreated = ImageCache != nullptr;
		if (!bSuccessfullyCreated)
			ENGINE_LOG(error, "Texture '{0}' fail to read image file", Path);

		// generate and bind texture
		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		// default texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// specify pixel array to texture and unbind texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageCache);
		glBindTexture(GL_TEXTURE_2D, 0);

		// free image data
		if (ImageCache)
			stbi_image_free(ImageCache);
	}

	Texture::Texture(Texture&& t) noexcept
		: RendererID(t.RendererID), FilePath(std::move(t.FilePath)), ImageCache(t.ImageCache),
		Width(t.Width), Height(t.Height), BPP(t.BPP), ActiveSlot(t.ActiveSlot), bSuccessfullyCreated(t.bSuccessfullyCreated)
	{
		t.ImageCache = nullptr;
		t.RendererID = 0;
	}

	Texture::~Texture()
	{
		if (RendererID != 0)
			glDeleteTextures(1, &RendererID);
	}

	void Texture::Bind(uint32_t slot) const
	{
		++slot;

		if (slot > GetMaxTextureBind() || !bSuccessfullyCreated)
		{
			ActiveSlot = 0;

			//ENGINE_LOG(info, "\tBind texture {} fail, use default slot: {}", GetPath(), GetActiveSlot());
			return;
		}

		ActiveSlot = slot;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		//ENGINE_LOG(info, "\tBind texture {0} on slot {1}", GetPath(), GetActiveSlot());
	}

	void Texture::Unbind() const
	{
		ActiveSlot = 0;
	}

	int Texture::GetMaxTextureBind()
	{
		static int MaxBindTextureUnitsNum = []() { int out; glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &out); return out; } ();
		return MaxBindTextureUnitsNum - 1; // Texture404 has slot 0
	}

}
