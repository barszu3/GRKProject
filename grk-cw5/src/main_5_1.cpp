#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

GLuint programColor;
GLuint programTexture;
GLuint moonTexture;
GLuint sunTexture;
GLuint mercuryTexture;
GLuint venusTexture;
GLuint earthTexture;
GLuint marsTexture;
GLuint jupiterTexture;
GLuint saturnTexture;
GLuint shipTexture;
GLuint meteoriteTexture;

Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;
obj::Model boxModel;

const int ile = 5000; // ISO C++ forbids variable length array 'losuj'
int losuj[ile];

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-7, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	switch(key)
	{
	case 'q': cameraAngle -= angleSpeed; break;
	case 'e': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	}
}

glm::mat4 createCameraMatrix()
{
	// Obliczanie kierunku patrzenia kamery (w plaszczyznie x-z) przy uzyciu zmiennej cameraAngle kontrolowanej przez klawisze.
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0,1,0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}



void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint textureID)
{
	GLuint program = programTexture;

	glUseProgram(program);

	//glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	Core::SetActiveTexture(textureID, "objectTex", program, 0);

	Core::DrawModel(model);

	glUseProgram(0);
}

void renderScene()
{
	// Aktualizacja macierzy widoku i rzutowania. Macierze sa przechowywane w zmiennych globalnych, bo uzywa ich funkcja drawObject.
	// (Bardziej elegancko byloby przekazac je jako argumenty do funkcji, ale robimy tak dla uproszczenia kodu.
	//  Jest to mozliwe dzieki temu, ze macierze widoku i rzutowania sa takie same dla wszystkich obiektow!)



	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	

	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 rotationY;
	// ZADANIE: Wyswietl obracajacy sie czworokat
	//
	// Uzyj zmiennej "time" zeby ustalic taka macierz rotacji, ktora powoduje obrot wokol poczatku ukladu wspolrzednych
	// Kat rotacji musi byc wyrazony w radianach
	struct Planet {
		float size;
		float distance;
		GLuint texture;
	};
	translation[3][0] = sin(time*0.0005f*(-1))*5000;
	rotation[0][0] = cos(time);
	rotation[0][1] = -sin(time);
	rotation[1][0] = sin(time);
	rotation[1][1] = cos(time);
	rotationY[0][0] = cos(time);
	rotationY[0][2] = sin(time);
	rotationY[2][0] = -sin(time);
	rotationY[2][2] = cos(time);


	glm::mat4 rotEarth;
	glm::mat4 rotMoon;
	glm::mat4 rotPlanet;
	glm::mat4 rotSaturn;
	Planet planets[7];

		planets[0] = { 0.9f,40.0f, mercuryTexture };
		planets[1] = { 1.5f,50.0f, venusTexture };
		planets[2] = { 3.0f,65.0f ,earthTexture };
		planets[3] = { 1.9f,80.0f, marsTexture };
		planets[4] = { 7.00f,95.0f, jupiterTexture };
		planets[5] = { 5.00f,110.0f, saturnTexture };
		planets[6] = { 50.00f, 0.0f, sunTexture };
		
	

	rotEarth[0][0] = cos(time * 1 / 0.5f);
	rotEarth[2][0] = -sin(time *  1 / 0.5f);
	rotEarth[0][2] = sin(time  * 1 / 0.5f);
	rotEarth[2][2] = cos(time  * 1 / 0.5f);
	rotEarth[3][0] = sin(time  * 1 / 3.0f)*65.0f;
	rotEarth[3][2] = cos(time  * 1 / 3.0f)*65.0f;

	rotMoon[0][0] = cos(time * 1 / 0.5f);
	rotMoon[2][0] = -sin(time * 1 / 0.5f);
	rotMoon[0][2] = sin(time * 1 / 0.5f);
	rotMoon[2][2] = cos(time * 1 / 0.5f);
	rotMoon[3][0] = sin(time * 1 / 3.2f)*7.1f;
	rotMoon[3][2] = cos(time * 1 / 3.2f)*7.1f;


	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Macierz statku "przyczepia" go do kamery. Warto przeanalizowac te linijke i zrozumiec jak to dziala.
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.25f));
	drawObjectTexture(&shipModel, shipModelMatrix, shipTexture);

	//drawObjectColor(&boxModel, glm::translate(glm::vec3(2, 0, 2)), glm::vec3(0.8f, 0.2f, 0.3f));
	

	int z = 0;
	for (int l = -11; l < 12; l++) {
		for (int i = -10; i < 10; i++) {
			for (int j = 0; j < 5; j++) {

				drawObjectTexture(&sphereModel, glm::translate(glm::vec3(2+l, i, losuj[z]-10) / 6)* glm::scale(glm::vec3(0.02f))* translation * rotation, meteoriteTexture);
				z++;
			}
		}
	}

	
	for (int i = 0; i < 6; i++) {

		rotPlanet[0][0] = cos(time * 1 / 0.5f);
		rotPlanet[2][0] = -sin(time * 1 / 0.5f);
		rotPlanet[0][2] = sin(time * 1 / 0.5f);
		rotPlanet[2][2] = cos(time * 1 / 0.5f);
		rotPlanet[3][0] = sin(time * 1 / planets[i].size)*planets[i].distance;
		rotPlanet[3][2] = cos(time * 1 / planets[i].size)*planets[i].distance;

		drawObjectTexture(&sphereModel, glm::translate(glm::vec3(70, 0, 25))*rotPlanet* glm::scale(glm::vec3(planets[i].size)), planets[i].texture);
		
	}

	drawObjectTexture(&sphereModel, glm::translate(glm::vec3(70, 0, 25))*rotationY* glm::scale(glm::vec3(30.0f)), sunTexture);
	drawObjectTexture(&sphereModel, glm::translate(glm::vec3(70, 0, 25))*rotEarth*rotMoon * glm::scale(glm::vec3(0.8f)), moonTexture);
	

	glutSwapBuffers();
}

void init()
{
	
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");

	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	shipModel = obj::loadModelFromFile("models/spaceship.obj");
	//boxModel = obj::loadModelFromFile("models/cuprocket.obj");

	earthTexture = Core::LoadTexture("textures/earth.png");
	shipTexture = Core::LoadTexture("textures/ship.png");
	moonTexture = Core::LoadTexture("textures/moon2.png");
	mercuryTexture = Core::LoadTexture("textures/mercury.png");
	venusTexture = Core::LoadTexture("textures/venus.png");
	marsTexture = Core::LoadTexture("textures/mars.png");
	jupiterTexture = Core::LoadTexture("textures/jupiter.png");
	saturnTexture = Core::LoadTexture("textures/saturn.png");
	sunTexture = Core::LoadTexture("textures/sun.png");
	meteoriteTexture = Core::LoadTexture("textures/nowa2.png");
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	srand(time(NULL));
	
	for (int i = 0; i < ile; i++)
	{
		losuj[i] = ((rand() % 10) )/10 + ((rand() % 30));
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("SPACE");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
