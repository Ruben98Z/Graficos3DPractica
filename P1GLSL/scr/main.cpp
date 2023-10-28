#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P1/shader.v6.vert", "../shaders_P1/shader.v6.frag"))
		return -1;


	glm::mat4 model = glm::mat4(1.);
	glm::mat4 proj = glm::mat4(0.);
	glm::mat4 view = glm::mat4(1.);
	view[3].z = -6.;

	proj[0].x = 1.73205080;
	proj[1].y = 1.73205080;
	proj[2].z = -1.2;
	proj[2].w = -1.0;
	proj[3].z = -2.2;
   
	//Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
		
	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	//Incluir texturas aquí.
	IGlib::addColorTex(objId, "../img/color.png");


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
	//Ajusta el aspect ratio al tamaño de la venta
}

void idleFunc()
{
	static float angle = 0.0f;
	angle += (angle < 2.0f * 3.141592) ? 0.01 : -angle;
	glm::mat4 model = glm::rotate(glm::mat4(1.0), angle, glm::vec3(1.f, 1.f, 0.f));
	IGlib::setModelMat(objId, model);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state==0)
		std::cout << "Se ha pulsado el botón ";
	else
		std::cout << "Se ha soltado el botón ";
	
	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

void mouseMotionFunc(int x, int y)
{
}
