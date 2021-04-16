#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include<iostream>
using namespace glm;
//-------------------------------------------------------------------------

Scene::Scene(){
	gTextures.reserve(10);
	
	Texture* tex1 = new Texture();
	tex1->load("../BmpsP1/baldosaC.bmp");
	gTextures.push_back(tex1);

	Texture* tex2 = new Texture();
	tex2->load("../BmpsP1/baldosaF.bmp");
	gTextures.push_back(tex2);

	Texture* tex3 = new Texture();
	tex3->load("../BmpsP1/baldosaP.bmp");
	gTextures.push_back(tex3);

	Texture* tex4 = new Texture();
	tex4->load("../BmpsP1/container.bmp");
	gTextures.push_back(tex4);

	Texture* tex5 = new Texture();
	tex5->load("../BmpsP1/grass.bmp", glm::u8vec3(0, 0, 0) , 0);
	gTextures.push_back(tex5);

	Texture* tex6 = new Texture();
	tex6->load("../BmpsP1/noche.bmp");
	gTextures.push_back(tex6);

	Texture* tex7 = new Texture();
	tex7->load("../BmpsP1/papelC.bmp");
	gTextures.push_back(tex7);

	Texture* tex8 = new Texture();
	tex8->load("../BmpsP1/papelE.bmp");
	gTextures.push_back(tex8);

	Texture* tex9 = new Texture();
	tex9->load("../BmpsP1/windowC.bmp");
	gTextures.push_back(tex9);

	Texture* tex10 = new Texture();
	tex10->load("../BmpsP1/windowV.bmp", 255 * 0.5);
	gTextures.push_back(tex10);
}

void Scene::init()
{
	setGL();  // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures
	
	// Graphics objects (entities) of the scene
	gObjects.clear();
	gObjectsTranslucidos.clear();
	switch(mId)
	{
	case 0:
		escena2D();
		break;
	case 1:
		escena3D();
		break;
	default:
		escena2D();
		break;
	}


	//gObjects.push_back(new Rectangulo(100, 100));
}

void Scene::escena3D() {
	gObjects.push_back(new EjesRGB(400.0));

	Estrella* st = new Estrella(80, 8, 80,0,0);
	
	st->setModelMat(glm::translate(st->modelMat(), dvec3(-150, 200, -150)));
	st->setTexture(gTextures[2]);
	gObjects.push_back(st);
	
	CajaFondo* cajFondo = new CajaFondo(50);
	cajFondo->setModelMat(glm::translate(cajFondo->modelMat(), dvec3(0, 25, 0)));
	cajFondo->setTexture(gTextures[3]);
	cajFondo->setTextureAlt(gTextures[6]);
	gObjects.push_back(cajFondo);

	Suelo* suelo = new Suelo(600, 600, 7, 7);
	//Suelo* suelo = new Suelo(70, 70);
	suelo->setColor(dvec3(1.0,1.0,0));
	suelo->setTexture(gTextures[0]);
	gObjects.push_back(suelo);

	Planta* p = new Planta(200, 200);
	p->setTexture(gTextures[4]);
	gObjects.push_back(p);

	Cristalera* c = new Cristalera(600);
	c->setTexture(gTextures.back());
	gObjectsTranslucidos.push_back(c);

	Foto* foto = new Foto(128, 70);
	foto->setTexture(gTextures[5]);
	gObjects.push_back(foto);


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
	for (Texture* el : gTextures)
	{
		delete el;  el = nullptr;
	}
	gTextures.clear();

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}

	for (Abs_Entity* el : gObjectsTranslucidos)
	{
		delete el;  el = nullptr;
	}
	gObjects.clear();
	gObjectsTranslucidos.clear();
}
//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test
	glEnable(GL_TEXTURE_2D); // activar el uso de texturas

}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test
	glDisable(GL_TEXTURE_2D); // desactivar el uso de texturas
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const 
{
	cam.upload();

	for (Abs_Entity* el : gObjects)
	{
	  el->render(cam.viewMat());
	}
	for (Abs_Entity* el : gObjectsTranslucidos)
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
		for (Abs_Entity* el : gObjectsTranslucidos)
		{
			el->update();
		}
	}
}

void Scene::updateTriangle()
{
	for (Abs_Entity* el : gObjects)
	{
		el->update();
	}
}

void Scene::changeScene(int id)
{
	mId = id;
	init();
}

void Scene::savePhoto(){
	if(mId==1){
		gObjects.back()->update(); //para sobreescribir la textura de la foto
		gTextures[5]->save("../BmpsP1/Photo.bmp"); //y guardar la foto
		std::cout << "guardado";
	}
}

//-------------------------------------------------------------------------