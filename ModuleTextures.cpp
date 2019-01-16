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

	defaultTexture = loadImg("./checkers.jpg","checkers.png");

	LoadTextures(); //since not using PhysFS this will need to be hardcoded

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
	for (std::list<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		delete (*it);
	}
	textures.clear();
	LOG("Freeing textures and Image library");
	return true;
}

// Load new texture from file path
Texture* ModuleTextures::loadImg(const char* path, const char* name)
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

		if (mipmaping)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipMapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipMapMode);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGenerateTextureMipmap(textureId);
		}

		ilDeleteImages(1, &imageId);
		glBindTexture(GL_TEXTURE_2D, 0);

		textures.push_back(new Texture(textureId, width, height,name));

		return new Texture(textureId, width, height,name);
	}
	LOG("Error: Texture loading %s, %s", iluErrorString(ilGetError()), name);

	return nullptr;
}

void ModuleTextures::LoadTextures()
{

	//had no time to implement PhysFS

	loadImg("./assets/Textures/BakerHouse/Baker_house.png","Baker_house.png");

	loadImg("./assets/Textures/Clock/ClockDiffuse.png", "ClockDiffuse.png");
	loadImg("./assets/Textures/Clock/ClockNormals.png", "ClockNormals.png");
	loadImg("./assets/Textures/Clock/ClockOcclusion.png", "ClockOcclusion.png");
	loadImg("./assets/Textures/Clock/ClockSpecular.tif", "ClockSpecular.tif");

	loadImg("./assets/Textures/DollHouse/DollhouseDiffuse.png", "DollhouseDiffuse.png");
	loadImg("./assets/Textures/DollHouse/DollhouseOcclusion.png", "DollhouseOcclusion.png");
	loadImg("./assets/Textures/DollHouse/DollhouseSpecular.tif", "DollhouseSpecular.tif");

	loadImg("./assets/Textures/Drawers/DrawersDiffuse.png", "DrawersDiffuse.png");
	loadImg("./assets/Textures/Drawers/DrawersOcclusion.png", "DrawersOcclusion.png");
	loadImg("./assets/Textures/Drawers/DrawersNormals.png", "DrawersNormals.png");
	loadImg("./assets/Textures/Drawers/DrawersSpecular.tif", "DrawersSpecular.tif");

	loadImg("./assets/Textures/Firetruck/FiretruckDiffuse.png", "FiretruckDiffuse.png");
	loadImg("./assets/Textures/Firetruck/FiretruckOcclusion.png", "FiretruckOcclusion.png");
	loadImg("./assets/Textures/Firetruck/FiretruckNormals.png", "FiretruckNormals.png");
	loadImg("./assets/Textures/Firetruck/FiretruckSpecular.tif", "FiretruckSpecular.tif");

	loadImg("./assets/Textures/Floor/PlanksDiffuse.png", "PlanksDiffuse.png");
	loadImg("./assets/Textures/Floor/PlanksOcclusion.png", "PlanksOcclusion.png");
	loadImg("./assets/Textures/Floor/PlanksNormals.png", "PlanksNormals.png");
	loadImg("./assets/Textures/Floor/PlanksSpecular.tif", "PlanksSpecular.tif");

	loadImg("./assets/Textures/Hearse/HearseDiffuse.png", "HearseDiffuse.png");
	loadImg("./assets/Textures/Hearse/HearseOcclusion.png", "HearseOcclusion.png");
	loadImg("./assets/Textures/Hearse/HearseNormals.png", "HearseNormals.png");
	loadImg("./assets/Textures/Hearse/HearseSpecular.tif", "HearseSpecular.tif");

	loadImg("./assets/Textures/Player/PlayerDiffuse.png", "PlayerDiffuse.png");
	loadImg("./assets/Textures/Player/PlayerOcclusion.png", "PlayerOcclusion.png");
	loadImg("./assets/Textures/Player/PlayerNormals.png","PlayerNormals.png");
	loadImg("./assets/Textures/Player/PlayerSpecular.tif","PlayerSpecular.tif");

	loadImg("./assets/Textures/Player/GunDiffuse.png", "GunDiffuse.png");
	loadImg("./assets/Textures/Player/GunOcclusion.png", "GunOcclusion.png");
	loadImg("./assets/Textures/Player/GunNormals.png", "GunNormals.png");
	loadImg("./assets/Textures/Player/GunSpecular.tif", "GunSpecular.tif");

	loadImg("./assets/Textures/Robot/RobotDiffuse.png", "RobotDiffuse.png");
	loadImg("./assets/Textures/Robot/RobotOcclusion.png", "RobotOcclusion.png");
	loadImg("./assets/Textures/Robot/RobotSpecular.tif", "RobotSpecular.tif");

	loadImg("./assets/Textures/SpinningTop/SpinningTopDiffuse.png","SpinningTopDiffuse.png");
	loadImg("./assets/Textures/SpinningTop/SpinningTopOcclusion.png","SpinningTopOcclusion.png");
	loadImg("./assets/Textures/SpinningTop/SpinningTopNormals.png", "SpinningTopNormals.png");

	loadImg("./assets/Textures/TracerPistol/tracerPistol.dds", "tracerPistol.dds");

	loadImg("./assets/Textures/Wall/WallDiffuse.png", "WallDiffuse.png");
	loadImg("./assets/Textures/Wall/WallOcclusion.png", "WallOcclusion.png");
	loadImg("./assets/Textures/Wall/WallNormals.png", "WallNormals.png");
	loadImg("./assets/Textures/Wall/WallSpecular.tif", "WallSpecular.tif");

	loadImg("./assets/Textures/ZomBunny/ZomBunnyDiffuse.png", "ZomBunnyDiffuse.png");
	loadImg("./assets/Textures/ZomBunny/ZomBunnyOcclusion.png", "ZomBunnyOcclusion.png");
	loadImg("./assets/Textures/ZomBunny/ZomBunnyNormals.png", "ZomBunnyNormals.png");
	loadImg("./assets/Textures/ZomBunny/ZomBunnySpecular.tif", "ZomBunnySpecular.tif");
	loadImg("./assets/Textures/ZomBunny/ZomBunnyEmissive.png", "ZomBunnyEmissive.png");
}