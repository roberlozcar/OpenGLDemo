#include "BOX.h"
//#include "Tetraedro.h"  //OPCIONAL 3d: OTRO MODELO
#include "auxiliar.h"
#include "PLANE.h"

#include <GL/glew.h>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cstdlib>

#define RAND_SEED 31415926			// semilla para generar numeros aleatorios
#define SCREEN_SIZE 500,500

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);

//Variables para el movimiento de la cámara
float camSpeed = 0.1;
float camAngle = 0.0;
glm::mat4 rot = glm::mat4(1.0);
glm::vec3 pivot = glm::vec3(0.0, 0.0, -25.0);

float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
bool mouseDown = false;

float WIDTH = 0.0f;
float HEIGHT = 0.0f;

//para controlar el blending por teclado (tecla H)
bool enableBlending;

//Uniform para modificar por teclado el tipo de luz (puntual, direccional, focal)
int typeOfLight = 0;
int uTypeOfLight = -1;
int uTypeOfLight2  = -1;

// Controlar deferred shading
bool enableDeferred = false;

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
float angle = 0.0f;     // para rotar el modelo
float f = 50.0f;
float n = 1.0f;
float offset = 0.0f;

//Utilizado para la parte opcional de rotacion de un cubo mediante curva de Bezier
glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);

//VAO
unsigned int vao;

//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int tangentVBO;
unsigned int triangleIndexVBO;

//Texturas
unsigned int colorTexId;
unsigned int emiTexId;
unsigned int specularTexId;
unsigned int normalTexId;

//Shaders y programs
unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform 
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;

//Texturas Uniform
int uColorTex;
int uEmiTex;
int uSpecularTex;
int uNormalTex;

//Dof por teclado
int uMaxDistFact;
int uFocalDist;

float maxDistanceFactor = 1.0 / 5.0;
float focalDistance = -25.0;

//motion blur por teclado
float motionBlurAlpha = 0.6f;

//mascaras de convolución con variables uniform
const int MASK_SIZE = 25;
const glm::vec2 texIdx[MASK_SIZE] = {
glm::vec2(-2.0,2.0), glm::vec2(-1.0,2.0), glm::vec2(0.0,2.0), glm::vec2(1.0,2.0), glm::vec2(2.0,2.0),
glm::vec2(-2.0,1.0), glm::vec2(-1.0,1.0), glm::vec2(0.0,1.0), glm::vec2(1.0,1.0), glm::vec2(2.0,1.0),
glm::vec2(-2.0,0.0), glm::vec2(-1.0,0.0), glm::vec2(0.0,0.0), glm::vec2(1.0,0.0), glm::vec2(2.0,0.0),
glm::vec2(-2.0,-1.0), glm::vec2(-1.0,-1.0), glm::vec2(0.0,-1.0), glm::vec2(1.0,-1.0), glm::vec2(2.0,-1.0),
glm::vec2(-2.0,-2.0), glm::vec2(-1.0,-2.0), glm::vec2(0.0,-2.0), glm::vec2(1.0,-2.0), glm::vec2(2.0,-2.0) };

//Gaussian blur
const float maskFactor = float(1.0 / 65.0);
const float mask[MASK_SIZE] = {
1.0 * maskFactor, 2.0 * maskFactor, 3.0 * maskFactor, 2.0 * maskFactor, 1.0 * maskFactor,
2.0 * maskFactor, 3.0 * maskFactor, 4.0 * maskFactor, 3.0 * maskFactor, 2.0 * maskFactor,
3.0 * maskFactor, 4.0 * maskFactor, 5.0 * maskFactor, 4.0 * maskFactor, 3.0 * maskFactor,
2.0 * maskFactor, 3.0 * maskFactor, 4.0 * maskFactor, 3.0 * maskFactor, 2.0 * maskFactor,
1.0 * maskFactor, 2.0 * maskFactor, 3.0 * maskFactor, 2.0 * maskFactor, 1.0 * maskFactor };

