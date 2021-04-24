#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Scene.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public SceneControl
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	bool LoadState(pugi::xml_node& data);
	bool SaveState(pugi::xml_node& data)const;


private:

	int imgX = 0, imgY = 0, imgW = 0, imgH = 0;
	bool transition = false;

	pugi::xml_document sceneFile;
	pugi::xml_node sceneStateFile;

	char Text[20] = { "\0" };
	int posX = 0, posY = 0;
	int padding = 0;

	bool fullScreen = false;
};

#endif // __SCENEINTRO_H__



