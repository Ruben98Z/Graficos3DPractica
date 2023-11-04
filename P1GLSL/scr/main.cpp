#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;
int objId2 = -1;
// Estado del botton izquierdo del raton
bool leftPressed = false;
// Angulo de desplazamiento camara
float alphaX = 0;
float alphaY = 0;
//Matriz identidad
glm::mat4 I = glm::mat4(1.0);
// Matriz de proyección
glm::mat4 proj = glm::mat4(0.);
// Viewport
float h = 500;
float w = 500;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P1/shader.v7.vert", "../shaders_P1/shader.v7.frag"))
		return -1;


	glm::mat4 model = I;
	glm::mat4 view = I;
	view[3].z = -6.;

	// Se inicia la matriz de proyeccion
	// ( 1.73	0		0		0		)
	// ( 0		1.73	0		0		)
	// ( 0		0		-1.2	-2.2	)
	// ( 0		0		-1.0	0		)
	proj[0].x = 1.73205080;
	proj[1].y = 1.73205080;
	proj[2].z = -1.2;
	proj[2].w = -1.0;
	proj[3].z = -2.2;
   
	//Se ajusta la cámara
	// Matriz de Proyección P (Perspectiva)
	//Si no se da valor se cojen valores por defecto
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);

	//Creamos segundo objeto a visualizar
	objId2 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);
		
	glm::mat4 modelMat = I;

	IGlib::setModelMat(objId, modelMat);
	IGlib::setModelMat(objId2, modelMat);
	//Incluir texturas aquí.
	IGlib::addColorTex(objId, "../img/color.png");
	IGlib::addColorTex(objId2, "../img/blanco-negro.png");


  //CBs
  IGlib::setResizeCB(resizeFunc);
  IGlib::setIdleCB(idleFunc);
  IGlib::setKeyboardCB(keyboardFunc);
  IGlib::setMouseCB(mouseFunc);
  IGlib::setMouseMoveCB(mouseMotionFunc);

	
	//Mainloop
	IGlib::mainLoop();
	IGlib::destroy();
	return 0;
}

void resizeFunc(int width, int height)
{
	//Ajusta el aspect ratio al tamaño de la ventana

	float aspect = (float)width / (float)height;
	float right = aspect;
	float left = -aspect;
	float top = 1.0;
	float bottom = -1.0;
	float near = 1.73;

	proj[0].x = (2 * near) / (right - left); //Apertura horizontal	
 
	std::cout << "Reajuste de la matriz de proyeccion, aspect = " << aspect << std::endl << std::endl;

	IGlib::setProjMat(proj);
	
}

void idleFunc()
{
	// Primer cubo
	static float angle = 0.0f;
	angle += (angle < 2.0f * 3.141592) ? 0.01 : -angle;
	glm::mat4 model = glm::rotate(I, angle, glm::vec3(1.f, 1.f, 0.f));
	IGlib::setModelMat(objId, model);

	// Segundo cubo 
	glm::mat4 rotate90y = glm::rotate(I, angle, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 translate = glm::translate(I, glm::vec3(-3.0, 0.0, 0.0));
	glm::mat4 model2 = rotate90y * translate * rotate90y;
	IGlib::setModelMat(objId2, model2);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state==0){
		std::cout << "Se ha pulsado el botón ";
		leftPressed = true;
	}
	else {
		leftPressed = false;
		std::cout << "Se ha soltado el botón ";
	}
		
	
	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

void mouseMotionFunc(int x, int y)
{
	
}
