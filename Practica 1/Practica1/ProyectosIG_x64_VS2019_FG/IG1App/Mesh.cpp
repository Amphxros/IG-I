#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
#include<glm.hpp>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const 
{
  glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const 
{
  if (vVertices.size() > 0) {  // transfer data
    // transfer the coordinates of the vertices
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 
    
if (vColors.size() > 0) { // transfer colors
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
    }
	if (vTextures.size() > 0) {
		// activar array de coordenadas de textura
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_DOUBLE, 0, vTextures.data());
	}

	draw();

	//Desactivamos TODO
    glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  }
}
//-------------------------------------------------------------------------

Mesh * Mesh::createRGBAxes(GLdouble l)
{
  Mesh* mesh = new Mesh();

  mesh->mPrimitive = GL_LINES;

  mesh->mNumVertices = 6;
  mesh->vVertices.reserve(mesh->mNumVertices);

  // X axis vertices
  mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
  mesh->vVertices.emplace_back(l, 0.0, 0.0);
  // Y axis vertices
  mesh->vVertices.emplace_back(0, 0.0, 0.0);
  mesh->vVertices.emplace_back(0.0, l, 0.0);
  // Z axis vertices
  mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
  mesh->vVertices.emplace_back(0.0, 0.0, l);

  mesh->vColors.reserve(mesh->mNumVertices);
  // X axis color: red  (Alpha = 1 : fully opaque)
  mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
  mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
  // Y axis color: green
  mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
  mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
  // Z axis color: blue
  mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
  mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
 
  return mesh;
}

Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = numL;
	mesh->vVertices.reserve(mesh->mNumVertices);
	
	glm::dvec2 centro = { 0.0, 0.0 };
	double ang= 3.1416 / 2; //angulo inicial 

	//bucle que calcula n puntos 
	for(int i=0; i < mesh->mNumVertices; i++)
	{		
		double x,y;
		ang += (2 * 3.1416)/  numL ;
		x = centro.x + rd * cos(ang);
		y = centro.y + rd * sin(ang);
		
		mesh->vVertices.emplace_back(x, y, 0.0);
	}
	
	return mesh;
}

Mesh* Mesh::generaSierpinski(GLdouble rd, GLuint numP)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_POINTS;
	mesh->mNumVertices = numP;
	mesh->vVertices.reserve(mesh->mNumVertices);

	Mesh * triangulo = generaPoligono(3, rd); //triangulo de inicio

	//pasamos los vertices del triangulo a nuestra mesh
	for (int i = 0; i < triangulo->mNumVertices; i++)
	{
		mesh->vVertices.emplace_back(triangulo->vVertices[i]);
	}
	//calculamos el resto de puntos
	for (int i = 2; i < mesh->mNumVertices; i++)
	{
		glm::dvec3 pA = mesh->vVertices[i]; //punto A desde donde empezamos a calcular
		glm::dvec3 pB = mesh->vVertices[rand() % 3]; //punto B random
	
		glm::dvec3 pM; //punto medio
		pM.x= (pA.x + pB.x )/2;
		pM.y = (pA.y + pB.y) / 2;
		pM.z = (pA.z + pB.z) / 2;

		mesh->vVertices.emplace_back(pM );

	}
	
	//borramos el triangulo porque ya no nos interesa
	delete triangulo;
	triangulo = nullptr;

	return mesh;
}

