#include "SceneManager.h"

#include "SceneIntro.h"
#include "Scene.h"
#include "Fonts.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager(Input* input, Render* render, Textures* tex) : Module()
{
	name.Create("scene_manager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;

	this->input = input;
	this->render = render;
	this->tex = tex;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	LOG("Loading Scene manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	current = new SceneIntro;
	current->Start();
	guiFont = app->fonts->Load("Assets/Fonts/RPGSystem.ttf", 25);
	titleFont = app->fonts->Load("Assets/Fonts/title_font.ttf", 48);
	next = nullptr;

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	bool ret = true;

	if (!onTransition)
	{

		ret = current->Update(dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				current->CleanUp();	// Unload current screen
				next->Start();	// Load next screen

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::INTRO: next = new SceneIntro(); break;
		case SceneType::LEVEL1: next = new Scene(); break;
		default: break;
		}

		current->transitionRequired = false;
	}
	

	return ret;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	// Draw current scene
	current->PostUpdate();
	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, WINDOW_W, WINDOW_H }, 0, 0, 0, (unsigned char)(255.0f * transitionAlpha));
	}	

	return ret;
}

void SceneManager::AddScene(SceneControl* scene, bool active)
{
	scene->active = active;
	scenes.Add(scene);
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");
	if (current != nullptr) current->CleanUp();

	app->fonts->UnLoad(0);
	app->fonts->UnLoad(1);

	input = nullptr;
	render = nullptr;
	tex = nullptr;
	current = nullptr;
	next = nullptr;
	transition = nullptr;

	scenes.Clear();

	return true;
}

bool SceneManager::LoadState(pugi::xml_node& data)
{
	
	return true;
}

bool SceneManager::SaveState(pugi::xml_node& data) const
{

	return true;
}
