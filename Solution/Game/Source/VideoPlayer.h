#pragma once
#include "Module.h"

#include <dshow.h>
#include <Vfw.h>
#pragma comment( lib, "vfw32.lib" )   

class VideoPlayer : public Module
{
public:
	VideoPlayer();

	virtual ~VideoPlayer();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	//It has been created so that it works by giving only the location and the name of the 2 files with the same name without extension,
	//both the audio and the video, so we can load them more comfortably. 
	void StartVideo(char* filePath);

	bool GetFinishVideo() { return finish; };


private:

	AVISTREAMINFO       psi;                        // Pointer To A Structure Containing Stream Info
	PAVISTREAM			pavi;                       // Handle To An Open Stream
	PGETFRAME			pgf;                        // Pointer To A GetFrame Object

	SDL_Surface* surface = nullptr;
	SDL_Texture* textureFrame = nullptr;
	char* pdata;

	int frameIndex = 0;
	uint time = 0;
	uint nFrames = 0;

	int prevFPS = 0;
	int FPS = 0;
	int width = 0;
	int height = 0;


	bool finish = false;
	
	char* video = "";
	char* audio = "";

	SString path = "";

};