//Edge detection
const float maskFactor2 = float(1.0);
const float mask2[MASK_SIZE] = {
1.0 * maskFactor2, 2.0 * maskFactor2, 3.0 * maskFactor2, 2.0 * maskFactor2, 1.0 * maskFactor2,
2.0 * maskFactor2, 3.0 * maskFactor2, 4.0 * maskFactor2, 3.0 * maskFactor2, 2.0 * maskFactor2,
3.0 * maskFactor2, 4.0 * maskFactor2, -60.0 * maskFactor2, 4.0 * maskFactor2, 3.0 * maskFactor2,
2.0 * maskFactor2, 3.0 * maskFactor2, 4.0 * maskFactor2, 3.0 * maskFactor2, 2.0 * maskFactor2,
1.0 * maskFactor2, 2.0 * maskFactor2, 3.0 * maskFactor2, 2.0 * maskFactor2, 1.0 * maskFactor2 };

//unsharp borders
const float maskFactor3 = float(-1.0 / 256.0);
const float mask3[MASK_SIZE] = {
1.0 * maskFactor3, 4.0 * maskFactor3, 6.0 * maskFactor3, 4.0 * maskFactor3, 1.0 * maskFactor3,
4.0 * maskFactor3, 16.0 * maskFactor3, 24.0 * maskFactor3, 16.0 * maskFactor3, 4.0 * maskFactor3,
6.0 * maskFactor3, 24.0 * maskFactor3, -476.0 * maskFactor3, 24.0 * maskFactor3, 6.0 * maskFactor3,
4.0 * maskFactor3, 16.0 * maskFactor3, 24.0 * maskFactor3, 16.0 * maskFactor3, 4.0 * maskFactor3,
1.0 * maskFactor3, 4.0 * maskFactor3, 6.0 * maskFactor3, 4.0 * maskFactor3, 1.0 * maskFactor3 };

int uTexIdx;
int uMask;

int useConvMask = 1;

//Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;
int inTangent;


//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int, int);

void renderCube();

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShaderFw(const char *vname, const char *fname);
void initObj();
void destroy();


//Carga el shader indicado, devuele el ID del shader
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
unsigned int loadTex(const char *fileName);

//////////////////////////////////////////////////////////////
// Variables auxiliares
//////////////////////////////////////////////////////////////

// Shaders de postProceso y Program
unsigned int postProccesVShader;
unsigned int postProccesFShader;
unsigned int postProccesProgram;

//Uniform
unsigned int uColorTexPP;
unsigned int uPosTexPP;
unsigned int uDepthTexPP;

// Plano
unsigned int planeVAO;
unsigned int planeVertexVBO;

// Frame Buffer Object
unsigned int fbo;
unsigned int colorBuffTexId;
unsigned int depthBuffTexId;
unsigned int uNormTexPP;
unsigned int uEmiTexPP;

// Nuevo objeto textura
unsigned int posBufferTexId;
unsigned int normBufferTexId;
unsigned int emiBufferTexId;

//
unsigned int uf, un, uoff; // far y near y offset

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////

void initPlane();												// inicializa el plano
void initFBO();													// Crea el FBO
void resizeFBO(unsigned int w, unsigned int h);					// cuando se redimensiona la ventana tambien lo hace el FBO
void initShaderPP(const char* vname, const char* fname);		// inicia shader de postproccess
glm::vec3 Bezier(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float t);

int main(int argc, char** argv)
{
	//std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();

	// PONER A TRUE CUANDO SE USE DEFERRED SHADING Y A FALSE CUANDO NO SE UTILICE
	enableDeferred = true;

	//Primera pasada
	initShaderFw("../shaders/fwRendering.v3.vert", "../shaders/fwRendering.v3.frag");

	// Iniciamos el shader de PostProcessing
	initShaderPP("../shaders/postProcessing.v6.vert", "../shaders/postProcessing.v6.frag");


	initObj();
	initPlane();

	// FBO
	initFBO();
	resizeFBO(SCREEN_SIZE);
	
	glutMainLoop();

	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Demo OpenGL");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	proj = glm::perspective(glm::radians(60.0f), 1.0f, n, f);
	view = glm::mat4(1.0f);
	view[3].z = -25.0f;
}

