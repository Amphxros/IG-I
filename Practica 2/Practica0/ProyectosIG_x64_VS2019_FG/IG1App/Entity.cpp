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
		glEnable(GL_COLOR_MATERIAL);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glDisable(GL_COLOR_MATERIAL);
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
	if (mTexture != nullptr) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gluQuadricTexture(q, GL_TRUE);
		mTexture->bind(GL_REPLACE);
	}
	gluSphere(q, r_, 50, 50);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	if(mTexture!=nullptr){
		glDisable(GL_BLEND);
		gluQuadricTexture(q, GL_FALSE);
		mTexture->unbind();
	}
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
	if (mTexture != nullptr) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gluQuadricTexture(q, GL_TRUE);
		mTexture->bind(GL_REPLACE);
	}
	gluCylinder(q, baseR_, topR_, height_, 50, 50);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	if(mTexture!=nullptr){
		glDisable(GL_BLEND);
		gluQuadricTexture(q, GL_FALSE);
		mTexture->unbind();
	}
	glDisable(GL_COLOR_MATERIAL);
}

Disk::Disk(GLdouble innerR, GLdouble outerR, GLint slices, GLint loops) :
	QuadricEntity(), innerR_(innerR), outerR_(outerR), slices_(slices), loops_(loops) {
}
Disk::Disk(GLdouble innerR, GLdouble outerR) :
	QuadricEntity(), innerR_(innerR), outerR_(outerR), slices_(50), loops_(50) {
}
void Disk::render(glm::dmat4 const& modelViewMat) const {
	glDepthMask(GL_FALSE);

	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// color:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(mColor.r, mColor.g, mColor.b);
	// modo:
	gluQuadricDrawStyle(q, GLU_FILL); // GLU_POINT / GLU_LINE / GLU_FILL
	if (mTexture != nullptr) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gluQuadricTexture(q, GL_TRUE);
		mTexture->bind(GL_REPLACE);
	}
	gluDisk(q, innerR_, outerR_, slices_, loops_);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	if(mTexture!=nullptr){
		glDisable(GL_BLEND);
		gluQuadricTexture(q, GL_FALSE);
		mTexture->unbind();
	}
glDepthMask(GL_TRUE);	
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
	if (mTexture!=nullptr){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gluQuadricTexture(q, GL_TRUE);
		mTexture->bind(GL_REPLACE);
	}
	// modo:
	gluQuadricDrawStyle(q, GLU_FILL); // GLU_POINT / GLU_LINE / GLU_FILL
	gluPartialDisk(q, innerR_, outerR_, 50, 50, startA_, sweepA_);
	// se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	if(mTexture!=nullptr){
		glDisable(GL_BLEND);
		gluQuadricTexture(q, GL_FALSE);
		mTexture->unbind();
	}
	glDisable(GL_COLOR_MATERIAL);
}


AnilloCuadrado::AnilloCuadrado(): Abs_Entity()
{
	mMesh = IndexMesh::generaAnilloCuadradoIndexado();
}

void AnilloCuadrado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);

		glEnable(GL_COLOR_MATERIAL);
		glColor3f(mColor.r, mColor.g, mColor.b);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mMesh->render();

		//default
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_COLOR_MATERIAL);
		glColor3f(0,0,0);
	}
}

Cubo::Cubo(): EntityWithIndexMesh()
{
	mIndexMesh = IndexMesh::generaCuboConTapasIndexado(100);
	static_cast<IndexMesh*>(mIndexMesh)->buildNormalVectors();
}

void Cubo::render(glm::dmat4 const& modelViewMat) const
{
	if (mIndexMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);

		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0, 1, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mIndexMesh->render();

		//default
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_COLOR_MATERIAL);
		glColor3f(1, 1, 1);
	}
}

//-----

void EntityWithIndexMesh::upload(glm::dmat4 const& mModelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(mModelViewMat));  // transfers modelView matrix to the GPU
}

//------------------------------------------------------------------------

void CompoundEntity::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;

	upload(aMat);

	//objetos no translucidos
	for (Abs_Entity* ent : gObjectsCEntity) {
		ent->render(aMat);
	}
	//renderizado objetos translucidos
	for (Abs_Entity* entTrans : gObjectsTranslucidosCEntity) {
		entTrans->render(aMat);
	}
}

void CompoundEntity::freeCEntity()
{
	for (Abs_Entity* ent : gObjectsCEntity) {
		delete ent;
		ent = nullptr;
	}
	gObjectsCEntity.clear();

	for (Abs_Entity* entTrans : gObjectsTranslucidosCEntity) {
		delete entTrans;
		entTrans = nullptr;
	}
	gObjectsTranslucidosCEntity.clear();

	for (auto t : gTextureCEntity) {
		delete t;
		t = nullptr;
	}
}


TIE::TIE():
	CompoundEntity()
{
	//textura que usamos
	Texture* noche = new Texture();
	noche-> load("../BmpsP1/noche.bmp", 255 * 0.75);
	gTextureCEntity.push_back(noche);
	
	// Ala izda.
	Disk* wingL = new Disk(0.0, 150.0, 6, 50);
	wingL->setColor(dvec3(0.0, 0.25, 0.42));
	wingL->setModelMat(glm::translate(wingL->modelMat(), dvec3(400, 300, 200)));
	wingL->setModelMat(glm::rotate(wingL->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	wingL->setModelMat(glm::rotate(wingL->modelMat(), radians(90.0), dvec3(0, 0, 1)));
	wingL->setModelMat(glm::scale(wingL->modelMat(), dvec3(1.75, 1.75, 1.0)));
	wingL->setTexture(noche);
	addEntityTranslucida(wingL);

	// Ala dcha.
	Disk* wingR = new Disk(0.0, 150.0, 6, 50);
	wingR->setColor(dvec3(0.0, 0.25, 0.42));
	wingR->setModelMat(glm::translate(wingR->modelMat(), dvec3(0, 300, 200)));
	wingR->setModelMat(glm::rotate(wingR->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	wingR->setModelMat(glm::rotate(wingR->modelMat(), radians(90.0), dvec3(0, 0, 1)));
	wingR->setModelMat(glm::scale(wingR->modelMat(), dvec3(1.75, 1.75, 1.0)));
	
	wingR->setTexture(noche);
	addEntityTranslucida(wingR);

	// N�cleo
	Sphere* core = new Sphere(100.0);
	core->setColor(dvec3(0.0, 0.25, 0.42));
	core->setModelMat(glm::translate(core->modelMat(), dvec3(200, 300, 200)));
	addEntity(core);

	// Eje
	Cylinder* shaft = new Cylinder(25.0, 25.0, 400.0);
	shaft->setColor(dvec3(0.0, 0.25, 0.42));
	shaft->setModelMat(glm::translate(shaft->modelMat(), dvec3(0, 300, 200)));
	shaft->setModelMat(glm::rotate(shaft->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	addEntity(shaft);

	// Morro
	CompoundEntity* front = new CompoundEntity();

	Cylinder* frontCylinder = new Cylinder(60.0, 60.0, 20.0);
	frontCylinder->setColor(dvec3(0.0, 0.25, 0.42));
	frontCylinder->setModelMat(glm::translate(frontCylinder->modelMat(), dvec3(200, 300, 280)));
	front->addEntity(frontCylinder);

	Disk* frontRing = new Disk(0.0, 60.0);
	frontRing->setColor(dvec3(0.0, 0.25, 0.42));
	frontRing->setModelMat(glm::translate(frontRing->modelMat(), dvec3(200, 300, 300)));
	front->addEntity(frontRing);

	addEntity(front);
}

TIEFormation::TIEFormation()
{
}
