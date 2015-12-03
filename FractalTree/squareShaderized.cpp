////////////////////////////////////////////////////////          
// squareShaderized.cpp
//
// Forward-compatible core GL 4.3 version of square.cpp.
// 
// Sumanta Guha
////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>


#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Vertex.h"
#include "Branch.h"
#include "Leaf.h"
#include "TreeBufferPos.h"

#include "tree.h"

#pragma comment(lib, "glew32.lib") 

using namespace std;

using namespace glm;

struct Matrix4x4
{
   float entries[16];
};

static const Matrix4x4 IDENTITY_MATRIX4x4 = 
{
   {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0
   }
};

static enum buffer {SQUARE_VERTICES}; 
static enum object {SQUARE}; 

// Globals
TreeBufferPos pos;
Vertex drawVertices[2000];

mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

//Camera
const float SPEED = 1;
const float ROTSPEED = 2;
float cameraTheta = 0;
float cameraPhi = 0;
vec3 cameraPos(0, 0, 100);
//Line of sight
vec3 LOS(0, 0, -1);

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   buffer[1],
   vao[1];
   
// Function to read text file.
char* readTextFile(char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");	
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char)); 
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

// Initialization routine.
void setup(void)
{
	
	//genTree();
	tree Tree;
	//pos = Tree.genTree(drawVertices, 0, 8,25,25);
	pos = Tree.genTree(drawVertices, 0, 8, 15, 15);
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   // Create shader program executable.
   char* vertexShader = readTextFile("vertexShader.glsl");
   vertexShaderId = glCreateShader(GL_VERTEX_SHADER); 
   glShaderSource(vertexShaderId, 1, (const char**) &vertexShader, NULL); 
   glCompileShader(vertexShaderId); 

   char* fragmentShader = readTextFile("fragmentShader.glsl");
   fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShaderId, 1, (const char**) &fragmentShader, NULL); 
   glCompileShader(fragmentShaderId); 

   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId); 
   glLinkProgram(programId); 
   glUseProgram(programId); 
   ///////////////////////////////////////

   // Create VAO and VBO and associate data with vertex shader.
   glGenVertexArrays(1, vao);
   glGenBuffers(1, buffer);
   glBindVertexArray(vao[SQUARE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SQUARE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(drawVertices), drawVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(drawVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(drawVertices[0]), (GLvoid*)sizeof(drawVertices[0].coords));
   glEnableVertexAttribArray(1);
   ///////////////////////////////////////

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId, "projMat");
   //projMat = glm::ortho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   projMat = glm::perspective(1.0472, 1.0, 0.1, 200.0);
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   ///////////////////////////////////////

   // Obtain modelview matrix uniform location and set value.
   //Matrix4x4 modelViewMat = IDENTITY_MATRIX4x4;
   modelViewMat = lookAt(vec3(0, 0, 100), vec3(0, -15, 0), vec3(0,1,0));
   modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); 
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   ///////////////////////////////////////
}

// Drawing routine.
void drawScene(void)
{
	/*LOS.x = sin(cameraTheta*3.14/180);
	LOS.z = -cos(cameraTheta*3.14/180);
	LOS = glm::normalize(LOS);*/

	LOS.x = cos(cameraPhi*3.14 / 180)* sin(cameraTheta*3.14 / 180);
	LOS.y = sin(cameraPhi*3.14 / 180);
	LOS.z = cos(cameraPhi*3.14 / 180) * -cos(cameraTheta*3.14 / 180);

	modelViewMat = lookAt(vec3(cameraPos.x, cameraPos.y, cameraPos.z), vec3(cameraPos.x+LOS.x, cameraPos.y+LOS.y, cameraPos.z + LOS.z), vec3(0, 1, 0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    //Draw tree
	//glDrawArrays(GL_LINES, 0, pos.treeFinish+1);
    int q = 0;
    for (int i = pos.treeStart; i < pos.treeFinish +1; i += 2) {
	    glLineWidth( (pos.depth[(q-1)/2])*2);
	    glDrawArrays(GL_LINES, i, 2);
	    q += 2;
   }
   //Draw leaves
   for (int i = pos.leafStart; i < pos.leafFinish; i += 4) {
	    glDrawArrays(GL_TRIANGLE_STRIP, i, 4);
   }
  
   glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
}
void reGen() {
	tree Tree;
	pos = Tree.genTree(drawVertices, 0, 8, 25, 25);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SQUARE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(drawVertices), drawVertices, GL_STATIC_DRAW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 27:
			exit(0);
			break;
		case 119://Forward
			cameraPos.x += LOS.x * SPEED;
			cameraPos.z += LOS.z * SPEED;
			glutPostRedisplay();
			break;
		case 115:
			cameraPos.x -= LOS.x * SPEED;
			cameraPos.z -= LOS.z * SPEED;
			glutPostRedisplay();
			break;
		case 97:
			cameraTheta-=ROTSPEED;
			glutPostRedisplay();
			break;
		case 100:
			cameraTheta+=ROTSPEED;
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
		case 114:
			reGen();
			glutPostRedisplay();
			break;
		default:
			break;
   }
}

// Main routine.
int main(int argc, char* argv[])
{	
   glutInit(&argc, argv);

   glutInitContextVersion(4, 2);
   glutInitContextProfile(GLUT_CORE_PROFILE);
  // glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
   
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("squareShaderized.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   
   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