void destroy()
{
	// LIBERAMOS RECURSOS
	// Shaders y program
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);

	// Shaders y program de postproces
	glDetachShader(postProccesProgram, postProccesVShader);
	glDetachShader(postProccesProgram, postProccesFShader);
	glDeleteShader(postProccesVShader);
	glDeleteShader(postProccesFShader);
	glDeleteProgram(postProccesProgram);

	if (inPos != -1) glDeleteBuffers(1, &posVBO);
	if (inColor != -1) glDeleteBuffers(1, &colorVBO);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBO);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBO);
	if (inTangent != -1) glDeleteBuffers(1, &tangentVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	
	// Plano
	glDeleteBuffers(1, &planeVertexVBO);
	glDeleteVertexArrays(1, &planeVAO);

	// VAO
	glDeleteVertexArrays(1, &vao);

	// Texturas
	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);

	// FBO y sus texturas
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &colorBuffTexId);
	glDeleteTextures(1, &depthBuffTexId);
	glDeleteTextures(1, &posBufferTexId);
	glDeleteTextures(1, &normBufferTexId);
	glDeleteTextures(1, &emiBufferTexId);
}

void initShaderFw(const char *vname, const char *fname)
{
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	glBindAttribLocation(program, 4, "inTangent");


	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;

		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}

	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
	uSpecularTex = glGetUniformLocation(program, "specularTex");
	uNormalTex = glGetUniformLocation(program, "normalTex");

	uTypeOfLight = glGetUniformLocation(program, "typeOfLight");

	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");
	inTangent = glGetAttribLocation(program, "inTangent");
}

void initObj()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (inPos != -1)
	{
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}

	if (inColor != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}

	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}


	if (inTexCoord != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}

	if (inTangent != -1)
	{
		glGenBuffers(1, &tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
			cubeVertexTangent, GL_STATIC_DRAW);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex*sizeof(unsigned int) * 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

	model = glm::mat4(1.0f);

	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");
	specularTexId = loadTex("../img/specMap.png");
	normalTexId = loadTex("../img/normal.png");
}

GLuint loadShader(const char *fileName, GLenum type)
{
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);

	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1,
		(const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete[] source;

	//Comprobamos que se compilo bien
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;

		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

unsigned int loadTex(const char *fileName)
{
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);

	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (GLvoid*)map);
	delete[] map;
	glGenerateMipmap(GL_TEXTURE_2D);

	// Filtro anisotrópico
	GLfloat fLargest;
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  //GL_CLAMP (bordes negros)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	return texId;
}

