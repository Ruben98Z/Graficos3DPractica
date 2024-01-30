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

glm::mat4	model2 = glm::mat4(1.0f);


//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//Por definir

//Shaders
unsigned int vshader;
unsigned int fshader;
unsigned int program;

unsigned int vshader2;
unsigned int fshader2;
unsigned int program2;

//Variables Uniform
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;
int uView;

int uModelViewMat2;
int uModelViewProjMat2;
int uNormalMat2;
int uView2;

//Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;

int inPos2;
int inColor2;
int inNormal2;
int inTexCoord2;



//VAO
unsigned int vao;
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;



//Texturas
unsigned int colorTexId;
unsigned int emiTexId;

unsigned int colorTexId2;
unsigned int emiTexId2;
unsigned int specTexId;
unsigned int normalTexId;


//Filtro anisotropico.
GLfloat fLargest; 



//Texturas Uniform
int uColorTex;
int uEmiTex;

int uColorTex2;
int uEmiTex2;
int uSpecTex;
int uNormalTex;

// Viewport
int w = 500;
int h = 500;

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

//Propiedades de la luz
glm::vec3	lightIntensity = glm::vec3(1.0f);
glm::vec3	lightPos = glm::vec3(0.0, 0.0, 0.0);

int uLightIntensity;
int uLightPos;

int uLightIntensity2;
int uLightPos2;




//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initShader2(const char* vname, const char* fname);
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
	initShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
	initShader2("../shaders_P3/shader.v2.vert", "../shaders_P3/shader.v2.frag");
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
	glutCreateWindow("Prácticas OGL");

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

	//Filtro anisotropico. 
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	}


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


	//Orientación de la cara front, configura la etapa de rasterizado y activa el culling
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	//Matriz de vista y la matriz de proyección
	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -6;

}
void destroy()
{
	//Primera pareja de shaders
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);


	//Segunda pareja de shaders
	glDetachShader(program2, vshader2);
	glDetachShader(program2, fshader2);
	glDeleteShader(vshader2);
	glDeleteShader(fshader2);
	glDeleteProgram(program2);

	
	//glDeleteBuffers(1, &posVBO);
	//glDeleteBuffers(1, &colorVBO);
	//glDeleteBuffers(1, &normalVBO);
	//glDeleteBuffers(1, &texCoordVBO);
	//glDeleteBuffers(1, &triangleIndexVBO);
	glDeleteVertexArrays(1, &vao);

	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);

	glDeleteTextures(1, &colorTexId2);
	glDeleteTextures(1, &emiTexId2);
	glDeleteTextures(1, &specTexId);
	glDeleteTextures(1, &normalTexId);

}
void initShader(const char *vname, const char *fname)
{
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	/*
	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	*/

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

	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");

	uLightIntensity = glGetUniformLocation(program, "lightIntensity");
	uLightPos = glGetUniformLocation(program, "lightPos");
	uView = glGetUniformLocation(program, "view");
	

}

void initShader2(const char* vname, const char* fname) {

	vshader2 = loadShader(vname, GL_VERTEX_SHADER);
	fshader2 = loadShader(fname, GL_FRAGMENT_SHADER);

	program2 = glCreateProgram();
	glAttachShader(program2, vshader2);
	glAttachShader(program2, fshader2);

	/*
	glBindAttribLocation(program2, 0, "inPos");
	glBindAttribLocation(program2, 1, "inColor");
	glBindAttribLocation(program2, 2, "inNormal");
	glBindAttribLocation(program2, 3, "inTexCoord");
	*/

	glLinkProgram(program2);


	GLint linked;
	glGetProgramiv(program2, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program2, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program2, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteProgram(program2);
		program2 = 0;
		exit(-1);
	}

	uNormalMat2 = glGetUniformLocation(program2, "normal");
	uModelViewMat2 = glGetUniformLocation(program2, "modelView");
	uModelViewProjMat2 = glGetUniformLocation(program2, "modelViewProj");

	uColorTex2 = glGetUniformLocation(program2, "colorTex");
	uEmiTex2 = glGetUniformLocation(program2, "emiTex");
	uSpecTex = glGetUniformLocation(program2, "specTex");
	uNormalTex = glGetUniformLocation(program2, "normalTex");


	inPos2 = glGetAttribLocation(program2, "inPos");
	inColor2 = glGetAttribLocation(program2, "inColor");
	inNormal2 = glGetAttribLocation(program2, "inNormal");
	inTexCoord2 = glGetAttribLocation(program2, "inTexCoord");

	uLightIntensity2 = glGetUniformLocation(program2, "lightIntensity");
	uLightPos2 = glGetUniformLocation(program2, "lightPos");
	uView2 = glGetUniformLocation(program2, "view");


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

	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");

	model2 = glm::mat4(1.0f);

	colorTexId2 = loadTex("../img/color2.png");
	emiTexId2 = loadTex("../img/emissive.png");
	specTexId = loadTex("../img/specMap.png");
	normalTexId = loadTex("../img/normal.png");

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
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);

	//Asignacion del codigo del shader
	glShaderSource(shader, 1,
		(const GLchar**)&source, (const GLint*)&fileLen);
	glCompileShader(shader);
	//Liberacion de memoria
	delete[] source;


	//Comprobamos que se compiló bien
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
unsigned int loadTex(const char *fileName){

	//Carga la textura almacenada en el fichero indicado
	unsigned char* map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}

	//Crea una textura, actívala y súbela a la tarjeta gráfica
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (GLvoid*)map);

	//Libera la memoria de la CPU
	delete[] map;

	//Crea los mipmaps asociados a la textura
	glGenerateMipmap(GL_TEXTURE_2D);

	//Configura el modo de acceso
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	glBindTexture(GL_TEXTURE_2D, 0);


	return texId;
}

