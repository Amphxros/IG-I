#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
#include<glm.hpp>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const 
{
  glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const 
{
  if (vVertices.size() > 0) {  // transfer data
    // transfer the coordinates of the vertices
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 
    
	if (vColors.size() > 0) { // transfer colors
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
    }
	if (vTextures.size() > 0) {
		// activar array de coordenadas de textura
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_DOUBLE, 0, vTextures.data());
	}
	if (vNormals.size() > 0) {
		// se añaden comandos para la tabla de normales:
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_DOUBLE, 0, vNormals.data());
	}

	draw();

	//Desactivamos TODO
    glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
  }
}
//-------------------------------------------------------------------------

Mesh * Mesh::createRGBAxes(GLdouble l)
{
  Mesh* mesh = new Mesh();

  mesh->mPrimitive = GL_LINES;

  mesh->mNumVertices = 6;
  mesh->vVertices.reserve(mesh->mNumVertices);

  // X axis vertices
  mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
  mesh->vVertices.emplace_back(l, 0.0, 0.0);
  // Y axis vertices
  mesh->vVertices.emplace_back(0, 0.0, 0.0);
  mesh->vVertices.emplace_back(0.0, l, 0.0);
  // Z axis vertices
  mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
  mesh->vVertices.emplace_back(0.0, 0.0, l);

  mesh->vColors.reserve(mesh->mNumVertices);
  // X axis color: red  (Alpha = 1 : fully opaque)
  mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
  mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
  // Y axis color: green
  mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
  mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
  // Z axis color: blue
  mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
  mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
 
  return mesh;
}

Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = numL;
	mesh->vVertices.reserve(mesh->mNumVertices);
	
	glm::dvec2 centro = { 0.0, 0.0 };
	double ang= 3.1416 / 2; //angulo inicial 

	//bucle que calcula n puntos 
	for(int i=0; i < mesh->mNumVertices; i++)
	{		
		double x,y;
		ang += (2 * 3.1416)/  numL ;
		x = centro.x + rd * cos(ang);
		y = centro.y + rd * sin(ang);
		
		mesh->vVertices.emplace_back(x, y, 0.0);
	}
	
	return mesh;
}

Mesh* Mesh::generaSierpinski(GLdouble rd, GLuint numP)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_POINTS;
	mesh->mNumVertices = numP;
	mesh->vVertices.reserve(mesh->mNumVertices);

	Mesh * triangulo = generaPoligono(3, rd); //triangulo de inicio

	//pasamos los vertices del triangulo a nuestra mesh
	for (int i = 0; i < triangulo->mNumVertices; i++)
	{
		mesh->vVertices.emplace_back(triangulo->vVertices[i]);
	}
	//calculamos el resto de puntos
	for (int i = 2; i < mesh->mNumVertices; i++)
	{
		glm::dvec3 pA = mesh->vVertices[i]; //punto A desde donde empezamos a calcular
		glm::dvec3 pB = mesh->vVertices[rand() % 3]; //punto B random
	
		glm::dvec3 pM; //punto medio
		pM.x= (pA.x + pB.x )/2;
		pM.y = (pA.y + pB.y) / 2;
		pM.z = (pA.z + pB.z) / 2;

		mesh->vVertices.emplace_back(pM );

	}
	
	//borramos el triangulo porque ya no nos interesa
	delete triangulo;
	triangulo = nullptr;

	return mesh;
}

