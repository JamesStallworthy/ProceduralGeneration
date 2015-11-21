////////////////////////////////////////////////////////          
// squareShaderized.cpp
//
// Forward-compatible core GL 4.3 version of square.cpp.
// 
// Sumanta Guha
////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"
#include "Branch.h"
#include "Leaf.h"

#include "tree.h"

#pragma comment(lib, "glew32.lib") 
#endif

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
Leaf leaves[16];
Vertex drawVertices[1000];
Branch branches[1000];
int branchesIterator = 0;

static Matrix4x4
modelViewMat = IDENTITY_MATRIX4x4;
static mat4 projMat = mat4(1.0);

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
	Tree.genTree(drawVertices,0);
   glClearColor(1.0, 1.0, 1.0, 0.0);

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
   projMat = glm::ortho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   ///////////////////////////////////////

   // Obtain modelview matrix uniform location and set value.
   Matrix4x4 modelViewMat = IDENTITY_MATRIX4x4;
   modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); 
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, modelViewMat.entries);
   ///////////////////////////////////////
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glLineWidth(8);
   //Draw tree
   for (int i = 0; i < 62; i += 2) {
	   glDrawArrays(GL_LINES, i, 2);
   }
   //Draw leaves
   for (int i = 62; i < 62+48; i += 3) {
	   glDrawArrays(GL_TRIANGLES, i, 3);
   }
  
   glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
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
