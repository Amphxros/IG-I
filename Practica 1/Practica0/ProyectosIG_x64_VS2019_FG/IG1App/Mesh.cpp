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

	draw();

    glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
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
	double ang= 3.1416 / 2;
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

	//pasamos los vertices a nuestra mesh
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