Mesh* Mesh::generaTrianguloRGB(GLdouble rd)
{
	Mesh * mesh = generaPoligono(3, rd); //generamos un triangulo
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->vColors.reserve(mesh->mNumVertices);
		
	// vector de colores
	mesh->vColors.emplace_back(1.0,0.0,0.0,1.0);
	mesh->vColors.emplace_back(0.0,1.0,0.0,1.0);
	mesh->vColors.emplace_back(0.0,0.0,1.0,1.0);

	return mesh; 
}

Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h)
{
	//w/2 y h/2 para que este centrado
	GLdouble w_ = w / 2;
	GLdouble h_ = h / 2;
	
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);

	//como la primitiva es triangle strip tenemos que dar los vertices en diferente orden
	mesh->vVertices.emplace_back(-w_, h_, 0.0);
	mesh->vVertices.emplace_back(-w_, -h_, 0.0);
	mesh->vVertices.emplace_back(w_, h_, 0.0);
	mesh->vVertices.emplace_back(w_, -h_, 0.0);

	return mesh;
}

Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h)
{
	Mesh* mesh = generaRectangulo(w, h);
	mesh->vColors.reserve(mesh->mNumVertices);

	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh* Mesh::generaRectanguloConTextura(GLdouble w,GLdouble h,GLuint rw,GLuint rh ){
	Mesh* mesh = generaRectangulo(w,h);
	mesh->vTextures.reserve(mesh->mNumVertices);

	mesh->vTextures.push_back(dvec2(0,rh));
	mesh->vTextures.push_back(dvec2(0,0));
	mesh->vTextures.push_back(dvec2(rw,rh));
	mesh->vTextures.push_back(dvec2(rw,0));

	return mesh;
}

Mesh* Mesh::generaContCubo(GLdouble ld)
{
	Mesh* mesh= new Mesh();
	
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;

	mesh->vVertices.reserve(mesh->mNumVertices);
	//mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vTextures.reserve(mesh->mNumVertices);

	GLdouble l=ld/2;

	//1º cara (Z positiva)
	mesh->vVertices.push_back(dvec3(-l,l,l)); //V0
	mesh->vTextures.push_back(dvec2(0.0, 1.0));
	mesh->vVertices.push_back(dvec3(-l,-l,l)); //V1
	mesh->vTextures.push_back(dvec2(0.0, 0.0));
	mesh->vVertices.push_back(dvec3(l,l,l)); //V2
	mesh->vTextures.push_back(dvec2(1.0, 1.0));
	mesh->vVertices.push_back(dvec3(l,-l,l)); //V3
	mesh->vTextures.push_back(dvec2(1.0, 0.0));

	//2º cara (Z negativa)
	mesh->vVertices.push_back(dvec3(l,l,-l)); //V4
	mesh->vTextures.push_back(dvec2(2.0, 1.0));
	mesh->vVertices.push_back(dvec3(l,-l,-l)); //V5
	mesh->vTextures.push_back(dvec2(2.0, 0.0));
	mesh->vVertices.push_back(dvec3(-l,l,-l)); //V6
	mesh->vTextures.push_back(dvec2(3.0, 1.0));
	mesh->vVertices.push_back(dvec3(-l,-l,-l)); //V7
	mesh->vTextures.push_back(dvec2(3.0, 0.0));

	//luego enlazamos con los del principio con 2 vertices mas
	mesh->vVertices.push_back(dvec3(-l,l,l)); //V0
	mesh->vTextures.push_back(dvec2(4.0, 1.0));
	mesh->vVertices.push_back(dvec3(-l,-l,l)); //V1
	mesh->vTextures.push_back(dvec2(4.0, 0.0));

	return mesh;
}

Mesh* Mesh::generaFondoCubo(GLdouble ld)
{
	Mesh* mesh = new Mesh;

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;

	mesh->vVertices.reserve(mesh->mNumVertices);
	//mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vTextures.reserve(mesh->mNumVertices);

	GLdouble l = ld / 2;

	mesh->vVertices.push_back(dvec3(-l, l, l)); //V0
	mesh->vTextures.push_back(dvec2(0.0, 1.0));
	mesh->vVertices.push_back(dvec3(-l, -l, l)); //V1
	mesh->vTextures.push_back(dvec2(0.0, 0.0));
	mesh->vVertices.push_back(dvec3(l, l, l)); //V2
	mesh->vTextures.push_back(dvec2(1.0, 1.0));
	mesh->vVertices.push_back(dvec3(l, -l, l)); //V3
	mesh->vTextures.push_back(dvec2(1.0, 0.0));

	return mesh;
}

Mesh* Mesh::generaEstrella3D(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 2* (np +1);

	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.push_back(dvec3(0,0,0));
	

	GLdouble ri = re / 2; //ri= radio interior
	double ang= 3.1416 / 2; //angulo inicial

 
	for(int i=0; i<mesh->mNumVertices-2; i++)
	{
		double x,y;
		ang += (2 * 3.1416)/(mesh->mNumVertices-2);
		if(i%2==0){ // circunferencia grande
			x = re * cos(ang);
			y = re * sin(ang);
		}
		else{ // circunferencia pequeña
			x = ri * cos(ang);
			y = ri * sin(ang);
			

		}
		mesh->vVertices.push_back(dvec3(x, y, h));
			
	}
	mesh->vVertices.push_back(mesh->vVertices[1]);
	
	return mesh;
}

Mesh* Mesh::generaEstrella3DconTextura(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 2* (np +1);

	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vTextures.reserve(mesh->mNumVertices);

	mesh->vColors.push_back(dvec4(1.0, 1.0, 0.0, 1.0));
	mesh->vVertices.push_back(dvec3(0,0,0));
	mesh->vTextures.push_back(dvec2(0.5, 0.5));

	GLdouble ri = re / 2; //ri= radio interior
	double ang= 3.1416 / 2; //angulo inicial

	double r_texture=0.5; // radio para la textura
 
	for(int i=0; i<mesh->mNumVertices-2; i++){
		double x,y;
		ang += (2 * 3.1416)/(mesh->mNumVertices-2);
		if(i%2==0){ // circunferencia grande
			x = re * cos(ang);
			y = re * sin(ang);
			r_texture = 0.5;
		}
		else{ // circunferencia pequeña
			x = ri * cos(ang);
			y = ri * sin(ang);
			r_texture=0.25;

		}
		double x_tex = 0.5 + r_texture * cos(ang);
		double y_tex = 0.5 + r_texture * sin(ang);

		mesh->vVertices.push_back(dvec3(x, y, h));
		mesh->vColors.push_back(dvec4(1.0, 1.0, 0.0, 1.0));
		mesh->vTextures.push_back(dvec2(x_tex, y_tex));
	}
	mesh->vVertices.push_back(mesh->vVertices[1]);
	mesh->vColors.push_back(dvec4(1.0, 1.0, 0.0, 1.0));
	mesh->vTextures.push_back(mesh->vTextures[1]);

	return mesh;
}

 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IndexMesh* IndexMesh::generaAnilloCuadradoIndexado()
{
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 8;
	mesh->mNumIndex = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->vVertices = {
		{30.0, 30.0, 0.0}, {10.0, 10.0, 0.0}, {70.0, 30.0, 0.0}, {90.0, 10.0, 0.0},
		{70.0, 70.0, 0.0}, {90.0, 90.0, 0.0}, {30.0, 70.0, 0.0}, {10.0, 90.0, 0.0}
	};

	mesh->vColors = {
		{0.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
		{1.0, 1.0, 0.0, 1.0}, {1.0, 0.0, 1.0, 1.0}, {0.0, 1.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0}
	};

	mesh->vIndices = new GLuint[mesh->mNumIndex]{0, 1, 2, 3, 4, 5, 6, 7, 0, 1};
	

	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vNormals.push_back(dvec3(0.0, 0.0, 1.0));
	}

	return mesh;
}

IndexMesh* IndexMesh::generaCuboConTapasIndexado(GLdouble l)
{
	IndexMesh* mesh = new IndexMesh();

	mesh->mPrimitive = GL_TRIANGLES;
	mesh->mNumVertices = 8;
	mesh->mNumIndex = 36;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vColors.reserve(mesh->mNumVertices);

	GLdouble ld = l / 2;
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->vVertices = {
		{ld, ld, ld}, {ld, -ld, ld}, {ld, ld, -ld}, {ld, -ld, -ld},
		{-ld, ld, -ld}, {-ld, -ld, -ld}, {-ld, ld, ld}, {-ld, -ld, ld}

		/*{ld, ld, -ld}, {ld, -ld, -ld}, {ld, ld, ld}, {ld, -ld, ld},
		{-ld, ld, ld}, {-ld, -ld, ld}, {-ld, ld, -ld}, {-ld, -ld, -ld}*/
	};

	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vColors.push_back(dvec4(0.0, 1.0, 0.0, 1.0));
	}

	mesh->vIndices = new GLuint[mesh->mNumIndex]
	{
	1,3,2, 2,0,1,													
	3,5,4, 4,2,3,												
	5,7,6, 6,4,5, 												
	7,1,0, 0,6,7, 												
	6,0,2, 2,4,6, 												
	5,3,1, 1,7,5
	
	/*0,1,2, 2,1,3,													
	2,3,4, 4,3,5,												
	4,5,6, 6,5,7, 												
	6,7,0, 0,7,1, 												
	4,6,2, 2,6,0, 												
	1,7,3, 3,7,5*/
	};

	//		/!\ ESTÁN MAL /!\
	//mesh->vNormals = {
	//	glm::normalize(dvec3(1, 1, -1)),
	//	glm::normalize(dvec3(1, -1, -1)),
	//	glm::normalize(dvec3(1, 1, 1)),
	//	glm::normalize(dvec3(1, -1, 1)),
	//	glm::normalize(dvec3(-1, 1, 1)),
	//	glm::normalize(dvec3(-1, -1, 1)),
	//	glm::normalize(dvec3(-1, 1, -1)),
	//	glm::normalize(dvec3(-1, -1, -1))
	//};
	
	return mesh;
}

