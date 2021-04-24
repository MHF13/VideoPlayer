#ifndef __SCENE_H__
#define __SCENE_H__

#include "SceneControl.h"
#include "Animation.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;

class Scene : public SceneControl
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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
	int GetNumThisScene()
	{
		return numThisScene;
	}

	// Load state game
	bool LoadState(pugi::xml_node& data);
	// Save state game
	bool SaveState(pugi::xml_node& data)const;
	

private:

	int numThisScene = 1;

	Animation* idleAnim= new Animation();
	int imgX = 0, imgY = 0, imgW = 0, imgH = 0;
	float speedImg=0;

};

#endif // __SCENE_H__