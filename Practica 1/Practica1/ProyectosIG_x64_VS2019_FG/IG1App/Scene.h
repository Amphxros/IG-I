//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

#include "Texture.h"

//-------------------------------------------------------------------------

class Scene	
{ 
public:
	Scene() {};
	~Scene() { free(); resetGL(); };

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment
		
	void init();

	void changeScene(int id); //elimina la escena actual y genera la nueva
    void render(Camera const& cam) const;

	void update();
	void updateTriangle();
	void setUpdate() { can_update = !can_update; }
	
	
protected:
	void free();
	void setGL();
	void resetGL();

	void escena2D();
	void escena3D();
	std::vector<Texture*> gTextures;
	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	bool can_update=true;

	int mId = 0; //identificador de escena
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