IndexMesh* IndexMesh::generateGrid(GLdouble lado, GLuint nDiv)
{
	// Grid cuadrado de lado*lado, centrado en el plano Y=0, dividido en nDiv*nDiv celdas (cada celda son 2 triángulos)
	IndexMesh* mesh = new IndexMesh();
	GLdouble x = -lado / 2;
	GLdouble z = -lado / 2;
	GLdouble incr = lado/ nDiv; // incremento para la coordenada x, y la c. z
	GLuint numFC = nDiv + 1; // número de vértices por filas y columnas
	// Generación de vértices
	mesh->mNumVertices = numFC * numFC; 
	mesh->vVertices.reserve(mesh->mNumVertices);

	for (int i = 0; i < numFC; i++) {
		for (int j = 0; j < numFC; j++) {
			mesh->vVertices.push_back(dvec3(x + i * incr, 0, z + j * incr));
		}
	}

	// Generación de índices
	mesh->mNumIndex = nDiv * nDiv * 6; 
	mesh->vIndices = new GLuint[mesh->mNumIndex];
	int cont = 0;
	
	for (int i = 0; i < nDiv; i++) {
		for (int j = 0; j < nDiv; j++) {
			int iv = i* numFC + j;
			
			/*
			iv, iv+nFC, iv+1;
			iv+1, iv+nFC, iv+nFC+1;			
			*/

			//triangulo1
			mesh->vIndices[cont] = iv;
			cont++;
			mesh->vIndices[cont] = iv + numFC;
			cont++;
			mesh->vIndices[cont] = iv + 1;
			cont++;

			//triangulo2
			mesh->vIndices[cont] = iv + 1;
			cont++;
			mesh->vIndices[cont] = iv + numFC;
			cont++;
			mesh->vIndices[cont] = iv + 1+ numFC;
			cont++;

		}
	}

	return mesh;

}
IndexMesh* IndexMesh::generateGridTex(GLdouble lado, GLuint nDiv) 
{
	// Grid cuadrado de lado*lado, centrado en el plano Y=0, dividido en nDiv*nDiv celdas
	IndexMesh* mesh = generateGrid(lado, nDiv); 
	// número de vértices por filas y columnas
	GLuint numFC = nDiv + 1;
	// Generación de las coordenadas de textura
	mesh->vTextures.reserve(mesh->mNumVertices);
	double s = 0;
	double t = 1;
	for (int i = 0; i < numFC; i++) {
		GLdouble indX = (double)i /(numFC-1);
		for (int j = 0; j < numFC; j++) {
			GLdouble indY = (double)j / (numFC-1);
			mesh->vTextures.push_back( dvec2(s+indX,t-indY));
		}
	}

	return mesh; 
}

