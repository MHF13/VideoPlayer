#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Fonts.h"
#include "SceneIntro.h"
#include "SceneManager.h"

#include <SDL_mixer\include\SDL_mixer.h>

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro()
{
	active = true;
	name.Create("sceneIntro");

}

SceneIntro::~SceneIntro()
{
}

bool SceneIntro::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool SceneIntro::Start()
{
	fullScreen = false;

	app->SetLastScene((Module*)this);
	transition = false;

	//app->audio->PlayMusic("Assets/Audio/Music/music_intro.ogg");

	app->render->camera.x = app->render->camera.y = 0;

	posX = 100, posY = 100;
	padding = 50;
	return true;
}

bool SceneIntro::PreUpdate()
{
	return true;
}

bool SceneIntro::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {

		TransitionToScene(SceneType::LEVEL1);
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {

		fullScreen = !fullScreen;
		app->fullScreen = fullScreen;
		app->win->FullScreen(app->fullScreen);
		app->render->FullScreen();
	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {

		return false;
	}
	return ret;
}

bool SceneIntro::PostUpdate()
{
	bool ret = true;

	for (int i = 0; i < 3; i++) {
		switch (i)
		{
		case 0:
			sprintf_s(Text, 20, "Play Video: 1");
			break;
		case 1:
			sprintf_s(Text, 20, "Full Screen: 2");
			break;
		case 2:
			sprintf_s(Text, 20, "Exit: 3");
			break;
		default:
			break;
		}

		app->fonts->BlitText(posX, posY + (padding * i), 0, Text, { 255, 255, 255 });
	}


	return ret;
}

bool SceneIntro::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing scene");
	Mix_HaltMusic();

	active = false;

	return true;
}

bool SceneIntro::LoadState(pugi::xml_node& data)
{

	return true;
}

bool SceneIntro::SaveState(pugi::xml_node& data) const
{

	return true;
}




