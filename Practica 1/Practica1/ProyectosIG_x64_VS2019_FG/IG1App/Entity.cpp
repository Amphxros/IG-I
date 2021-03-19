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

TrianguloRGB::TrianguloRGB(GLdouble rd): angleC(0), angleR(0)
{
	mMesh = Mesh::generaTrianguloRGB(rd);
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
	glm::dmat4 id = glm::dmat4(1.0);
	angleC += 5;
	angleR -= 5;
	double angleX = 150 * glm::cos(glm::radians(angleC));
	double angleY = 150 * glm::sin(glm::radians(angleC));
	setModelMat(glm::translate(id, dvec3(angleX, angleY, 0)));
	setModelMat(glm::rotate(modelMat(), radians(angleR), dvec3(0, 0, 1)));
}

RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generaRectanguloRGB(w, h);
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

Estrella::Estrella(GLdouble w, GLuint np, GLdouble h, double angle_y, double angle_z): angleY(angle_y), angleZ(angle_z)
{
	mMesh = Mesh::generaEstrella3DconTextura(w, np, h);
}


void Estrella::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);
		if(mTexture!=nullptr){
			mTexture->bind(GL_REPLACE);
		}
		mMesh->render();

		glm::dmat4 bMat = glm::rotate(mModelMat, 3.1416, glm::dvec3(0.0, 1.0, 0.0));
		bMat= modelViewMat*bMat;
		upload(bMat);
		mMesh->render();

		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glLineWidth(1);
		if (mTexture != nullptr) mTexture->unbind();
	}
}

void Estrella::update() /// TODO: PREGUTAR A LA PROFE
{
	setModelMat(glm::translate(dmat4(1.0), dvec3(150, 300, 150)));
	setModelMat(glm::rotate(mModelMat, angleY, dvec3(0.0, 1.0, 0.0)));
	angleY += 0.1;

	setModelMat(glm::rotate(mModelMat, angleZ, dvec3(0.0, 0.0, 1.0)));
	angleZ += 0.1;			 
	

}	

Caja::Caja(GLdouble ld){
	mMesh = Mesh::generaContCubo(ld);
}

void Caja::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		glEnable(GL_CULL_FACE);

		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT, GL_LINES);
		glColor3d(mColor.r, mColor.g, mColor.b);
		glLineWidth(2);
		if (mTexture != nullptr) {
			mTexture->bind(GL_REPLACE);
		}
		mMesh->render();
	
		glCullFace(GL_FRONT);
		glPolygonMode(GL_BACK, GL_LINES);
		glColor3d(mColor.r, mColor.g, mColor.b);
		glLineWidth(2);
		if (mTextureAlt != nullptr) {
			mTextureAlt->bind(GL_REPLACE);
		}
		mMesh->render();

		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glLineWidth(1);
		if(mTexture!=nullptr){
		mTexture->unbind();
		}
		if (mTextureAlt != nullptr) {
			mTextureAlt->unbind();
		}
		glDisable(GL_CULL_FACE);
	}
}

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	mMesh = Mesh::generaRectanguloConTextura(w, h, rw, rh);
	mModelMat = rotate(glm::dmat4(1), glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));
}

void Suelo::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3d(mColor.r, mColor.g, mColor.b);
		glLineWidth(2);

		if (mTexture != nullptr) {
			mTexture->bind(GL_MODULATE);
		}

		mMesh->render();

		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glLineWidth(1);
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
	}
}
