#include "Light.h"
#include <gtc\type_ptr.hpp>

GLuint Light::cont = 0;

Light::Light(): id(0)
{
	id = GL_LIGHT0 + GL_MAX_LIGHTS;
	if (cont < GL_MAX_LIGHTS) {
		id = GL_LIGHT0 + cont;
		++cont;
		//glEnable(id); //recomendado por antonio
	}
}

void Light::uploadL() { 
// Transfiere las características de la luz a la GPU
glLightfv(id, GL_AMBIENT, value_ptr(ambient));
glLightfv(id, GL_DIFFUSE, value_ptr(diffuse));
glLightfv(id, GL_SPECULAR, value_ptr(specular));
}

void Light::disable()
{
	if (id < GL_LIGHT0 + GL_MAX_LIGHTS) { 
		glDisable(id);
	}
}

void Light::enable()
{
	if (id < GL_LIGHT0 + GL_MAX_LIGHTS) {
		glEnable(id);
	}
}

void Light::setAmb(glm::fvec4 amb) {
	ambient = amb;
	uploadL();
};

void Light::setDiff(glm::fvec4 diff){
	diffuse = diff; 
	uploadL();
};

void Light::setSpec(glm::fvec4 spec){
	specular = spec;
	uploadL();
};

////////////////////////////////////////////////

DirLight::DirLight(glm::fvec3 dir) : Light() {
	setPosDir(dir);
}
void DirLight::upload(glm::dmat4 const& modelViewMat) {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));
	glLightfv(id, GL_POSITION, value_ptr(posDir));
	uploadL();
}
// Ojo al 0.0 que determina que la luz sea remota
void DirLight::setPosDir(glm::fvec3 dir) {
	posDir = glm::fvec4(dir, 0.0);
}

PosLight::PosLight(glm::fvec3 pos) : Light() {
	setPosDir(pos);
}
void PosLight::upload(glm::dmat4 const& modelViewMat) {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));
	glLightfv(id, GL_POSITION, value_ptr(posDir));
	glLightf(id, GL_CONSTANT_ATTENUATION, kc_);
	glLightf(id, GL_LINEAR_ATTENUATION, kl_);
	glLightf(id, GL_QUADRATIC_ATTENUATION, kq_);
	uploadL();
}
void PosLight::setAtte(GLfloat kc, GLfloat kl, GLfloat kq) {
	kc_ = kc;
	kl_ = kl;
	kq_ = kq;
}
// Ojo al 1.0 que determina que la luz sea local
void PosLight::setPosDir(glm::fvec3 dir) {
	posDir = glm::fvec4(dir, 1.0);
}

SpotLight::SpotLight(glm::fvec3 dir, GLfloat cut, GLfloat ex, glm::fvec3 pos) : PosLight(pos) {
	setSpot(dir, cut, ex);
}
void SpotLight::upload(glm::dmat4 const& modelViewMat) {
	PosLight::upload(modelViewMat);
	glLightfv(id, GL_SPOT_DIRECTION, value_ptr(direction));
	glLightf(id, GL_SPOT_CUTOFF, cutoff);
	glLightf(id, GL_SPOT_EXPONENT, exp);
}
// Ojo al 0.0: la dirección de emisión del foco es vector
void SpotLight::setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e) {
	direction = glm::fvec4(dir, 0.0);
	cutoff = cf;
	exp = e;
}