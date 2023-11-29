#include "BOX.h"
#include "auxiliar.h"


#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);


//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//Por definir

//Shaders
unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;
//Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;



//VAO
unsigned int vao;
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;






// Viewport
int w = 500;
int h = 500;



//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaraci�n de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicializaci�n y destrucci�n
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initObj1();
void initObj2();
void destroy();


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	initShader("../shaders_P3/shader.v0.vert", "../shaders_P3/shader.v0.frag");
	initObj2();

	glutMainLoop();

	destroy();

	return 0;
}
	
//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//Frame buffer
	//Contexto
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pr�cticas OGL");

	//Extensiones
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;


	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);

}
void initOGL(){

	//Activa el test de profundidad y establece el color de fondo
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);


	//Orientaci�n de la cara front, configura la etapa de rasterizado y activa el culling
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	//Matriz de vista y la matriz de proyecci�n
	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -6;

}
void destroy()
{
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);

	
	//glDeleteBuffers(1, &posVBO);
	//glDeleteBuffers(1, &colorVBO);
	//glDeleteBuffers(1, &normalVBO);
	//glDeleteBuffers(1, &texCoordVBO);
	//glDeleteBuffers(1, &triangleIndexVBO);
	glDeleteVertexArrays(1, &vao);
}
void initShader(const char *vname, const char *fname)
{
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");

}
void initObj1()
{
	///////////////////////////////
	//Creacion buffers
	unsigned int buff[5];
	glGenBuffers(5, buff);

	posVBO = buff[0];
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	//Reserva de memoria grafica
	glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
		cubeVertexPos, GL_STATIC_DRAW);
	
	colorVBO = buff[1];
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	//Reserva espacio
	glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
		cubeVertexColor, GL_STATIC_DRAW);
	//Subida de informacion
	glBufferSubData(GL_ARRAY_BUFFER, 0, cubeNVertex * sizeof(float) * 3, ::cubeVertexColor);
	
	normalVBO = buff[2];
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
		cubeVertexNormal, GL_STATIC_DRAW);
	
	texCoordVBO = buff[3];
	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2,
		cubeVertexTexCoord, GL_STATIC_DRAW);


	triangleIndexVBO = buff[4];
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

	///////////////////////////////////////////
	//Config buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glVertexAttribPointer(0, //Attr location
		3, // Posicion es un vector de 3 componentes (Num componentes)
		GL_FLOAT, // Tipo de cada componentes
		GL_FALSE, // Si lo meto dentro del componente lo voy a normalizar
		0, // Stride
		0); // Offset -> Donde empieza

	//Activar atributo
	glEnableVertexAttribArray(0); //Attr location


	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(1); //Attr location

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); //Attr location

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); //Attr location

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);


	model = glm::mat4(1.0f);

}

void initObj2()
{
	unsigned int buff;
	glGenBuffers(1, &buff);

	unsigned int sizeOfIndex = cubeNTriangleIndex * sizeof(unsigned int) * 3;
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	//Reserva de memoria grafica
	glBufferData(GL_ARRAY_BUFFER,
		sizeOfIndex +
		cubeNVertex * sizeof(float) * 11, //(3+3+3+2)
		NULL, GL_STATIC_DRAW);

	//No pueden tener todos el mismo offset, se tiene que empezar desde donde termina el ultimo
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfIndex, cubeTriangleIndex);
	glBufferSubData(GL_ARRAY_BUFFER, sizeOfIndex,
		cubeNVertex * sizeof(float) * 3, cubeVertexPos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeOfIndex + cubeNVertex * sizeof(float) * 3,
		cubeNVertex * sizeof(float) * 3, cubeVertexColor);
	glBufferSubData(GL_ARRAY_BUFFER, sizeOfIndex + cubeNVertex * sizeof(float) * 6,
		cubeNVertex * sizeof(float) * 3, cubeVertexNormal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeOfIndex + cubeNVertex * sizeof(float) * 9,
		cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord);

	///////////////////////////////////////////
	//Config buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0); //Attr location
	glEnableVertexAttribArray(1); //Attr location
	glEnableVertexAttribArray(2); //Attr location
	glEnableVertexAttribArray(3); //Attr location

	glBindBuffer(GL_ARRAY_BUFFER, buff);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) sizeOfIndex);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeOfIndex + cubeNVertex * sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeOfIndex + cubeNVertex * sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeOfIndex + cubeNVertex * sizeof(float) * 9));


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff);

	model = glm::mat4(1.0f);
}

//
// 	   GLenum type -> tipo de shader (vertices o fragmentos)
//
GLuint loadShader(const char *fileName, GLenum type)
{
	//Mala practica de programacion debido a la reserva de memoria
	unsigned int fileLen;
	char* source = loadStringFromFile(fileName, fileLen);

	//////////////////////////////////////////////
	//Creaci�n y compilaci�n del Shader
	GLuint shader;
	shader = glCreateShader(type);

	//Asignacion del codigo del shader
	glShaderSource(shader, 1,
		(const GLchar**)&source, (const GLint*)&fileLen);
	glCompileShader(shader);
	//Liberacion de memoria
	delete[] source;


	//Comprobamos que se compil� bien
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		//Reservar espacio de memoria para la cadena de error
		char* logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		//Liberacion de memoria
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader; 
}
unsigned int loadTex(const char *fileName){ return 0; }

void renderFunc(){

	//Limpia el buffer de color y el buffer de profundidad antes de cada renderizado
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
			&(modelView[0][0]));
	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
			&(modelViewProj[0][0]));
	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
			&(normal[0][0]));

	//Activo la geometria que voy a pintar
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);



	//Cuando termine la renderizaci�n hay que cambiar el buffer front por back
	//Llamada sincrona
	glutSwapBuffers();

}
void resizeFunc(int width, int height)
{
	glViewport(0, 0, width, height);

	w = width;
	h = height;

	float aspect = (float)w / (float)h; //Ajusta el aspect ratio al tama�o de la ventana

	proj = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

	glutPostRedisplay();
}
void idleFunc()
{
	model = glm::mat4(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	glutPostRedisplay();

}
void keyboardFunc(unsigned char key, int x, int y){}
void mouseFunc(int button, int state, int x, int y){}