void renderFunc()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);			//debe hacerse antes de limpiar
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Activamos el culling y la profundidad
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glUseProgram(program);

	//Texturas
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

	if (uSpecularTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, specularTexId);
		glUniform1i(uSpecularTex, 2);
	}

	if (uNormalTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, normalTexId);
		glUniform1i(uNormalTex, 3);
	}

	// Tipo de luz (puntual, direccional, focal)
	if (uTypeOfLight != -1)
	{
		glUniform1i(uTypeOfLight, typeOfLight);
	}

	model = glm::mat4(1.0f);
	model[3].w = 1.0f;
	glm::mat4 trans = glm::translate(glm::mat4(1.0), point);
	model = trans * model;
	renderCube();

	model = glm::mat4(2.0f);
	model[3].w = 1.0f;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));
	renderCube();

	std::srand(RAND_SEED);
	for (unsigned int i = 0; i < 10; i++)
	{
		float size = float(std::rand() % 3 + 1);

		glm::vec3 axis(glm::vec3(float(std::rand() % 2),
			float(std::rand() % 2), float(std::rand() % 2)));
		if (glm::all(glm::equal(axis, glm::vec3(0.0f))))
			axis = glm::vec3(1.0f);

		float trans = float(std::rand() % 7 + 3) * 1.00f + 0.5f;
		glm::vec3 transVec = axis * trans;
		transVec.x *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.y *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.z *= (std::rand() % 2) ? 1.0f : -1.0f;

		model = glm::rotate(glm::mat4(1.0f), angle*2.0f*size, axis);
		model = glm::translate(model, transVec);
		model = glm::rotate(model, angle*2.0f*size, axis);
		model = glm::scale(model, glm::vec3(1.0f / (size*0.7f)));
		renderCube();
	}
	
	// Activamos el Default Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Desactivamos el culling y el depth
	// porque no hay triangulos encima de otros
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	// Activamos el blending
	// combinacion de un pixel en un momento dado y un fragmento
	if (enableBlending) {
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}

	glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
	glBlendColor(1.0f - motionBlurAlpha, 1.0f - motionBlurAlpha, 1.0f - motionBlurAlpha, motionBlurAlpha);						
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Por que factores multiplica origen y destino en la siguiente operacion
	glBlendEquation(GL_FUNC_ADD);								// Hace la suma con las multiplicaciones indicadas anteriormente
	

	// Activamos el programa de postprocess
	glUseProgram(postProccesProgram);

	// Activo el VAO con la configuracion del plano
	glBindVertexArray(planeVAO);

	// Le indicamos que textura tiene que utilizar
	if (uColorTexPP != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, colorBuffTexId);
		glUniform1i(uColorTexPP, 1);
	}

	if (uPosTexPP != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, posBufferTexId);
		glUniform1i(uPosTexPP, 0);
	}

	if (uNormTexPP != -1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normBufferTexId);
		glUniform1i(uNormTexPP, 2);
	}

	if (uEmiTexPP != -1)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, emiBufferTexId);
		glUniform1i(uEmiTexPP, 3);
	}

	if (uTypeOfLight2 != -1)
	{
		glUniform1i(uTypeOfLight2, typeOfLight);
	}

	// DOF POR TECLADO
	if (uMaxDistFact != -1)
	{
		glUniform1f(uMaxDistFact, maxDistanceFactor);
	}

	if (uFocalDist != -1)
	{
		glUniform1f(uFocalDist, focalDistance);
	}

	//mascaras de convolucion uniform
	if (uTexIdx != -1)
	{
		glUniform2fv(uTexIdx, MASK_SIZE, &(texIdx[0].x));
	}

	if (uMask != -1)
	{
		if (useConvMask == 1)
			glUniform1fv(uMask, MASK_SIZE, &(mask[0]));
		if (useConvMask == 2)
			glUniform1fv(uMask, MASK_SIZE, &(mask2[0]));
		if (useConvMask == 3)
			glUniform1fv(uMask, MASK_SIZE, &(mask3[0]));
	}

	// Depthbuffer
	if (uDepthTexPP != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
		glUniform1i(uDepthTexPP, 4);
	}

	if (uf != -1)
		glUniform1f(un, n);

	if (uf != -1)
		glUniform1f(uf, f);

	if (uf != -1)
		glUniform1f(uoff, offset);

	// Dibujamos 
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glutSwapBuffers();
}

void renderCube()
{
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
	
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);
}

void resizeFunc(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;

	glViewport(0, 0, width, height);
	proj = glm::perspective(glm::radians(60.0f), float(width) /float(height), 1.0f, 50.0f);

	// Tambien queremos que cambie el FBO
	resizeFBO(width, height);

	glutPostRedisplay();
}

