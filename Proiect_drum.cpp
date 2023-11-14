#include <windows.h>        //	Utilizarea functiilor de sistem Windows (crearea de ferestre, manipularea fisierelor si directoarelor);
#include <stdlib.h>         //  Biblioteci necesare pentru citirea shaderelor;
#include <stdio.h>
#include <GL/glew.h>        //  Definește prototipurile functiilor OpenGL si constantele necesare pentru programarea OpenGL moderna; 
#include <GL/freeglut.h>    //	Include functii pentru: 
//	- gestionarea ferestrelor si evenimentelor de tastatura si mouse, 
//  - desenarea de primitive grafice precum dreptunghiuri, cercuri sau linii, 
//  - crearea de meniuri si submeniuri;
#include "loadShaders.h"	//	Fisierul care face legatura intre program si shadere;
#include "glm/glm.hpp"		//	Bibloteci utilizate pentru transformari grafice;
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SOIL.h"			//	Biblioteca pentru texturare;

//  Identificatorii obiectelor de tip OpenGL;
GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
matrScaleLocation,
matrTranslLocation;

GLuint
texture;
//	Dimensiunile ferestrei de afisare;
GLfloat
winWidth = 800, winHeight = 800;
//	Variabile catre matricile de transformare;
glm::mat4
myMatrix,
resizeMatrix,
matrTranslRoad,
matrTranslCar,
matrTranslPolice,
matrTranslTree,
matrRot,
matrTranslCarInv,
matrTranslCarT4,
matrScaleCars;


float tx = 0.0f; float ty = 100.0f;			//	Coordonatele de translatie ale patratului pe Ox si Oy;
float tx1 = 100.0f; float ty1 = -340.0f;	// translatie masina
float tx2 = 300.0f; float ty2 = 30.0f;		// translatie masina de politie
float tx3 = 0.0f;   float ty3 = 0.0f;		// translatie copaci
float tx4 = 0.0f;   float ty4 = 0.0f;		// miscare copaci
float xMin = -400.f, xMax = 400.f, yMin = -300.f, yMax = 300.f;		//	Variabile pentru proiectia ortogonala;
float coefx, coefy;
float angle = 0;
int refreshMillis = 10;


void Move() {
	//Aici o sa se miste marcajul și copacii
	if (ty > -100)
	{
		ty = ty - 20;
		ty3 = ty3 - 20;
	}
	else
	{
		ty = 90;
		ty3 = 90;
	}
	glutPostRedisplay();	//	Actualizare
}


void ProcessSpecialKeys(int key, int xx, int yy)
{
	// Coeficienti folositi pentru a misca masina pe directia in care este orientata
	float coefX = cos(glm::radians(angle + 90));
	float coefY = sin(glm::radians(angle + 90));
	if (ty1 <= -250) return;
	if (ty1 >= 200) return;
	switch (key)			//	Procesarea tastelor 'LEFT', 'RIGHT', 'UP', 'DOWN'
	{						//	duce la deplasarea patratului pe axele Ox si Oy;
	case GLUT_KEY_LEFT:

	{
		if (tx1 < 270.0f or ty1 < 60.0f)
		{
			angle += 5;
			coefX = cos(glm::radians(angle + 90));
			coefY = sin(glm::radians(angle + 90));
		}
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		if (tx1 < 270.0f or ty1 < 60.0f)
		{
			angle -= 5;
			coefX = cos(glm::radians(angle + 90));
			coefY = sin(glm::radians(angle + 90));
		}
		break;
	}
	case GLUT_KEY_UP:
	{
		ty4 += 5 * coefY;
		ty1 += 5 * coefY;
		tx1 += 5 * coefX;
		tx4 += 5 * coefX;
		break;
	}

	case GLUT_KEY_DOWN:
	{
		ty4 -= 5 * coefY;
		ty1 -= 5 * coefY;
		tx1 -= 5 * coefX;
		tx4 -= 5 * coefX;
		break;
	}
	}
}

