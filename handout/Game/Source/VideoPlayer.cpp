#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"	
#include "VideoPlayer.h"

#include <SDL_mixer\include\SDL_mixer.h>

#include "Defs.h"
#include "Log.h"
#include <string.h>

VideoPlayer::VideoPlayer()
{
}

VideoPlayer::~VideoPlayer()
{
}

bool VideoPlayer::Awake(pugi::xml_node&)
{
	active = false;
	// TODO 2: Activate the library
	// The AVIFileInit function initializes the AVIFile library.


	return true;
}

bool VideoPlayer::Start()
{
	return true;
}

bool VideoPlayer::StartVideo(char* filePath)
{

	active = true;
	finish = false;
	frameIndex = 0;
	nFrames = 0;
	width = 0;
	height = 0;
	state = START;

	path = filePath;
	audio = "";
	video = "";

	// TODO 1.1: We make the paths for the video and audio files from the filePath 
	int size = strlen(filePath);
	int nieuwSize = size + 5;
	audio = (char*)malloc(nieuwSize);
	strcpy_s(audio, nieuwSize, path.GetString());
	strcat_s(audio, nieuwSize, ".ogg");

	video = (char*)malloc(nieuwSize);
	strcpy_s(video, nieuwSize, path.GetString());
	strcat_s(video, nieuwSize, ".avi");


	// TODO 2: Access the video information 
	// Open input AVI file


	// Get file info


	// Save width and height of video
	width = fileInfo.dwWidth;
	height = fileInfo.dwHeight;

	// Get video stream


	// TODO 3: Set the program to the framerate of the video 
	
	// nFrames = // Video frames  
	// timeVideo =  // Video time in seconds 

	FPS = nFrames / timeVideo; // fps of the video 
	prevFPS = app->GetFramerate(); // Save actual framerate
	app->ChangeFPS(FPS); // Change FPS

	/* 
	TODO 4: Decompressing the frames of the video stream 
	This function returns a PGETFRAME
	We will be able to visualize individual frames of an open sequence.
	*/
	


	// TODO BONUS: Initialize skip bar parameters 
	skip = false;
	skipBarMax = { (int)WINDOW_W - 100,(int)WINDOW_H - 40,60,15 };
	skipBar = { (int)WINDOW_W - 100,(int)WINDOW_H - 40,0,15 };
	skipBarV = (int)(prevFPS/FPS);

	return true;
}

void VideoPlayer::NextFrame()
{

	// TODO 5: Create a texture of the current frame 
	// Create a bitmap for select frame
	LPBITMAPINFOHEADER bitmap = NULL;
	bitmap = (LPBITMAPINFOHEADER)AVIStreamGetFrame(frmSequence, frameIndex);
	// Pointer to bitmap data
	pointerData = (char*)bitmap + bitmap->biSize + bitmap->biClrUsed * sizeof(RGBQUAD); 
	// Create a surface of the current frame and with the surface obtain a texture.
	surface = SDL_CreateRGBSurfaceFrom(pointerData, width, height, bitmap->biBitCount, width * 3, 0, 0, 0, 0);
	
	// Get the texture


	// Next frame
	frameIndex++;
}

bool VideoPlayer::Update(float dt)
{
	
	switch (state)
	{
	case START:
	{
		// TODO 6: Play music

		NextFrame();
		state = PLAY;
	}
		break;
	case PLAY:
	{
		NextFrame();
		if (frameIndex >= nFrames) {
			state = FINISH;
		}
	}
		break;
	case FINISH:
	{
		CloseVideoPlayer();
	}
		break;
	default:
		break;
	}
	
	// TODO BONUS: Logic Skip Bar
	/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		skip = true;
	}*/

	return true;
}

bool VideoPlayer::PostUpdate()
{
	if (state == PLAY)
	{
		// TODO 7: Draw the texture of the frame.(180 angle because the image obtained is rotated)
		

		// TODO BONUS: Draw Skip Bar.
		if (skip) {


		}

		// TODO 7:To avoid memory overload, we will UnLoad the texture and free the surface after each frame is painted. 


	}

	return true;
}

void VideoPlayer::CloseVideoPlayer()
{
	frameIndex = 0;
	finish = true;

	// TODO 7.1:Remember to unload the texture and free the surface here too.


	active = false;
}

bool VideoPlayer::CleanUp()
{
	bool ret = true;
	if (!active)
		return true;

	LOG("Freeing scene");

	CloseVideoPlayer();

	// TODO 2.1: Close the library


	return ret;
}