void IndexMesh::render() const
{
	if (vVertices.size() > 0) {  // transfer data
	  // transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 

		if (vColors.size() > 0) { // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}
		if (vTextures.size() > 0) {
			// activar array de coordenadas de textura
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTextures.data());
		}
		if (vIndices!=nullptr) {
			glEnableClientState(GL_INDEX_ARRAY);
			glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
		}
		if (vNormals.size() > 0) {
		// se añaden comandos para la tabla de normales:
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}

		draw();

		//Desactivamos TODO
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}

void IndexMesh::buildNormalVectors()
{
	for (int i = 0; i < mNumVertices; i++) {
		vNormals.emplace_back(glm::dvec3(0, 0, 0));
	}

	for (int i = 0; i < mNumIndex; i+=3) {
		
		GLuint auxA, auxB, auxC; //puntos a, b y c que forman 1 cara

		auxA = vIndices[i];
		auxB = vIndices[i + 1];
		auxC = vIndices[i + 2];

		//calculamos la normal cruzando los vertices de dichos puntos
		dvec3 n = cross((vVertices[auxB] - vVertices[auxA]), (vVertices[auxC] - vVertices[auxA]));

		//le sumamos a cada uno su normal 
		vNormals[auxA] += n; 
		vNormals[auxB] += n;
		vNormals[auxC] += n;
	}
	for (int i = 0; i < mNumVertices; i++) {
		vNormals[i] = normalize(vNormals[i]);
	}

}

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, mNumIndex, GL_UNSIGNED_INT,vIndices);
}

