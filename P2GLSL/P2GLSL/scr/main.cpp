#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId = -1;
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
	if (!IGlib::init("../shaders_P2/shader.v7.vert", "../shaders_P2/shader.v7.frag"))
		return -1;
  //Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto
	glm::mat4 view = glm::mat4(1.0);
	view[3].z = -6;

	glm::mat4 proj = glm::mat4(1.0);
	float f = 1.0f / tan(3.141592f / 6.0f);
	float far = 10.0f;
	float near = 0.1f;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near) / (near - far);
	proj[2].w = -1.0f;
	proj[3].z = (2.0f * far * near) / (near - far);
	proj[3].w = 0.0f;
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	IGlib::addColorTex(objId, "../img/color2.png");
	IGlib::addSpecularTex(objId, "../img/specMap.png");
	IGlib::addEmissiveTex(objId, "../img/emissive.png");
		
	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	
	//CBs
	IGlib::setIdleCB(idleFunc);
	IGlib::setResizeCB(resizeFunc);
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
	float nplane = 1.73;
	float fplane = 19.03;
	float aspect = (float)w / (float)h; //Ajusta el aspect ratio al tamaño de la ventana
	glm::mat4 P = glm::perspective(glm::radians(60.f), aspect, nplane, fplane);

	std::cout << "Reajuste de la matriz de proyeccion, aspect = " << aspect << std::endl << std::endl;

	proj = P;

	IGlib::setProjMat(proj);

}

void idleFunc()
{
	glm::mat4 modelMat(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	
	modelMat = glm::rotate(modelMat, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	//IGlib::setModelMat(objId, modelMat);
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

	// Distancia de la camara al objeto
	float dist = distance(glm::vec3(posX, 0, posZ),glm::vec3(0));
	std::cout << "Distancia de la camara al objeto " << dist << std::endl << std::endl;

	// Matriz view
	glm::vec3 pos(posX, 0, posZ);
	lookat = glm::vec3(x_lookAt, y_lookAt, z_lookAt);
	glm::vec3 up(0.0, 1.0, 0.0);
	view = glm::lookAt(pos, pos + lookat, up);

	IGlib::setViewMat(view);
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
	// Posición del raton con respecto el viewport
	int posYMouse = (h / 2) - y;
	int posXMouse = (w / 2) - x;

	// Ángulo de desplazamiento de la camara aplicando una sensibilidad
	aY = posYMouse * 0.3;
	aX = posXMouse * 0.3;


	// Limitar los ángulos para evitar giros bruscos
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
