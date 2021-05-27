//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>
#include "Light.h"
#include "Texture.h"

//-------------------------------------------------------------------------

class Scene	
{ 
public:
	Scene();

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment
		
	~Scene() { free();  resetGL(); }
	void init();

	void changeScene(int id); //elimina la escena actual y genera la nueva
    void render(Camera const& cam) const;

	void update();
	void updateTriangle();
	void setUpdate() { can_update = !can_update; }
	inline int getmID() { return mId; }

	void savePhoto();

	void sceneDirLight(Camera const& cam) const;
	
	DirLight* getDirLight() { return dirLight; }
	void enableDirLight() { isLightEnabled = true; }
	void disableDirLight() { isLightEnabled = false; }


protected:
	void free();
	void setGL();
	void resetGL();

	void escena2D();
	void escena3D();
	void escenaCaza();
	void escenaAnillo();
	void escenaIndices();
	void escenaLuces();

	void escenaCono();
	void escenaEsferas();
	void escenaGrids();

	std::vector<Texture*> gTextures;	//vector de texturas
	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gObjectsTranslucidos;  // Entities (graphic objects) of the scene
	bool can_update=true;

	bool isLightEnabled = false;

	int mId = 0; //identificador de escena

	DirLight* dirLight;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

