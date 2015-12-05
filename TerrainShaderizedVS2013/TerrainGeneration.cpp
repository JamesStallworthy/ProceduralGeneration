#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 

#include "getbmp.h"

#include <vector>

using namespace std;

using namespace glm;

// Size of the terrain
const int MAP_SIZE = 2049;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

const int SEED = 1213;

const float START_RAND_AMOUNT = 100;
const float RAND_AMOUNT = 100;

//Smooth
const float iterationAmount = 1.9;

float terrain[MAP_SIZE][MAP_SIZE] = {};

static const vec4 globAmb = vec4(0.9, 0.9, 0.9, 1.0);

struct Vertex
{
	float coords[4];
	float normals[3];
	float texCoords[2];
};

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

struct Matrix4x4
{
	float entries[16];
};

static mat4 projMat = mat4(1.0);

static const Matrix4x4 IDENTITY_MATRIX4x4 =
{
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	}
};

static enum buffer { SQUARE_VERTICES };
static enum object { SQUARE };

// Globals
static Vertex terrainVertices[MAP_SIZE*MAP_SIZE] = {};

const int numStripsRequired = MAP_SIZE - 1;
const int verticesPerStrip = 2 * MAP_SIZE;

unsigned int terrainIndexData[numStripsRequired][verticesPerStrip];

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
buffer[1],
vao[1],
normalMatLoc,
texture[1],
grassTexLoc;

//Camera
const float SPEED = 10;
const float ROTSPEED = 2;
float cameraTheta = 0;
float cameraPhi = 0;
vec3 cameraPos(0, 0, 100);
//Line of sight
vec3 LOS(0, 0, -1);

static const Material terrainFandB =
{
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 0.0)
};mat4 modelViewMat = mat4(1.0);
static BitMapFile *image[1]; // Local storage for bmp image data.

float randomFloat(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random*range) + min;
}
vec3 tempNormals[MAP_SIZE][MAP_SIZE];

void CalcNormal()
{
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int z = 0; z < MAP_SIZE; z++) {
			if ((z + 1 == MAP_SIZE) && (x + 1 == MAP_SIZE)) {
				tempNormals[x][z] = glm::normalize(glm::cross(glm::normalize(vec3(0, terrain[x][z - 1] - terrain[x][z], -1)), glm::normalize(vec3(-1, terrain[x - 1][z] - terrain[x][z], 0))));
			}
			else if (x + 1 == MAP_SIZE) {
				tempNormals[x][z] = glm::normalize(glm::cross(glm::normalize(vec3(0, terrain[x][z + 1] - terrain[x][z], 1)), glm::normalize(vec3(-1, terrain[x - 1][z] - terrain[x][z], 0))));
			}
			else if (z + 1 == MAP_SIZE) {
				tempNormals[x][z] = glm::normalize(glm::cross(glm::normalize(vec3(0, terrain[x][z - 1] - terrain[x][z], -1)), glm::normalize(vec3(1, terrain[x + 1][z] - terrain[x][z], 0))));
			}
			else {
				tempNormals[x][z] = glm::normalize(glm::cross(glm::normalize(vec3(0, terrain[x][z + 1] - terrain[x][z], 1)), glm::normalize(vec3(1, terrain[x + 1][z] - terrain[x][z], 0))));
			}
		}
	}
}