//---------------------------------------------------

MbR::MbR(int muestras, int puntos, dvec3* perfilPtos) : 
	IndexMesh(), n(muestras), m(puntos), perfil(perfilPtos)
{
	// Definir el número de vértices como nn*mm
	mNumVertices = n * m;
}

MbR* MbR::generaIndexMeshByRevolution(int mm, int nn, glm::dvec3* perfil)
{
	MbR* mesh = new MbR(mm, nn, perfil);
	// Definir la primitiva como GL_TRIANGLES
	// Usar un vector auxiliar de vértices
	dvec3* vertices = new dvec3[mesh->mNumVertices];
	for (int i = 0; i < nn; i++) {
		// Generar la muestra i-ésima de vértices
		GLdouble theta = i * 360 / nn;
		GLdouble c = cos(radians(theta));
		GLdouble s = sin(radians(theta));
		// R_y(theta) es la matriz de rotación alrededor del eje Y
		for (int j = 0; j < mm; j++) {
			int indice = i * mm + j;
			GLdouble x = c * perfil[j].x + s * perfil[j].z;
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			vertices[indice] = dvec3(x, perfil[j].y, z);
		}
	}
	// Volcar el array auxiliar vertices que acabamos de usar, en mesh->vVertices de la malla
	mesh->vVertices.reserve(mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; i++){
		mesh->vVertices.emplace_back(vertices[i]);
	}

	mesh->mNumIndex = (mm - 1) * nn * 6;
	// Índices caras cuadrangulares:
	mesh->vIndices = new GLuint[mesh->mNumIndex];
	// El contador indice sirve para llevar cuenta de los índices generados hasta ahora.
	int indMayor = 0;
	for (int i = 0; i < nn; i++) {
	// Se recorre las muestras alrededor del eje Y
		//Las caras cuadrangulares resultan al unir la muestra i-ésima con la (i+1)%nn-ésima
		// El contador j recorre los vértices del perfil, de abajo arriba.
		for (int j = 0; j < mm - 1; j++) {
			// Se recorre la cara desde la esquina inferior izquierda 
			int indice = i * mm + j;
			//triangulo1
			mesh->vIndices[indMayor] = indice;
			indMayor++;
			mesh->vIndices[indMayor] = (indice + mm) % (nn * mm);
			indMayor++;
			mesh->vIndices[indMayor] = (indice + mm + 1) % (nn * mm);
			indMayor++;
			//triangulo2
			mesh->vIndices[indMayor] = (indice + mm + 1) % (nn * mm);
			indMayor++;
			mesh->vIndices[indMayor] = (indice +1);
			indMayor++;
			mesh->vIndices[indMayor] = indice;
			indMayor++;
		}
		//// Los cuatro índices son entonces:
		//// * indice
		//// * (indice + mm) % (nn * mm)
		//// * (indice + mm + 1) % (nn * mm)
		//// * indice + 1
		//------------------------------------------
		// abajo: indice, (indice + mm) % (nn * mm), (indice + mm + 1) % (nn * mm)
		// arriba: (indice + mm + 1) % (nn * mm), indice + 1, indice
		// | - - / |
		// | - / - |
		// | / - - |
	}
	
	mesh->buildNormalVectors();
	return mesh;
}