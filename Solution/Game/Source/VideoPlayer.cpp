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
	skip = false;
	frameIndex = 0;
	nFrames = 0;
	width = 0;
	height = 0;
	state = PLAY;

	path = filePath;
	audio = "";
	video = "";
	//TODO +: We make the paths for the video and audio files from the filePath 
	int size = strlen(filePath);
	int nieuwSize = size + 5;
	audio = (char*)malloc(nieuwSize);
	strcpy_s(audio, nieuwSize, path.GetString());
	strcat_s(audio, nieuwSize, ".ogg");

	video = (char*)malloc(nieuwSize);
	strcpy_s(video, nieuwSize, path.GetString());
	strcat_s(video, nieuwSize, ".avi");

	//TODO +: The AVIFileInit function initializes the AVIFile library.
	AVIFileInit();

	/*
	TODO 2.1: Use AVIStreamOpenFromFile(...) to open a single stream from the AVI file.
	- The first parameter is a pointer to a buffer that receives the stream handle.
	- The second parameter is the path to the file.
	- The third parameter is the type of stream we want to open (in this case streamtypeVIDEO).
	- The fourth parameter is which video stream we want (there can be more than one), in this case: 0.
	*/
	/*if (AVIStreamOpenFromFile(&pavi, (LPCSTR)video, streamtypeVIDEO, 0, OF_READ, NULL) != 0) // Opens The AVI Stream
		LOG("Failed To Open The AVI Stream");
	*/
	// open input AVI file
	AVIFileOpenA(&pInFile, (LPCSTR)video, OF_SHARE_DENY_WRITE, NULL);
	//Get file info
	AVIFileInfo(pInFile, &pInFileInf, sizeof(AVIFILEINFO));

	//width = psi.rcFrame.right - psi.rcFrame.left;
	width = pInFileInf.dwWidth;
	height = pInFileInf.dwHeight;
	/*
	AVIStreamInfo(pavi, &psi, sizeof(psi));					// Reads Information About The Stream Into psi
	width = psi.rcFrame.right - psi.rcFrame.left;           // Width Is Right Side Of Frame Minus Left
	height = psi.rcFrame.bottom - psi.rcFrame.top;          // Height Is Bottom Of Frame Minus Top
	*/
	AVIFileGetStream(pInFile, &pInStream, streamtypeVIDEO, 0);
	if (pgf == NULL)
		LOG("Failed To Open The AVI Frame");

	nFrames = AVIStreamLength(pInStream);
	timeVideo = AVIStreamLengthTime(pInStream) / 1000;

	FPS = nFrames / timeVideo;
	prevFPS = app->GetFramerate();
	app->ChangeFPS(FPS);

	// TODO 3.1: Use AVIStreamFrameOpen(...) to decompress video frames from the AVI file. On the second parameter you can pass AVIGETFRAMEF_BESTDISPLAYFMT to select the best display format.
	// Hint: this function returns a PGETFRAME
	pgf = AVIStreamGetFrameOpen(pInStream, NULL);              // Create The PGETFRAME Using Our Request Mode
	if (pgf == NULL)
		LOG("Failed To Open The AVI Frame");


	//TODO BONUS: 
	skipBarMax = { (int)WINDOW_W - 100,(int)WINDOW_H - 40,60,15 };
	skipBar = { (int)WINDOW_W - 100,(int)WINDOW_H - 40,0,15 };
	skipBarV = (int)(prevFPS/FPS);

	return true;
}

void VideoPlayer::NextFrame()
{
	//Uncomment this after you have finished TODO 3.
	LPBITMAPINFOHEADER lpbi = NULL;													 // Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frameIndex);					// Grab Data From The AVI Stream
	pointerData = (char*)lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);    // Pointer To Data Returned By AVIStreamGetFrame
																				// (Skip The Header Info To Get To The Data)
																				// Convert Data To Requested Bitmap Format
	/*TODO 5.1: Create a surface using the bitmap data we have above this TODO, and create the texture of the frame with that surface (use LoadSurface from textures module)
			- pdata holds the texture data (pixels)
			- biBitCount holds the depht in bits and is contained in the LPBITMAPINFOHEADER structure
			- pitch is the length of a row of pixels in bytes (width * 3)
	*/
	surface = SDL_CreateRGBSurfaceFrom(pointerData, width, height, lpbi->biBitCount, width * 3, 0, 0, 0, 0);
	textureFrame = app->tex->LoadSurface(surface);

	//Next frame
	frameIndex++;
}

bool VideoPlayer::Update(float dt)
{
	switch (state)
	{
	case START:
	{
		app->audio->PlayMusic(audio);
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
		CleanUp();
	}
		break;
	default:
		break;
	}
	
	//TODO BONUS: Skip Bar
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		skip = true;
		skipBar.w += skipBarV;
		if (skipBar.w > skipBarMax.w)
		{
			skipBar.w = skipBarMax.w;
			CleanUp();
		}
	}
	else
	{
		if (skipBar.w > 0)
		{
			skipBar.w -= skipBarV;
		}
		else
		{
			skipBar.w = 0;
			skip = false;
		}
	}

	return true;
}

bool VideoPlayer::PostUpdate()
{
	if (state == PLAY)
	{
		//TODO +: Draw the texture of the frame.
		app->render->DrawTexture(textureFrame, (WINDOW_W / 2) - (width / 2), (WINDOW_H / 2) - (height / 2), NULL, 0, 180);
	
		//TODO BONUS:
		if (skip) {
			app->render->DrawRectangle(skipBarMax, 255, 255, 255, 255);
			app->render->DrawRectangle(skipBar, 59, 131, 189, 255);
		}
	
		// TODO +: To avoid memory overload, we will UnLoad the texture and free the surface after each frame is painted. 
		app->tex->UnLoad(textureFrame);
		SDL_FreeSurface(surface);
	}

	return true;
}

bool VideoPlayer::CleanUp()
{
	bool ret = true;
	if (!active)
		return true;

	LOG("Freeing scene");

	//RELEASE(audio);
	//RELEASE(video);

	frameIndex = 0;
	finish = true;

	// TODO +: We use AVIStreamGetFrameClose() and AVIStreamRelease() to release the resources used 
	AVIStreamGetFrameClose(pgf);
	AVIStreamRelease(pInStream);

	// TODO +: Remember that whenever we have made you AVIFileInit () we must also close it 
	AVIFileExit();

	Mix_HaltMusic();
	app->ChangeFPS(prevFPS);

	active = false;

	return ret;
}