void idleFunc()
{
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.02f;

	// Calculamos el offset para distorsion en PostProcess
	offset = angle;

	// Definicion de la curva de beier para la parte opcional
	glm::vec3 p1(9.0, 9.0, 0.0), p2(0.0, 0.0, 9.0), p3(-9.0, -9.0, 0.0), p4(0.0, 0.0, -9.0);
	static float t = 0;
	t = (t > 1 ? 0 : t + 0.01);
	point = Bezier(p1, p2, p3, p4, t);
	
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	static float angle_0 = 0.175;
	static glm::mat4 Rot = glm::rotate(glm::mat4(1.0), angle_0, glm::vec3(0.0, 1.0, 0.0));

	switch (key)
	{
	case 'w': case 'W':
		view[3].z += camSpeed;
		pivot.z -= camSpeed;
		break;
	case 'a': case 'A':
		view[3].x += camSpeed;
		pivot.x -= camSpeed;
		break;
	case 's': case 'S':
		view[3].z -= camSpeed;
		pivot.z += camSpeed;
		break;
	case 'd': case 'D':
		view[3].x -= camSpeed;
		pivot.x += camSpeed;
		break;
	case 'q': case 'Q':
		view = glm::transpose(Rot) * view;
		pivot = glm::mat3(rot) * pivot;
		break;
	case 'e': case 'E':
		view = Rot * view;
		pivot = glm::transpose(glm::mat3(rot)) * pivot;
		break;
	// CONTROL DEL DOF POR TECLADO
	case '+':
		focalDistance += 1.0;
		break;
	case '-':
		focalDistance -= 1.0;
		break;
	case 'm': case 'M':
		maxDistanceFactor += 0.05;
		break;
	case 'l': case 'L':
		maxDistanceFactor -= 0.05;
		break;
	//CONTROL MOTION BLUR POR TECLADO
	case 'b': case 'B':
		motionBlurAlpha = (motionBlurAlpha >= 0.9f) ? 0.9f : motionBlurAlpha + 0.1f;
		break;
	case 'v': case 'V':
		motionBlurAlpha = (motionBlurAlpha <= 0.0f) ? 0.0f : motionBlurAlpha - 0.1f;
		break;
	//CAMBIO DE MASCARA
	case 'c': case 'C':
		useConvMask = (useConvMask == 3) ? 1 : useConvMask + 1;
		break;
	case 'h': case 'H':
		enableBlending = !enableBlending;
		break;
	case 't': case 'T':
		typeOfLight = (typeOfLight == 2) ? 0 : typeOfLight + 1;
		break;
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
		xrot = 0;
		yrot = 0;

		xdiff = x;
		ydiff = y;
	}
	else {
		mouseDown = false;
	}
}

void mouseMotionFunc(int x, int y)
{
	if (mouseDown)
	{
		xrot = x - xdiff;
		yrot = y - ydiff;

		std::cout << x << " " << y << std::endl;
		std::cout << xdiff << " " << ydiff << std::endl;
		std::cout << xrot << " " << yrot << std::endl;

		glm::mat4 rot(1.0);
		glm::mat4 tras(1.0);
		glm::mat4 tras2(1.0);

		tras = glm::translate(glm::mat4(1.0), -pivot);
		tras2 = glm::translate(glm::mat4(1.0), pivot);
		rot = glm::rotate(rot, xrot / (HEIGHT * 10), glm::vec3(0.0, 1.0, 0.0));
		rot = glm::rotate(rot, yrot / (WIDTH * 10), glm::vec3(1.0, 0.0, 0.0));
		view = tras2 * rot * tras * view;

		glutPostRedisplay();
	}
}

