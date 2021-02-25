//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity(): mModelMat(1.0) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() {};

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	
protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	glm::dvec4 mColor;    // modeling matrix

	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const; 
};
//-------------------------------------------------------------------------

class EjesRGB : public Abs_Entity 
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Poligono : public Abs_Entity
{

public:
	explicit Poligono(GLdouble numL, GLdouble rd);
	~Poligono();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	void setColor(glm::dvec3 color) { mColor = glm::dvec4( color.r, color.g,color.b,1 ); }
};


#endif //_H_Entities_H_