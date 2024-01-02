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

float aX = 0.0;
float aY = 0.0;

// Posicion de la camara
float posX = 1;
float posZ = -6;
// Vector look at de la camara
glm::vec3 lookat(0, 0, -1);

//Matriz identidad
glm::mat4 I = glm::mat4(1.0);
// Matriz de proyección
glm::mat4 proj = glm::mat4(0.);
// Matriz view
glm::mat4 view = I;


// Viewport
int w = 500;
int h = 500;

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
	view[3].z = posZ;

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

	// -1.2 * (f-n) = -f-n // -1.2f + 1.2n = -f - n // - 0.2f = -2.2n // -0.2f = -3.806
   
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
	w = width;
	h = height;

	// Matriz de Proyección P (Perspectiva)
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
	glm::mat4 rotateY = glm::rotate(I, angle, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 translate = glm::translate(I, glm::vec3(-3.0, 0.0, 0.0));
	glm::mat4 model2 = rotateY * translate * rotateY;
	IGlib::setModelMat(objId2, model2);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;

	switch (key) {
		case 'a':
			posX = posX + 0.2;
			break;
		case 'w':
			posZ = posZ + 0.2;
			break;
		case 's':
			posZ = posZ - 0.2;
			break;
		case 'd':
			posX = posX - 0.2;
			break;
		case 'y':
			aX = aX + 10;
			break;
		case 'Y':
			aX = aX - 10;
			break;
		default:
			break;
	}

	std::cout << "PosX " << posX << std::endl << std::endl;
	std::cout << "PosZ " << posZ << std::endl << std::endl;
	std::cout << "aX " << aX << std::endl << std::endl;
	std::cout << "aY " << aY << std::endl << std::endl;

	// Ángulo de desplazamiento de la camara
	alphaX = glm::radians(aX);

	float x_lookAt = glm::cos(alphaY) * glm::sin(alphaX);
	float y_lookAt = glm::sin(alphaY);
	float z_lookAt = glm::cos(alphaY) * glm::cos(alphaX);

	// Matriz view
	glm::vec3 pos(posX, 0, posZ);
	lookat = glm::vec3(x_lookAt, y_lookAt, z_lookAt);
	glm::vec3 up(0.0, 1.0, 0.0);
	view = glm::lookAt(pos, pos + lookat, up);

	IGlib::setViewMat(view);
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
	// Posición del raton con respecto el viewport
	int posYMouse = (h / 2) - y;
	int posXMouse = (w / 2) - x;

	// Ángulo de desplazamiento de la camara aplicando una sensibilidad
	aY = posYMouse * 0.3;
	aX = posXMouse * 0.3;


	//Obtener angulos
	alphaY = glm::radians(aY);
	alphaX = glm::radians(aX);

	// Calcular la dirección de la mirada (lookAt)
	float x_lookAt = glm::cos(alphaY) * glm::sin(alphaX);
	float y_lookAt = glm::sin(alphaY);
	float z_lookAt = glm::cos(alphaY) * glm::cos(alphaX);

	// Actualiza la posición y la orientación de la cámara
	glm::vec3 pos = glm::vec3(posX, 0, posZ);
	lookat = glm::vec3(x_lookAt, y_lookAt, z_lookAt);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Actualiza la matriz de vista (view)
	view = glm::lookAt(pos, pos + lookat, up);

	// Actualiza la matriz de vista en IGlib (o donde sea necesario)
	IGlib::setViewMat(view);
}

