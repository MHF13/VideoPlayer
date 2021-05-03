#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "SceneManager.h"
#include "VideoPlayer.h"

#include <SDL_mixer\include\SDL_mixer.h>

#include "Defs.h"
#include "Log.h"

Scene::Scene()
{
	active = true;
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	bool ret = true;
	LOG("Loading Scene");
	numThisScene = 1;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	videoActive = false;
	app->SetLastScene((Module*)this);

	// TODO 1: play video 
	

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	bool ret = true;

	if (videoActive){
		videoActive = !app->video->GetFinishVideo();
		return true;
	}

	//When the video ends we will return to the first scene 
	if (app->video->GetFinishVideo())
	{
		TransitionToScene(SceneType::INTRO);
	}

	return ret;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	bool ret = true;
	if (!active)
		return true;

	LOG("Freeing scene");

	active = false;
	return ret;
}

bool Scene::LoadState(pugi::xml_node& data)
{
	return true;
}

bool Scene::SaveState(pugi::xml_node& data) const
{

	return true;
}