void LoadTexture(const char* photoPath)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//	Desfasurarea imaginii pe orizonatala/verticala in functie de parametrii de texturare;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = SOIL_load_image(photoPath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//  Crearea si compilarea obiectelor de tip shader;
//	Trebuie sa fie in acelasi director cu proiectul actual;
//  Shaderul de varfuri / Vertex shader - afecteaza geometria scenei;
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
void CreateShaders(void)
{
	ProgramId = LoadShaders("proiect_shader.vert", "proiect_shader.frag");
	glUseProgram(ProgramId);
}

//  Se initializeaza un Vertex Buffer Object (VBO) pentru tranferul datelor spre memoria placii grafice (spre shadere);
//  In acesta se stocheaza date despre varfuri (coordonate, culori, indici, texturare etc.);
void CreateVBO(void)
{
	//  Coordonatele varfurilor;
	GLfloat Vertices[] = {
		// drum
	   -400.0f, -500.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		400.0f, -500.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		400.0f,  500.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	   -400.0f,  500.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	   // masina 
	   25.0f, -400.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	   175.0f, -400.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f,
	   175.0f, -280.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	   25.0f, -280.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	   // politie
	   -300.0f, -160.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
	   -100.0f, -160.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f,
	   -100.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 1.0f,
	   -300.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    0.0f, 1.0f,

	   // primul copac
	   -450.0f, -250.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	   -150.0f, -250.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f,
	   -150.0f, -100.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	   -450.0f, -100.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	   // al doilea copac
	   150.0f,  350.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	   450.0f,  350.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f,
	   450.0f,  200.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	   150.0f,  200.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	};

	//  Se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO;
	glGenVertexArrays(1, &VaoId);                                                   //  Generarea VAO si indexarea acestuia catre variabila VaoId;
	glBindVertexArray(VaoId);

	//  Se creeaza un buffer pentru VARFURI;
	glGenBuffers(1, &VboId);                                                        //  Generarea bufferului si indexarea acestuia catre variabila VboId;
	glBindBuffer(GL_ARRAY_BUFFER, VboId);                                           //  Setarea tipului de buffer - atributele varfurilor;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);      //  Punctele sunt "copiate" in bufferul curent;

	//  Se asociaza atributul (0 = coordonate) pentru shader;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

	//  Se asociaza atributul (1 =  culoare) pentru shader;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

	//  Se asociaza atributul (2 =  texturare) pentru shader;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

//  Elimina obiectele de tip shader dupa rulare;
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

//  Eliminarea obiectelor de tip VBO dupa rulare;
void DestroyVBO(void)
{
	//  Eliberarea atributelor dinshadere (pozitie, culoare, texturare etc.);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//  Stergerea bufferelor pentru varfuri, culori;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId);

	//  Eliberaea obiectelor de tip VAO;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

//  Functia de eliberare a resurselor alocate de program;
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

//  Setarea parametrilor necesari pentru fereastra de vizualizare;
void Initialize(void)
{
	glClearColor(0.0f, 0.64f, 0.0f, 1.0f);		//  Culoarea de fond a ecranului;
	CreateVBO();								//  Trecerea datelor de randare spre bufferul folosit de shadere;
	CreateShaders();							//  Initilizarea shaderelor;

	//	Instantierea variabilelor uniforme pentru a "comunica" cu shaderele;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");

	//	Dreptunghiul "decupat"; 
	resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
}

//  Functia de desenarea a graficii pe ecran;
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);			//  Se curata ecranul OpenGL pentru a fi desenat noul continut;

	//	Transformari
	matrTranslRoad = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, 0.0));			// drum
	matrTranslCar = glm::translate(glm::mat4(1.0f), glm::vec3(tx1, ty1, 0.0));			// masini
	matrTranslPolice = glm::translate(glm::mat4(1.0f), glm::vec3(tx2, ty2, 0.0));		// masina politie
	matrTranslTree = glm::translate(glm::mat4(1.0f), glm::vec3(tx3, ty3, 0.0));			// copaci
	matrRot = glm::rotate(glm::mat4(1.0f), glm::radians((float)angle), glm::vec3(0.0f, 0.0f, 1.0f));	// rotatie
	matrTranslCarInv = glm::translate(glm::mat4(1.0f), glm::vec3(-tx1, -ty1, 0.0));		// masina inversa
	matrTranslCarT4 = glm::translate(glm::mat4(1.0f), glm::vec3(tx4, ty4, 0.0));        // miscare masina
	matrScaleCars = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 1.0f));         // ambele masini

	// Drum
	myMatrix = resizeMatrix;
	CreateVBO();
	LoadTexture("drum3.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	myMatrix = resizeMatrix * matrTranslRoad;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArrays(GL_QUADS, 0, 4);

	// Masina
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	LoadTexture("car1.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	myMatrix = resizeMatrix * matrScaleCars * matrTranslCar * matrRot * matrTranslCarInv * matrTranslCarT4;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArrays(GL_QUADS, 4, 4);

	// Masina de politie
	LoadTexture("police.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	myMatrix = resizeMatrix * matrScaleCars * matrTranslPolice;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArrays(GL_QUADS, 8, 4);

	// Primul copac
	LoadTexture("tree.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	myMatrix = resizeMatrix * matrTranslTree;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArrays(GL_QUADS, 12, 4);

	// Al doilea copac
	LoadTexture("tree.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	myMatrix = resizeMatrix * matrTranslTree;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArrays(GL_QUADS, 16, 4);

	glutSwapBuffers();	//	Inlocuieste imaginea deseneata in fereastra cu cea randata; 

	// Animatia de inceput
	if (ty1 <= -250) {
		ty4 += 5;
		ty1 += 5;
	}

	if (tx1 >= 80.0f and ty1 >= 200.0f and angle == 0) {
		ty1 += 10;
		ty4 += 10;
		ty2 += 10;
	}

	glFlush();	//  Asigura rularea tuturor comenzilor OpenGL apelate anterior;
}


void Timer(int value) {
	glutPostRedisplay(); // Marcheaza fereastra care trebuie sa fie reafisata
	glutTimerFunc(refreshMillis, Timer, 0); // Functie care ajuta asigurarea rerandarii
}


//	Punctul de intrare in program, se ruleaza rutina OpenGL;
int main(int argc, char* argv[])
{
	//  Se initializeaza GLUT si contextul OpenGL si se configureaza fereastra si modul de afisare;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);					//	Se folosesc 2 buffere (unul pentru afisare si unul pentru randare => animatii cursive) si culori RGB;
	glutInitWindowSize(winWidth, winHeight);						//  Dimensiunea ferestrei;
	glutInitWindowPosition(30, 0);								//  Pozitia initiala a ferestrei;
	glutCreateWindow("Proiect 1 - OpenGL <<nou>>");		//	Creeaza fereastra de vizualizare, indicand numele acesteia;

	//	Se initializeaza GLEW si se verifica suportul de extensii OpenGL modern disponibile pe sistemul gazda;
	//  Trebuie initializat inainte de desenare;

	glewInit();
	Initialize();							//  Setarea parametrilor necesari pentru fereastra de vizualizare; 
	glutDisplayFunc(RenderFunction);		//  Desenarea scenei in fereastra;
	glutIdleFunc(RenderFunction);			//	Asigura rularea continua a randarii;
	glutIdleFunc(Move);						//  Functie pentru animatie
	glutSpecialFunc(ProcessSpecialKeys);	//  Functie pentru folosirea tastelor
	glutCloseFunc(Cleanup);					//  Eliberarea resurselor alocate de program;
	glutTimerFunc(0, Timer, 0);				//  // Functie care ajuta asigurarea rerandarii

	//  Bucla principala de procesare a evenimentelor GLUT (functiile care incep cu glut: glutInit etc.) este pornita;
	//  Prelucreaza evenimentele si deseneaza fereastra OpenGL pana cand utilizatorul o inchide;
	glutMainLoop();
	return 0;
}
