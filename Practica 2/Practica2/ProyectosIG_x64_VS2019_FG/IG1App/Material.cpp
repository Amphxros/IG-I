#include "Material.h"
#include <gtc\type_ptr.hpp>

void Material::upload()
{
	glMaterialfv(face, GL_AMBIENT, value_ptr(ambient));
	glMaterialfv(face, GL_DIFFUSE, value_ptr(diffuse));
	glMaterialfv(face, GL_SPECULAR, value_ptr(specular));
	glMaterialf(face, GL_SHININESS, expF);
	glShadeModel(sh);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); // Defecto
}

void Material::setCopper()
{
	ambient = {0.19125, 0.0735, 0.0225, 1};
	diffuse = {0.7038, 0.27048, 0.0828, 1};
	specular = {0.256777, 0.137622, 0.086014, 1};
	expF = 12.8;
}

void Material::setBrass()
{
	ambient = {0.329412, 0.223529, 0.027451, 1};
	diffuse = {0.780392, 0.568627, 0.113725, 1};
	specular = {0.992157, 0.941176, 0.807843, 1};
	expF = 27.8974;
}