#include "Entity.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const 
{ 
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l): Abs_Entity()
{
  mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

EjesRGB::~EjesRGB() 
{ 
	delete mMesh; mMesh = nullptr; 
};
//-------------------------------------------------------------------------

void EjesRGB::render(dmat4 const& modelViewMat) const 
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}
//---------------------------------------------------------------------

Poligono::Poligono(GLdouble numL, GLdouble rd)
{
	mMesh = Mesh::generaPoligono(numL, rd);
}

Poligono::~Poligono()
{
	delete mMesh; mMesh = nullptr;
}

void Poligono::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);

		mMesh->render();

		//default
		glLineWidth(1);
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
}


TrianguloSierpinsky::TrianguloSierpinsky(GLdouble rd,GLdouble numP)
{
	mMesh= Mesh::generaSierpinski(rd, numP);
}

TrianguloSierpinsky::~TrianguloSierpinsky()
{
	delete mMesh; mMesh = nullptr;
}

void TrianguloSierpinsky::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPointSize(2);
		glColor3d(mColor.r, mColor.g, mColor.b);

		mMesh->render();

		//default
		glPointSize(1);
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
}

TrianguloRGB::TrianguloRGB(GLdouble rd)
{
	mMesh = Mesh::generaTrianguloRGB(rd);
}

TrianguloRGB::~TrianguloRGB()
{
	delete mMesh;
}

void TrianguloRGB::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();

		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
	}
}

void TrianguloRGB::update()
{
	setModelMat(glm::translate(modelMat(), dvec3(10, 0, 0)));
	setModelMat(glm::rotate(modelMat(), radians(-25.0), dvec3(0, 0, 1)));
}

RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generaRectanguloRGB(w, h);
}

RectanguloRGB::~RectanguloRGB()
{
	delete mMesh; mMesh = nullptr;

}

void RectanguloRGB::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_POINT);
		
		glPolygonMode(GL_FRONT, GL_LINES);
		
		mMesh->render();

		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		
	}
}