void initShaderPP(const char* vname, const char* fname) {

	// Declaramos los shaders
	postProccesVShader = loadShader(vname, GL_VERTEX_SHADER);
	postProccesFShader = loadShader(fname, GL_FRAGMENT_SHADER);

	// Enlazamos los shader al program
	postProccesProgram = glCreateProgram();

	glAttachShader(postProccesProgram, postProccesVShader);
	glAttachShader(postProccesProgram, postProccesFShader);
	glBindAttribLocation(postProccesProgram, 0, "inPos");
	glLinkProgram(postProccesProgram);

	// Comprobamos que se haya enlazado bien
	int linked;
	glGetProgramiv(postProccesProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(postProccesProgram, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(postProccesProgram, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteProgram(postProccesProgram);
		postProccesProgram = 0;
		exit(-1);
	}

	// Obtenemos el location de la textura
	
	if (enableDeferred) {
		uPosTexPP = glGetUniformLocation(postProccesProgram, "PosTex");
		uColorTexPP = glGetUniformLocation(postProccesProgram, "DiffTex");

		uTypeOfLight2 = glGetUniformLocation(postProccesProgram, "typeOfLight");
	}
	else {
		uPosTexPP = glGetUniformLocation(postProccesProgram, "posTex");
		uColorTexPP = glGetUniformLocation(postProccesProgram, "colorTex");
	}
	uNormTexPP = glGetUniformLocation(postProccesProgram, "NormalTex");
	uEmiTexPP = glGetUniformLocation(postProccesProgram, "EmiTex");

	// DOF POR TECLADO
	uMaxDistFact = glGetUniformLocation(postProccesProgram, "maxDistanceFactor");
	uFocalDist = glGetUniformLocation(postProccesProgram, "focalDistance");

	//mascaras de convolución uniform
	uTexIdx = glGetUniformLocation(postProccesProgram, "texIdx");
	uMask = glGetUniformLocation(postProccesProgram, "mask");

	// Comprobamos si esta bien el location de la posicion
	if (0 != glGetAttribLocation(postProccesProgram, "inPos"))
	{
		std::cerr << "ERROR!!!" << std::endl;
		exit(-1);
	}

	// Location  depth
	uDepthTexPP = glGetUniformLocation(postProccesProgram, "depthTex");
	uf = glGetUniformLocation(postProccesProgram, "f");
	un = glGetUniformLocation(postProccesProgram, "n");
	uoff = glGetUniformLocation(postProccesProgram, "offset");
}

void initPlane() 
{
	// Cramos el ID del VAO y lo activamos para configurarlo
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);

	// Creamos un VBO para guardar los vertices y lo activamos para configurarlo
	glGenBuffers(1, &planeVertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexVBO);
	// Subir el buffer
	glBufferData(GL_ARRAY_BUFFER, planeNVertex * sizeof(float) * 2, planeVertexPos, GL_STATIC_DRAW);
	
	// Lo enlazamos a la variable inPos (location = 0)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void initFBO() 
{
	// Crea los Identificadores de la textura de profundidad 
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &colorBuffTexId);
}

void resizeFBO(unsigned int w, unsigned int h)
{
	// Activamos la textura de color
	// por cada texele tendra 4 canales de color de 8bytes
	// solo utilizamos el nivel 0
	// w , h numero de texteles de ancho y alto
	glBindTexture(GL_TEXTURE_2D, colorBuffTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_FLOAT, NULL); // NULL sin subir dato

	// Configuramos el modo de acceso (TEXTURA DE COLOR)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	


	// Activamos el FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//Le asignamos las texturas de color y profundidad (Y EL NUEVO OBJETO)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffTexId, 0);		// 0 nivel de textura
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, posBufferTexId, 0); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffTexId, 0);		// 0 nivel de textura
	
	if (enableDeferred)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
			GL_TEXTURE_2D, normBufferTexId, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
			GL_TEXTURE_2D, emiBufferTexId, 0);

		const GLenum buffs[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, buffs);
	}
	else {
		// ATTACHMENT0 se corresponde con la salida 0 del fwrendering 
		const GLenum buffs[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_DEPTH_ATTACHMENT};
		glDrawBuffers(3, buffs);
	}

	// Comprobamos que va a funcionar
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		std::cerr << "Error configurando el FBO" << std::endl;
		exit(-1);
	}

	// Activamos el Default Frame Buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


glm::vec3 Bezier(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float t) {

	float base = (1.0f - t);
	glm::vec3 point = p1 * (pow(base, 4.0f) + pow(t, 4.0f)) + p2 * 4.0f * t * pow(base, 3.0f) + 6.0f * p3 * pow(t, 2.0f) * pow(base, 2.0f) + p4 * 4.0f * pow(t, 3.0f) * base;
	return point;
}