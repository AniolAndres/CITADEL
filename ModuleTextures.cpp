#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleEditor.h"
#include "ModuleProgram.h"
#include "GL/glew.h"
#include <IL/il.h>
#include <IL/ilut.h>

using namespace std;

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{

}

// Called before render is available
bool ModuleTextures::Init()
{
	App->editor->consoleApp.AddLog("Init Image library \n");
	bool ret = true;

	ilInit();
	iluInit();
	ilutInit();

	defaultTexture = loadImg("./checkers.jpg");
	noCamTexture = loadImg("./nocamselected.jpg");


	return ret;
}

//Updating
update_status ModuleTextures::Update()
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");
	return true;
}

// Load new texture from file path
Texture* ModuleTextures::loadImg(const char* path)
{
	assert(path != nullptr);

	ILuint imageId = 0u;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(path))
	{

		unsigned textureId = 0u;
		ILinfo imageInfo;
		int pixelDepth = 0;
		int format = 0;
		int height = 0;
		int width = 0;

		// Generate a new texture
		glGenTextures(1, &textureId);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureId);

		iluGetImageInfo(&imageInfo);

		if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}
		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);
		format = ilGetInteger(IL_IMAGE_FORMAT);
		pixelDepth = ilGetInteger(IL_IMAGE_DEPTH);

		// Using RGBA if we got an alpha channel
		bool success = false;
		int channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (channels == 3) {
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		}
		else if (channels == 4) {
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		}

		// Quit if we failed the conversion
		if (!success) {
			LOG("Error: Could not convert the image to texture correctly. %s", iluErrorString(ilGetError()));
			return nullptr;
		}

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ilGetData());

		// Filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);

		if (wrapMode != NULL)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		}

		// TODO: Set linear or nearest mipmap
		if (mipmaping)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipMapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipMapMode);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGenerateTextureMipmap(textureId);
		}

		ilDeleteImages(1, &imageId);
		glBindTexture(GL_TEXTURE_2D, 0);

		return new Texture(textureId, width, height);
	}
	LOG("Error: Texture loading %s", iluErrorString(ilGetError()));

	return nullptr;
}