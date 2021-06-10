//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"

#include "Texture.h"
#include "Light.h";
#include "Material.h"

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

class CajaFondo : public Abs_Entity
{
public:
	explicit CajaFondo(GLdouble ld);
	~CajaFondo() { delete mMesh; delete meshFondo; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
private:
	Mesh* meshFondo = nullptr;
	glm::dmat4 mModelMatFondo = glm::dmat4(1.0);
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


class Cristalera :public Abs_Entity {
public:
	explicit Cristalera(GLdouble ld);
	~Cristalera() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Planta:public Abs_Entity {
public:
	explicit Planta(GLdouble w, GLdouble h);
	~Planta() { delete mMesh; };
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

/////////////////////////////////////////////////////////////////////////////////

//ENTIDADES CUÁDRICAS
class QuadricEntity : public Abs_Entity {
public:
	QuadricEntity();
	~QuadricEntity() { gluDeleteQuadric(q); };
protected:
	GLUquadricObj* q;
};

class Sphere : public QuadricEntity {
public:
	Sphere(GLdouble r); // r es el radio de la esfera 
	virtual ~Sphere() {}
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble r_;
};
class Cylinder : public QuadricEntity {
public:
	Cylinder(GLdouble baseR, GLdouble topR, GLdouble height);
	virtual ~Cylinder() {}
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble baseR_=0;
	GLdouble topR_=0;
	GLdouble height_=0;
};
class Disk : public QuadricEntity {
public:
	Disk(GLdouble innerR, GLdouble outerR);
	Disk(GLdouble innerR, GLdouble outerR, GLint slices, GLint loops); // personalizado
	virtual ~Disk() {}
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble innerR_;
	GLdouble outerR_;
	GLint slices_; // personalizado
	GLint loops_; // personalizado
};
class PartialDisk : public QuadricEntity {
public:
	PartialDisk(GLdouble innerR, GLdouble outerR, GLdouble startA, GLdouble sweepA);
	virtual ~PartialDisk() {}
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble innerR_;
	GLdouble outerR_;
	GLdouble startA_;
	GLdouble sweepA_;
};

///////////////////////////////////////////////////////////////////////////
class EntityWithIndexMesh: public Abs_Entity {
public:
	EntityWithIndexMesh() : mModelMat(1.0) {};  // 4x4 identity matrix
	virtual ~EntityWithIndexMesh() {};

	EntityWithIndexMesh(const EntityWithIndexMesh& e) = delete;  // no copy constructor
	EntityWithIndexMesh& operator=(const EntityWithIndexMesh& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method
	virtual void update() {};
	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };

	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(glm::dvec3 color) { mColor = glm::dvec4(color.r, color.g, color.b, 1); }
	void setTexture(Texture* tex) { mTexture = tex; }; // establecer textura
	void setTextureAlt(Texture* tex) { mTextureAlt = tex; }; // establecer textura

protected:
	IndexMesh* mIndexMesh = nullptr;   // the mesh
	Texture* mTexture = nullptr; // atributo para la textura principal
	Texture* mTextureAlt = nullptr; // atributo para la textura alternativa
	glm::dmat4 mModelMat;    // modeling matrix

	glm::dvec4 mColor;    // modeling matrix
	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;
};

class AnilloCuadrado : public Abs_Entity {
public:
	AnilloCuadrado();
	virtual ~AnilloCuadrado() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

class Cubo : public EntityWithIndexMesh {
public:
	Cubo();
	virtual ~Cubo() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

////////////////////////ENTIDADES COMPUESTAS///////////////////////////////////

class CompoundEntity : public Abs_Entity {
public:
	CompoundEntity() {} ;
	~CompoundEntity() { freeCEntity(); };

	void addEntity(Abs_Entity* ae) { gObjectsCEntity.push_back(ae); };
	void addEntityTranslucida(Abs_Entity* ae) { gObjectsTranslucidosCEntity.push_back(ae); };
	void addLight(Light* l) { luces.push_back(l); }
	void render(glm::dmat4 const& modelViewMat) const override;

	void enableLights();
	void disableLights();

	std::vector<Abs_Entity*> gEntities() { return gObjectsCEntity; }
	std::vector<Abs_Entity*> gEntitiesT() { return gObjectsTranslucidosCEntity; }


protected:
	std::vector<Abs_Entity*> gObjectsCEntity;
	std::vector<Abs_Entity*> gObjectsTranslucidosCEntity;
	std::vector<Texture*> gTextureCEntity;
	void freeCEntity();

	std::vector<Light*> luces;

};

class TIE:
	public CompoundEntity {
public:
	TIE();
	virtual ~TIE() {};
};

class TIEIluminado : public CompoundEntity {
public:
	TIEIluminado();
	virtual ~TIEIluminado() {};
};

class TIEFormation :
	public CompoundEntity {
public:
	TIEFormation(GLdouble rd);
	virtual ~TIEFormation() {};
	void rota();
	void orbita();
	void turnLights(bool b);
	void uploadLights(glm::dmat4 const& modelViewMat);
protected:
	GLdouble rd_Orbita;
	GLdouble rd_rotation;
	
	GLdouble angle_Orbita;
	GLdouble angle_TIEs;
	GLdouble angle_rotation;
};

/////////////////////////////////////////////////////////////////

class EntityWithMaterial : public Abs_Entity {
public:
	EntityWithMaterial() : Abs_Entity() { };
	virtual ~EntityWithMaterial() { };
	void setMaterial(Material* mat) { mat_ = mat; };
protected:
	Material* mat_ = nullptr;
};

class Cono: public EntityWithIndexMesh
{
public:
	Cono(GLdouble h, GLdouble r, GLuint n);
	virtual ~Cono() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

class Esfera: public EntityWithMaterial
{
public:
	Esfera(GLdouble r, GLint p, GLint m);
	virtual ~Esfera() {};
	void render(glm::dmat4 const& modelViewMat) const;
};

class Grid : public EntityWithIndexMesh {
public :
	Grid(GLdouble l, GLint nDiv);
	virtual ~Grid(){}
	void render(glm::dmat4 const& modelViewMat) const;

};

class GridCube : public CompoundEntity {
public:
	GridCube(GLdouble l, GLuint nDiv);
	virtual ~GridCube() {}
};

////////////////////////////////////////////////////////////////////////

#endif //_H_Entities_H_