Mesh* Mesh::generaTrianguloRGB(GLdouble rd)
{
	Mesh * mesh = generaPoligono(3, rd); //generamos un triangulo
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->vColors.reserve(mesh->mNumVertices);
		
	// vector de colores
	mesh->vColors.emplace_back(1.0,0.0,0.0,1.0);
	mesh->vColors.emplace_back(0.0,1.0,0.0,1.0);
	mesh->vColors.emplace_back(0.0,0.0,1.0,1.0);

	return mesh; 
}

Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h)
{
	//w/2 y h/2 para que este centrado
	GLdouble w_ = w / 2;
	GLdouble h_ = h / 2;
	
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);

	//como la primitiva es triangle strip tenemos que dar los vertices en diferente orden
	mesh->vVertices.emplace_back(-w_, h_, 0.0);
	mesh->vVertices.emplace_back(-w_, -h_, 0.0);
	mesh->vVertices.emplace_back(w_, h_, 0.0);
	mesh->vVertices.emplace_back(w_, -h_, 0.0);

	return mesh;
}

Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h)
{
	Mesh* mesh = generaRectangulo(w, h);
	mesh->vColors.reserve(mesh->mNumVertices);

	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh* Mesh::generaRectanguloConTextura(GLdouble w,GLdouble h,GLuint rw,GLuint rh ){
	Mesh* mesh = generaRectangulo(w,h);
	mesh->vTextures.reserve(mesh->mNumVertices);

	mesh->vTextures.push_back(dvec2(0,0));
	mesh->vTextures.push_back(dvec2(0,rh));
	mesh->vTextures.push_back(dvec2(rw,0));
	mesh->vTextures.push_back(dvec2(rw,rh));

	return mesh;
}
Mesh* Mesh::generaContCubo(GLdouble ld)
{
	Mesh* mesh= new Mesh();
	
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;

	mesh->vVertices.reserve(mesh->mNumVertices);
	//mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vTextures.reserve(mesh->mNumVertices);

	GLdouble l=ld/2;

	//1º cara (Z positiva)
	mesh->vVertices.push_back(dvec3(-l,l,l)); //V0
	mesh->vTextures.push_back(dvec2(0.0, 1.0));
	mesh->vVertices.push_back(dvec3(-l,-l,l)); //V1
	mesh->vTextures.push_back(dvec2(0.0, 0.0));
	mesh->vVertices.push_back(dvec3(l,l,l)); //V2
	mesh->vTextures.push_back(dvec2(1.0, 1.0));
	mesh->vVertices.push_back(dvec3(l,-l,l)); //V3
	mesh->vTextures.push_back(dvec2(1.0, 0.0));

	//2º cara (Z negativa)
	mesh->vVertices.push_back(dvec3(l,l,-l)); //V4
	mesh->vTextures.push_back(dvec2(2.0, 1.0));
	mesh->vVertices.push_back(dvec3(l,-l,-l)); //V5
	mesh->vTextures.push_back(dvec2(2.0, 0.0));
	mesh->vVertices.push_back(dvec3(-l,l,-l)); //V6
	mesh->vTextures.push_back(dvec2(3.0, 1.0));
	mesh->vVertices.push_back(dvec3(-l,-l,-l)); //V7
	mesh->vTextures.push_back(dvec2(3.0, 0.0));

	//luego enlazamos con los del principio con 2 vertices mas
	mesh->vVertices.push_back(dvec3(-l,l,l)); //V0
	mesh->vTextures.push_back(dvec2(4.0, 1.0));
	mesh->vVertices.push_back(dvec3(-l,-l,l)); //V1
	mesh->vTextures.push_back(dvec2(4.0, 0.0));

	return mesh;
}

Mesh* Mesh::generaEstrella3D(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 2* (np +1);

	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.push_back(dvec3(0,0,0));
	

	GLdouble ri = re / 2; //ri= radio interior
	double ang= 3.1416 / 2; //angulo inicial

 
	for(int i=0; i<mesh->mNumVertices-2; i++)
	{
		double x,y;
		ang += (2 * 3.1416)/(mesh->mNumVertices-2);
		if(i%2==0){ // circunferencia grande
			x = re * cos(ang);
			y = re * sin(ang);
		}
		else{ // circunferencia pequeña
			x = ri * cos(ang);
			y = ri * sin(ang);
			

		}
		mesh->vVertices.push_back(dvec3(x, y, h));
			
	}
	mesh->vVertices.push_back(mesh->vVertices[1]);
	
	return mesh;
}
Mesh* Mesh::generaEstrella3DconTextura(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 2* (np +1);

	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vTextures.reserve(mesh->mNumVertices);

	mesh->vColors.push_back(dvec4(1.0, 1.0, 0.0, 1.0));
	mesh->vVertices.push_back(dvec3(0,0,0));
	mesh->vTextures.push_back(dvec2(0.5, 0.5));

	GLdouble ri = re / 2; //ri= radio interior
	double ang= 3.1416 / 2; //angulo inicial

	double r_texture=0.5; // radio para la textura
 
	for(int i=0; i<mesh->mNumVertices-2; i++){
		double x,y;
		ang += (2 * 3.1416)/(mesh->mNumVertices-2);
		if(i%2==0){ // circunferencia grande
			x = re * cos(ang);
			y = re * sin(ang);
			r_texture = 0.5;
		}
		else{ // circunferencia pequeña
			x = ri * cos(ang);
			y = ri * sin(ang);
			r_texture=0.25;

		}
		double x_tex = 0.5 + r_texture * cos(ang);
		double y_tex = 0.5 + r_texture * sin(ang);

		mesh->vVertices.push_back(dvec3(x, y, h));
		mesh->vColors.push_back(dvec4(1.0, 1.0, 0.0, 1.0));
		mesh->vTextures.push_back(dvec2(x_tex, y_tex));
	}
	mesh->vVertices.push_back(mesh->vVertices[1]);
	mesh->vColors.push_back(dvec4(1.0, 1.0, 0.0, 1.0));
	mesh->vTextures.push_back(mesh->vTextures[1]);

	return mesh;
}

