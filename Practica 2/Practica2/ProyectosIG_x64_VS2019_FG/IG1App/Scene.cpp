#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include<iostream>
using namespace glm;
//-------------------------------------------------------------------------

Scene::Scene(){
	gTextures.reserve(15);
	
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
	tex6->load("../BmpsP1/noche.bmp", 255 * 0.75);
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

	Texture* tex11 = new Texture();
	tex11->load("../BmpsP1/checker.bmp");
	gTextures.push_back(tex11);
	
	Texture* tex12 = new Texture();
	tex12->load("../BmpsP1/stones.bmp");
	gTextures.push_back(tex12);

	//luces
	glm::fvec4 ambient, diffuse, specular;

	dirLight = new DirLight(fvec3(1, 1, 1));
	
	posLight = new PosLight(fvec3(600, 600, 0));
	//ambient = { 0.0, 0.0, 0.0, 1 };
	//diffuse = { 1, 2, 0, 1 };
	//specular = { 0.5, 0.5, 1.5, 1 };
	//posLight->setAmb(ambient);
	//posLight->setDiff(diffuse);
	//posLight->setSpec(specular);


	spotLight= new SpotLight(fvec3(0,0,-1), 20, 5, fvec3(0, 0, 700));
	//ambient = { 0, 0, 0, 1 };
	//diffuse = { 0.3, 0.3, 0.3, 1 };
	//specular = { 0.5, 1, 0.5, 0.5 };
	//spotLight->setAmb(ambient);
	//spotLight->setDiff(diffuse);
	//spotLight->setSpec(specular);
	//spotLight->setAtte(0.5, 0.5, 0.5);
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
	case 2:
		escenaCaza();
		break;
	case 3:
		escenaIndices();
		break;
	case 4:
		escenaAnillo();
		break;
	case 5:
		escenaCono();
		break;
	case 6:
		escenaEsferas();
		break;
	case 7:
		escenaGrids();
		break;
	case 8:
		escenaLuces();
		break;
	default:
		escena2D();
		break;
	}

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
	c->setTexture(gTextures[9]);
	gObjectsTranslucidos.push_back(c);

	Foto* foto = new Foto(128, 70);
	foto->setTexture(gTextures[6]);
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

void Scene::escenaCaza() {
	gObjects.push_back(new EjesRGB(400.0));

	TIE* caza = new TIE();
	gObjects.push_back(caza);
}

void Scene::escenaAnillo()
{
	gObjects.push_back(new EjesRGB(400.0));
	gObjects.push_back(new AnilloCuadrado());
}

void Scene::escenaIndices() {
	gObjects.push_back(new EjesRGB(400.0));
	gObjects.push_back(new Cubo());
}

void Scene::escenaCono()
{
	gObjects.push_back(new EjesRGB(400.0));
	Cono* c = new Cono(100, 50, 50);
	c->setColor(dvec3(0.5, 0.0, 1.0));
	gObjects.push_back(c);
}

void Scene::escenaEsferas()
{

	gObjects.push_back(new EjesRGB(400.0));
	Sphere* sphere = new Sphere(100);
	sphere->setColor(dvec3(0.0, 0.0, 1.0));
	gObjects.push_back(sphere);

	Esfera* e = new Esfera(100, 30, 30);
	e->setModelMat(glm::translate(e->modelMat(), dvec3(250, 0, 0)));
	e->setColor(dvec3(0.0, 0.0, 1.0));
	gObjects.push_back(e);
}

void Scene::escenaGrids()
{

	gObjects.push_back(new EjesRGB(400.0));
	GridCube* c = new GridCube(200, 5);
	gObjects.push_back(c);
}

void Scene::escenaLuces() {

	Material* m = new Material();
	m->setBrass();
	Esfera* e = new Esfera(400, 500, 500);
	e->setMaterial(m);
	e->setColor(dvec3(0.5, 0.65, 0.65)); //e->setColor(dvec3(0, 1, 1));
	gObjects.push_back(e);

	TIEFormation* f = new TIEFormation(410);
	gObjects.push_back(f);
}




void Scene::TIEsLightsOn()
{
	if (mId == 8)
	{
		//encender los focos de los tres TIE’s
		static_cast<TIEFormation*>(gObjects.back())->turnLights(true);
	}
}

void Scene::TIEsLightsOff()
{
	if (mId == 8)
	{
		//apagar los focos de los tres TIE’s
		static_cast<TIEFormation*>(gObjects.back())->turnLights(false);
	}
}

//-------------------------------------------------------------------------
void Scene::free() 
{ // release me2mory and resources
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

	delete dirLight;
	delete posLight;
	delete spotLight;

	gObjects.clear();
	gObjectsTranslucidos.clear();
}
//-------------------------------------------------------------------------
void Scene::setGL()
{
	// OpenGL basic setting
	switch (mId)
	{
	case 0:
		glClearColor(1.0, 1.0, 1.0, 1.0);  // background color (alpha=1 -> opaque)
		break;
	case 1:
		glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
		break;
	case 2:
		glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		glClearColor(0.7, 0.8, 0.9, 1.0);  // background color (alpha=1 -> opaque)
		break;
	case 8:
		glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
		break;
	default:
		glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
		break;
	}
	glEnable(GL_DEPTH_TEST);  // enable Depth test
	glEnable(GL_TEXTURE_2D); // activar el uso de texturas
	glEnable(GL_NORMALIZE); // evitar sobreexposición
}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test
	glDisable(GL_TEXTURE_2D); // desactivar el uso de texturas
	glDisable(GL_NORMALIZE); // evitar sobreexposición
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const 
{
	//Luz
	if (mId >= 2) {
		// ¿Alguna luz de escena?

		if (isDirLightEnabled || isPosLightEnabled || isSpotLightEnabled) {
			glEnable(GL_LIGHTING);
			
			dirLight->upload(cam.viewMat());
			posLight->upload(cam.viewMat());
			spotLight->upload(cam.viewMat());

			
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixd(value_ptr(cam.viewMat()));
		}
		else {
			dirLight->disable();
			posLight->disable();
			spotLight->disable();
			
			glm::fvec4 amb = { 0.0, 0.0, 0.0, 1.0 };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, value_ptr(amb));
		}
	}
	else {
		dirLight->disable();
		posLight->disable();
		spotLight->disable();
		glm::fvec4 amb = { 0.0, 0.0, 0.0, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, value_ptr(amb));
		
		//glDisable(GL_LIGHTING);
	}
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

void Scene::orbita()
{
	if (mId == 8) {
		if (dynamic_cast<TIEFormation*>(gObjects.back()) != nullptr) {
			static_cast<TIEFormation*>(gObjects.back())->orbita();
		}
	}
}

void Scene::rota()
{
	if (mId == 8) {
		if (dynamic_cast<TIEFormation*>(gObjects.back()) != nullptr) {
			static_cast<TIEFormation*>(gObjects.back())->rota();
		}
	}
}

void Scene::changeScene(int id)
{
	mId = id;
	init();
}

void Scene::savePhoto(){
	if(mId == 1){
		gObjects.back()->update(); //para sobreescribir la textura de la foto
		gTextures[6]->save("../BmpsP1/Photo.bmp"); //y guardar la foto
		std::cout << "guardado";
	}
}

void Scene::disableAllLights()
{
	disableDirLight();
	disablePosLight();
	disableSpotLight();

	glm::fvec4 amb = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, value_ptr(amb));
}

//-------------------------------------------------------------------------