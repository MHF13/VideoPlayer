#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "SceneManager.h"

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
	LOG("Loading Scene");
	bool ret = true;
	numThisScene = 1;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	app->SetLastScene((Module*)this);

	victory=false;
	
	// Active calls
	app->audio->active = true;

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/level_music.ogg");

	//app->dialogueSystem->missClick = false;
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
