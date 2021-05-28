#pragma once

#include <GL/freeglut.h>
#include <glm.hpp>
// Abstracta
class Light
{
public:
	Light();
	virtual ~Light() { disable(); };
	void uploadL();
	// Método abstracto
	virtual void upload(glm::dmat4 const& modelViewMat) {};
	// Setter’s para cambiar el valor de los atributos lumínicos
	void setAmb(glm::fvec4 amb);
	void setDiff(glm::fvec4 diff);
	void setSpec(glm::fvec4 spec);
	void disable();
	void enable();

protected:
	// Atributo para poder generar un nuevo id cada vez
	static GLuint cont;

	// Primer id no válido
	GLuint id;
	// Atributos lumínicos y geométrico de la fuente de luz
	glm::fvec4 ambient = {0.1, 0.1, 0.1, 1};
	glm::fvec4 diffuse = {0.5, 0.5, 0.5, 1};
	glm::fvec4 specular = {0.5, 0.5, 0.5, 1};
	glm::fvec4 posDir = {0, 0, 1, 0};
};

class DirLight : public Light {
public:
	DirLight(glm::fvec3 dir = { 1, 0, 0 });
	virtual ~DirLight() {};
	void upload(glm::dmat4 const& modelViewMat);
	void setPosDir(glm::fvec3 dir);
};

class PosLight : public Light {
public:
	PosLight(glm::fvec3 pos = { 0, 0, 0 });
	virtual ~PosLight() {};
	virtual void upload(glm::dmat4 const& modelViewMat);
	void setPosDir(glm::fvec3 dir);
	void setAtte(GLfloat kc, GLfloat kl, GLfloat kq);
protected:
	// Factores de atenuación 
	GLfloat kc_ = 1, kl_ = 0, kq_ = 0;
};

class SpotLight : public PosLight {
public: 
	SpotLight(glm::fvec3 dir, GLfloat cut, GLfloat ex, glm::fvec3 pos = { 0, 0, 0 });
	virtual void upload(glm::dmat4 const& modelViewMat);
	void setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e);
protected:
	// Atributos del foco
	glm::fvec4 direction = { 0, 0, -1, 0 }; 
	GLfloat cutoff = 180;
	GLfloat exp = 0;
};