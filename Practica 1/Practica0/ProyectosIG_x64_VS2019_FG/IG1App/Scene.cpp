#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

void Scene::init()
{
	setGL();  // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.clear();
	switch(mId)
	{
	case 0:
		escena2D();
		break;
	case 1:
		break;
	default:
		escena2D();
		break;
	}


	//gObjects.push_back(new Rectangulo(100, 100));
}

void Scene::escena2D() {
	gObjects.push_back(new EjesRGB(400.0));

	Poligono* p = new Poligono(3, 300);
	p->setColor({ 1,1,0 });
	gObjects.push_back(p);

	Poligono* circ = new Poligono(100, 150);
	circ->setColor({ 1, 0, 1 });
	gObjects.push_back(circ);

	TrianguloSierpinsky* triSierp = new TrianguloSierpinsky(300, 10000);
	triSierp->setColor({ 1, 1, 0 });
	gObjects.push_back(triSierp);

	TrianguloRGB* rgb = new TrianguloRGB(30);
	rgb->setModelMat(glm::translate(rgb->modelMat(), dvec3(150, 0, 0)));
	rgb->setModelMat(glm::rotate(rgb->modelMat(), radians(-25.0), dvec3(0, 0, 1)));
	gObjects.push_back(rgb);

	RectanguloRGB* r = new RectanguloRGB(1200, 800);
	r->setModelMat(glm::translate(r->modelMat(), dvec3(150, 0, 0)));
	gObjects.push_back(r);
}
//-------------------------------------------------------------------------
void Scene::free() 
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
}
//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 

}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const 
{
	cam.upload();

	for (Abs_Entity* el : gObjects)
	{
	  el->render(cam.viewMat());
	}
}

void Scene::update()
{
	if (can_update) {
		for (Abs_Entity* el : gObjects)
		{
			el->update();
		}
	}
}

void Scene::changeScene(int id)
{
	mId = id;
	init();
}
//-------------------------------------------------------------------------