//DiamondSquare
void Diamond(int x1, int y1, int width, float iteration) {
	//Diamond
	terrain[x1 + (width) / 2][y1 + (width) / 2] = ((terrain[x1][y1] + terrain[x1 + width][y1] + terrain[x1][y1 + width] + terrain[x1 + width][y1 + width]) / 4) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
}
void Square(int x1, int y1, int width, float iteration) {
	//Sqaure
	//top
	if (y1 - width / 2 < 0)
		terrain[x1 + width / 2][y1] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1 + width][y1]) / 3) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
	else
		terrain[x1 + width / 2][y1] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1 + width][y1] + terrain[x1 + width / 2][y1 - width / 2]) / 4) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
	//Left
	if (x1 - width / 2 < 0)
		terrain[x1][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1][y1 + width]) / 3) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
	else
		terrain[x1][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1] + terrain[x1][y1 + width] + terrain[x1 - width / 2][y1 + width / 2]) / 4) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
	//Right
	if (x1 +  width / 2 > MAP_SIZE - 1)
		terrain[x1 + width][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1 + width][y1] + terrain[x1 + width][y1 + width]) / 3) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
	else
		terrain[x1 + width][y1 + width / 2] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1 + width][y1] + terrain[x1 + width][y1 + width] + terrain[x1 + width / 2][y1 + width / 2]) / 4) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
	//Bottom
	if (y1 + width / 2 > MAP_SIZE)
		terrain[x1 + width / 2][y1 + width] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1 + width] + terrain[x1 + width][y1 + width]) / 3) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
	else
		terrain[x1 + width / 2][y1 + width] = ((terrain[x1 + (width / 2)][y1 + (width / 2)] + terrain[x1][y1 + width] + terrain[x1 + width][y1 + width] + terrain[x1 + width / 2][y1 +  width / 2]) / 4) + randomFloat(-RAND_AMOUNT / iteration, RAND_AMOUNT / iteration);
}
void DiamondSquare() {
	terrain[0][0] = randomFloat(-START_RAND_AMOUNT, START_RAND_AMOUNT);
	terrain[MAP_SIZE - 1][0] = randomFloat(-START_RAND_AMOUNT, START_RAND_AMOUNT);
	terrain[0][MAP_SIZE - 1] = randomFloat(-START_RAND_AMOUNT, START_RAND_AMOUNT);
	terrain[MAP_SIZE - 1][MAP_SIZE - 1] = randomFloat(-START_RAND_AMOUNT, START_RAND_AMOUNT);

	int width = MAP_SIZE - 1;
	float iteration = 1;
	while (width > 1) {
		for (int x = 0; x < MAP_SIZE - 1; x += width) {
			for (int y = 0; y < MAP_SIZE - 1; y += width) {
				Diamond(x, y, width, iteration);
			}
		}
		for (int x = 0; x < MAP_SIZE - 1; x += width) {
			for (int y = 0; y < MAP_SIZE - 1; y += width) {
				Square(x, y, width, iteration);
			}
		}
		width = width / 2.0f;
		iteration = iteration*iterationAmount;
	}
}

