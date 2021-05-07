//#pragma once
#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>

//-------------------------------------------------------------------------

class Mesh 
{
public:

	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generaPoligono(GLuint numL, GLdouble rd);
	static Mesh* generaSierpinski(GLdouble rd, GLuint numP);
	static Mesh* generaTrianguloRGB(GLdouble rd);
	static Mesh* generaRectangulo(GLdouble w, GLdouble h);
	static Mesh* generaRectanguloRGB(GLdouble w, GLdouble h);

	static Mesh* generaEstrella3D(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generaEstrella3DconTextura(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generaContCubo(GLdouble ld);
	static Mesh* generaFondoCubo(GLdouble ld);
	static Mesh* generaRectanguloConTextura(GLdouble w, GLdouble h,GLuint rw,GLuint rh );

	Mesh() {};
	virtual ~Mesh() {};

	Mesh(const Mesh & m) = delete; // no copy constructor
	Mesh & operator=(const Mesh & m) = delete;  // no copy assignment
			
	virtual void render() const;
	
	GLuint size() const { return mNumVertices; };   // number of elements
	std::vector<glm::dvec3> const& vertices() const { return vVertices; };
	std::vector<glm::dvec4> const& colors() const { return vColors; };
	std::vector<glm::dvec3> const& normals() const { return vNormals; };
		
protected:
	
	GLuint mPrimitive = GL_TRIANGLES;   // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0;  // number of elements ( = vVertices.size())
	std::vector<glm::dvec3> vVertices;  // vertex array
	std::vector<glm::dvec4> vColors;    // color array
	std::vector<glm::dvec2> vTextures;  // texture array
	std::vector<glm::dvec3> vNormals;	// tabla de normales
	virtual void draw() const;
};

//-------------------------------------------------------------------------

class IndexMesh:
	public Mesh {
public:

	static IndexMesh* generaAnilloCuadradoIndexado();
	static IndexMesh* generaCuboConTapasIndexado(GLdouble l);

	IndexMesh() : Mesh() {};
	virtual ~IndexMesh() {  delete[] vIndices;  vIndices = nullptr; };

	virtual void render() const;

	void buildNormalVectors(); //construye las normales
	GLuint* const& indices() const { return vIndices; };
protected:
	GLuint* vIndices;  // index array
	virtual void draw() const;
	GLuint mNumIndex = 0;  // number of element

};

class MbR : public IndexMesh {
public:
	MbR(int muestras, int puntos, glm::dvec3* perfilPtos);
	~MbR() {};
	static MbR* generaIndexMeshByRevolution(int mm, int nn, glm::dvec3* perfil);
protected:
	int n; //número de muestras
	glm::dvec3* perfil; //array de vértices a girar
	int m; //número de puntos del perfil
};

#endif //_H_Scene_H_