void renderFunc(){

	//Limpia el buffer de color y el buffer de profundidad antes de cada renderizado
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	//Primer cubo
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	if (uView != -1)
		glUniformMatrix4fv(uView, 1, GL_FALSE,
			&(view[0][0]));
	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
			&(modelView[0][0]));
	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
			&(modelViewProj[0][0]));
	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
			&(normal[0][0]));

	if (uLightIntensity != -1) 
		glUniform3fv(uLightIntensity, 1, &(lightIntensity.x));
	if (uLightPos != -1) 
		glUniform3fv(uLightPos, 1, &(lightPos.x));


	//Texturas primer cubo
	if (uColorTex != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}
	if (uEmiTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}


	//Activo la geometria que voy a pintar
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

	glUseProgram(program2);

	// Segundo cubo
	modelView = view * model2;
	modelViewProj = proj * view * model2;
	normal = glm::transpose(glm::inverse(modelView));
	if(uView2 != -1)
		glUniformMatrix4fv(uView2, 1, GL_FALSE,
			&(view[0][0]));
	if (uModelViewMat2 != -1)
		glUniformMatrix4fv(uModelViewMat2, 1, GL_FALSE,
			&(modelView[0][0]));
	if (uModelViewProjMat2 != -1)
		glUniformMatrix4fv(uModelViewProjMat2, 1, GL_FALSE,
			&(modelViewProj[0][0]));
	if (uNormalMat2 != -1)
		glUniformMatrix4fv(uNormalMat2, 1, GL_FALSE,
			&(normal[0][0]));

	if (uLightIntensity2 != -1)
		glUniform3fv(uLightIntensity2, 1, &(lightIntensity.x));
	if (uLightPos2 != -1)
		glUniform3fv(uLightPos2, 1, &(lightPos.x));



	//Texturas segundo cubo

	if (uColorTex2 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, colorTexId2);
		glUniform1i(uColorTex2, 2);
	}
	if (uEmiTex2 != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, emiTexId2);
		glUniform1i(uEmiTex2, 3);
	}

	if (uSpecTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, specTexId);
		glUniform1i(uSpecTex, 4);
	}
	if (uNormalTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, normalTexId);
		glUniform1i(uNormalTex, 5);
	}

	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

	

	//Cuando termine la renderización hay que cambiar el buffer front por back
	//Llamada sincrona
	glutSwapBuffers();

}

void resizeFunc(int width, int height)
{
	glViewport(0, 0, width, height);

	w = width;
	h = height;

	float aspect = (float)w / (float)h; //Ajusta el aspect ratio al tamaño de la ventana

	proj = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

	glutPostRedisplay();
}
void idleFunc()
{
	model = glm::mat4(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	// Segundo cubo 
	glm::mat4 rotate90y = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0, 0.0, 0.0));
	model2 = rotate90y * translate * rotate90y;

	glutPostRedisplay();

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
	case 'x':
		lightPos = lightPos + glm::vec3(-1.0, 0, 0);
		break;
	case 'X':
		lightPos = lightPos + glm::vec3(1.0, 0, 0);
		break;
	case 'z':
		lightPos = lightPos + glm::vec3(0, 0, -1.0);
		break;
	case 'Z':
		lightPos = lightPos + glm::vec3(0, 0, 1.0);
		break;
	case 'i':
		if(lightIntensity.x > 0)
			lightIntensity = lightIntensity - glm::vec3(0.1, 0.1, 0.1);
		break;
	case 'I':
		if (lightIntensity.x < 1)
			lightIntensity = lightIntensity + glm::vec3(0.1, 0.1, 0.1);
		break;
	default:
		break;
	}


	std::cout << "PosX " << posX << std::endl << std::endl;
	std::cout << "PosZ " << posZ << std::endl << std::endl;
	std::cout << "aX " << aX << std::endl << std::endl;
	std::cout << "aY " << aY << std::endl << std::endl;
	std::cout << "PosX luz " << lightPos.x << std::endl << std::endl;
	std::cout << "PosZ luz " << lightPos.z << std::endl << std::endl;
	std::cout << "Int luz " << lightIntensity.x << std::endl << std::endl;

	// Ángulo de desplazamiento de la camara
	alphaX = glm::radians(aX);

	float x_lookAt = glm::cos(alphaY) * glm::sin(alphaX);
	float y_lookAt = glm::sin(alphaY);
	float z_lookAt = glm::cos(alphaY) * glm::cos(alphaX);

	// Distancia de la camara al objeto
	float dist = distance(glm::vec3(posX, 0, posZ), glm::vec3(0));
	std::cout << "Distancia de la camara al objeto " << dist << std::endl << std::endl;

	// Matriz view
	glm::vec3 pos(posX, 0, posZ);
	lookat = glm::vec3(x_lookAt, y_lookAt, z_lookAt);
	glm::vec3 up(0.0, 1.0, 0.0);
	view = glm::lookAt(pos, pos + lookat, up);

	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y)
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
	glutPostRedisplay();
}










