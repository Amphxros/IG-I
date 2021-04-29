#include "Entity.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include "IG1App.h"
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

Estrella::Estrella(GLdouble w, GLuint np, GLdouble h, double angle_y, double angle_z): 
	angleY(angle_y), angleZ(angle_z)
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

void Estrella::update()
{
	setModelMat(glm::translate(dmat4(1.0), dvec3(-150, 200, -150)));
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

CajaFondo::CajaFondo(GLdouble ld): mModelMatFondo(1.0) {
	mMesh = Mesh::generaContCubo(ld);
	meshFondo = Mesh::generaFondoCubo(ld);

	mModelMat = translate(dmat4(1.0), dvec3(-150, 0, -150));
	
	mModelMatFondo = rotate(glm::dmat4(1.0), glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));
	mModelMatFondo = translate(mModelMatFondo, dvec3(-150,-150,-ld/2));
}

void CajaFondo::render(glm::dmat4 const& modelViewMat) const
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
		
		dmat4 aMatFondo = modelViewMat * mModelMatFondo;  // glm matrix multiplication
		upload(aMatFondo);

		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT, GL_LINES);
		glColor3d(mColor.r, mColor.g, mColor.b);
		glLineWidth(2);
		if (mTexture != nullptr) {
			mTexture->bind(GL_REPLACE);
		}
		meshFondo->render();

		glCullFace(GL_FRONT);
		glPolygonMode(GL_BACK, GL_LINES);
		glColor3d(mColor.r, mColor.g, mColor.b);
		glLineWidth(2);
		if (mTextureAlt != nullptr) {
			mTextureAlt->bind(GL_REPLACE);
		}
		meshFondo->render();

		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glLineWidth(1);
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		if (mTextureAlt != nullptr) {
			mTextureAlt->unbind();
		}
		glDisable(GL_CULL_FACE);
	}
}

void CajaFondo::update()
{
	setModelMat(glm::rotate(modelMat(), radians(1.0), dvec3(0.0, 0.0, 1.0)));

	mModelMatFondo = rotate(mModelMatFondo, glm::radians(1.0), glm::dvec3(0.0, 1.0, 0.0));
}

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	mMesh = Mesh::generaRectanguloConTextura(w, h, rw, rh);
	mModelMat = rotate(glm::dmat4(1), glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));
}

Suelo::Suelo(GLdouble w, GLdouble h) {
	mMesh = Mesh::generaRectanguloConTextura(w, h, (int)(w/10), (int)(h/10));
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

Foto::Foto(GLdouble w, GLdouble h) {
	mMesh = Mesh::generaRectanguloConTextura(w, h, 1,1);
	
	mModelMat = glm::translate(glm::dmat4(1),dvec3(0, 1, 0));
	mModelMat = rotate(modelMat(), glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));
	mModelMat = rotate(modelMat(), glm::radians(180.0), glm::dvec3(0.0, 0.0, 1.0));
}

void Foto::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3d(mColor.r, mColor.g, mColor.b);
		glLineWidth(2);

		if (mTexture != nullptr) {
			mTexture->bind(GL_REPLACE);
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

void Foto::update()
{
	mTexture->loadColorBuffer(IG1App::s_ig1app.getWidth(), IG1App::s_ig1app.getHeight(), GL_FRONT);
}

Cristalera::Cristalera(GLdouble ld)
{
	mMesh = Mesh::generaContCubo(ld);

	setModelMat(glm::translate(dmat4(1.0),dvec3(0.0,ld/4,0.0)));
	setModelMat(glm::scale(modelMat(), dvec3(1.0, 0.5, 1.0)));
}

void Cristalera::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);

		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_FRONT, GL_LINES);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLineWidth(2);
		if (mTexture != nullptr) {
			mTexture->bind(GL_REPLACE);
		}
		mMesh->render();

		//default
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glLineWidth(1);
		if(mTexture!=nullptr){
		mTexture->unbind();
		}
	}
}

Planta::Planta(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generaRectanguloConTextura(w, h, 1, 1);
	setModelMat(glm::translate(dmat4(1.0), dvec3(w, h/4, w)));

	setModelMat(glm::scale(modelMat(), dvec3(0.5, 0.5, 0.5)));

}

void Planta::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0);


		glColor3d(mColor.r, mColor.g, mColor.b);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		if (mTexture != nullptr) {
			mTexture->bind(GL_REPLACE);
		}

		mMesh->render();

		glm::dmat4 bMat = glm::rotate(mModelMat, glm::radians(60.0), glm::dvec3(0.0, 1.0, 0.0));
		bMat = modelViewMat * bMat;
		upload(bMat);
		mMesh->render();
		
		glm::dmat4 cMat = glm::rotate(mModelMat, glm::radians(120.0), glm::dvec3(0.0, 1.0, 0.0));
		cMat = modelViewMat * cMat;
		upload(cMat);
		mMesh->render();


		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glLineWidth(1);
		if (mTexture != nullptr) {
			mTexture->unbind();
		}
		glDisable(GL_ALPHA_TEST);
	}
}

QuadricEntity::QuadricEntity():Abs_Entity() {
	q = gluNewQuadric();
}

Sphere::Sphere(GLdouble r) :
	QuadricEntity(), r_(r) {
}
void Sphere::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// color:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// modo:
	gluQuadricDrawStyle(q, GLU_FILL); // GLU_POINT / GLU_LINE / GLU_FILL
	gluSphere(q, r_, 50, 50);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

Cylinder::Cylinder(GLdouble baseR, GLdouble topR, GLdouble height) :
	QuadricEntity(), baseR_(baseR), topR_(topR), height_(height) {
}
void Cylinder::render(glm::dmat4 const& modelViewMat) const {
	glm::dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// color:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// modo:
	gluQuadricDrawStyle(q, GLU_FILL); // GLU_POINT / GLU_LINE / GLU_FILL
	gluCylinder(q, baseR_, topR_, height_, 50, 50);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

Disk::Disk(GLdouble innerR, GLdouble outerR, GLint slices, GLint loops) :
	QuadricEntity(), innerR_(innerR), outerR_(outerR), slices_(slices), loops_(loops) {
}
Disk::Disk(GLdouble innerR, GLdouble outerR) :
	QuadricEntity(), innerR_(innerR), outerR_(outerR), slices_(50), loops_(50) {
}
void Disk::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// color:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// modo:
	gluQuadricDrawStyle(q, GLU_FILL); // GLU_POINT / GLU_LINE / GLU_FILL
	gluDisk(q, innerR_, outerR_, slices_, loops_);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

PartialDisk::PartialDisk(GLdouble innerR, GLdouble outerR, GLdouble startA, GLdouble sweepA) :
	QuadricEntity(), innerR_(innerR), outerR_(outerR), startA_(startA), sweepA_(sweepA) {
}
void PartialDisk::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// color:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// modo:
	gluQuadricDrawStyle(q, GLU_FILL); // GLU_POINT / GLU_LINE / GLU_FILL
	gluPartialDisk(q, innerR_, outerR_, 50, 50, startA_, sweepA_);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

void TIE::render(glm::dmat4 const& modelViewMat) const {
	for (Abs_Entity* ent : gObjectsCEntity) {
		ent->render(modelViewMat);
	}
}