// Function to read text file, used to read shader files
char* readTextFile(char* aTextFile)
{
	FILE* filePointer = fopen(aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

// Function to replace GluPerspective provided by NEHE
// http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	projMat = frustum(-fW, fW, -fH, fH, zNear, zFar);
}

void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 119://Forward
		cameraPos.x += LOS.x * SPEED;
		cameraPos.z += LOS.z * SPEED;
		cameraPos.y += LOS.y * SPEED;
		glutPostRedisplay();
		break;
	case 115:
		cameraPos.x -= LOS.x * SPEED;
		cameraPos.z -= LOS.z * SPEED;
		cameraPos.y -= LOS.y * SPEED;
		glutPostRedisplay();
		break;
	case 97:
		cameraTheta -= ROTSPEED;
		glutPostRedisplay();
		break;
	case 100:
		cameraTheta += ROTSPEED;
		glutPostRedisplay();
		break;
	case 113:
		cameraPhi += ROTSPEED;
		glutPostRedisplay();
		break;
	case 122:
		cameraPhi -= ROTSPEED;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

// Initialization routine.
void setup(void)
{
	// Initialise terrain - set values in the height map to 0
	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int z = 0; z < MAP_SIZE; z++)
		{
			terrain[x][z] = 0;
		}
	}
	srand(SEED);
	//Diamond Square
	DiamondSquare();
	CalcNormal();


	// Intialise vertex array
	int i = 0;

	// Generate texture co-ordinates
	float fTextureS = float(MAP_SIZE)*0.1f;
	float fTextureT = float(MAP_SIZE)*0.1f;

	for (int z = 0; z < MAP_SIZE; z++)
	{
		for (int x = 0; x < MAP_SIZE; x++)
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			terrainVertices[i] = { { (float)x, terrain[x][z], (float)z, 1.0 },{ tempNormals[x][z].x, tempNormals[x][z].y, tempNormals[x][z].z } };
			float fScaleC = float(x) / float(MAP_SIZE - 1);
			float fScaleR = float(z) / float(MAP_SIZE - 1);
			terrainVertices[i].texCoords[0] = fTextureS*fScaleC;			terrainVertices[i].texCoords[1] = fTextureT*fScaleR;
			i++;
		}
	}

	// Now build the index data 
	i = 0;
	for (int z = 0; z < MAP_SIZE; z++)
	{
		i = z * MAP_SIZE;
		for (int x = 0; x < MAP_SIZE * 2; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
		for (int x = 1; x < MAP_SIZE * 2 + 1; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
	}
	glEnable(GL_DEPTH_TEST);	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Create shader program executable - read, compile and link shaders
	char* vertexShader = readTextFile("vertexShader.glsl");
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);

	char* fragmentShader = readTextFile("fragmentShader.glsl");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);
	///////////////////////////////////////
	cout << "Vertex:" << endl;
	shaderCompileTest(vertexShaderId);
	cout << "Fragment:" << endl;
	shaderCompileTest(fragmentShaderId);

	// Create vertex array object (VAO) and vertex buffer object (VBO) and associate data with vertex shader.
	glGenVertexArrays(1, vao);
	glGenBuffers(1, buffer);
	glBindVertexArray(vao[SQUARE]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SQUARE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)sizeof(terrainVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]),(GLvoid*)(sizeof(terrainVertices[0].coords) + sizeof(terrainVertices[0].normals)));
	glEnableVertexAttribArray(2);
	///////////////////////////////////////

	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.ambRefl"), 1,
		&terrainFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.difRefl"), 1,
		&terrainFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.specRefl"), 1,
		&terrainFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.emitCols"), 1,
		&terrainFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "terrainFandB.shininess"),
		terrainFandB.shininess);

	glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1,
		&light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1,
		&light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1,
		&light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1,
		&light0.coords[0]);	mat3 normalMat = mat3(1);
	normalMatLoc = glGetUniformLocation(programId, "normalMat");

	///////////////////////////////////////

	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");
	projMat = glm::perspective(1.0472, 1.0, 0.1, 2000.0);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
	// Obtain modelview matrix uniform location and set value.
	modelViewMat = lookAt(vec3(0, 0, 100), vec3(0, -15, 0), vec3(0, 1, 0));
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));	normalMat = transpose(inverse(mat3(modelViewMat)));
	glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));
	///////////////////////////////////////
	// Texture loading
	// Load the image.
	image[0] = getbmp("Grass.bmp");
	// Create texture id.
	glGenTextures(1, texture);
	// Bind grass image.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	grassTexLoc = glGetUniformLocation(programId, "grassTex");
	glUniform1i(grassTexLoc, 0);

	///////////////////////////////////////
}

// Drawing routine.
void drawScene(void)
{
	LOS.x = cos(cameraPhi*3.14 / 180)* sin(cameraTheta*3.14 / 180);
	LOS.y = sin(cameraPhi*3.14 / 180);
	LOS.z = cos(cameraPhi*3.14 / 180) * -cos(cameraTheta*3.14 / 180);

	modelViewMat = lookAt(vec3(cameraPos.x, cameraPos.y, cameraPos.z), vec3(cameraPos.x + LOS.x, cameraPos.y + LOS.y, cameraPos.z + LOS.z), vec3(0, 1, 0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// For each row - draw the triangle strip
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, terrainIndexData[i]);
	}

	glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}


// Main routine.
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	// Set the version of OpenGL (4.2)
	glutInitContextVersion(4, 2);
	// The core profile excludes all discarded features
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// Forward compatibility excludes features marked for deprecation ensuring compatability with future versions
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TerrainGeneration");

	// Set OpenGL to render in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
