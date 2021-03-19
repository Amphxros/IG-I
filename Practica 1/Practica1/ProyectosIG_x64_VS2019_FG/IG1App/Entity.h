//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"

#include "Texture.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity() : mModelMat(1.0) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() {};

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method
	virtual void update() {}
	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(glm::dvec3 color) { mColor = glm::dvec4(color.r, color.g, color.b, 1); }
	void setTexture(Texture* tex) { mTexture = tex; }; // establecer textura
	void setTextureAlt(Texture* tex) { mTextureAlt = tex; }; // establecer textura

protected:
	Mesh* mMesh = nullptr;   // the mesh
	Texture* mTexture = nullptr; // atributo para la textura principal
	Texture* mTextureAlt = nullptr; // atributo para la textura alternativa
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
	~EjesRGB() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Poligono : public Abs_Entity
{

public:
	explicit Poligono(GLdouble numL, GLdouble rd);
	~Poligono(){ delete mMesh; }
	
	virtual void render(glm::dmat4 const& modelViewMat) const;

};

class TrianguloSierpinsky : public Abs_Entity
{
public:
	explicit TrianguloSierpinsky(GLdouble rd, GLdouble numP);
	~TrianguloSierpinsky() {delete mMesh; }

	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class TrianguloRGB : public Abs_Entity
{
public:
	explicit TrianguloRGB(GLdouble rd);
	~TrianguloRGB() {delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
private:
	double angleC, angleR; //angle es par
};

class RectanguloRGB : public Abs_Entity
{
public:
	explicit RectanguloRGB(GLdouble w, GLdouble h);
	~RectanguloRGB() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Estrella : public Abs_Entity
{
public:
	explicit Estrella(GLdouble re, GLuint np, GLdouble h, double angle_y, double angle_z);
	~Estrella() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();

private:
	double angleY, angleZ;
};

class Caja : public Abs_Entity
{
public:
	explicit Caja(GLdouble ld);
	~Caja() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Suelo : public Abs_Entity
{
public:
	explicit Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	explicit Suelo(GLdouble w, GLdouble h);
	~Suelo() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Foto : public Abs_Entity
{
public:
	explicit Foto(GLdouble w, GLdouble h);
	~Foto() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
};

#endif //_H_Entities_H_