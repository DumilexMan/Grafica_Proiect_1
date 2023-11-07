//
// ================================================
// | Grafica pe calculator                        |
// =====================================================
// | Laboratorul III - 03_05_transformari_keyboard.cpp |
// =====================================================
// 
//	Realizarea unei scene 2D in care obiectele se misca, folosindu-se tehnicile MODERN OpenGL;
//	ELEMENTE DE NOUTATE:
//	- functii pentru tastatura;
//
//
// 
//	Biblioteci

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
EboId,
viewLocation,
projLocation,
myMatrixLocation,
matrScaleLocation,
matrTranslLocation,
matrRotlLocation,
codColLocation;

GLuint
texture;

//	Dimensiunile ferestrei de afisare;
GLfloat
winWidth = 800, winHeight = 600;
//	Variabile catre matricile de transformare;
glm::mat4
myMatrix, resizeMatrix, matrTransl, matrTransl1, matrTransl2, matrScale, matrRot;

int codCol;							//	Variabila ce determina schimbarea culorii pixelilor in shader;
float angle = 0;					//	Unghiul de rotire al patratului;
float tx = 0; float ty = 100;			//	Coordonatele de translatie ale patratului pe Ox si Oy;
float xMin = -400.f, xMax = 400.f, yMin = -300.f, yMax = 300.f;		//	Variabile pentru proiectia ortogonala;
GLfloat aux1, aux2 = 800.0f;


void Move() {
	//Aici o sa se miste marcajul și copacii
	if (ty > -120)
	{
		ty = ty - 5;
	}
	else
		ty = 100;


	glutPostRedisplay();	//	Actualizare
}

void LoadTexture(const char* photoPath)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//	Desfasurarea imaginii pe orizonatala/verticala in functie de parametrii de texturare;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = SOIL_load_image(photoPath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
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
	ProgramId = LoadShaders("Proiect_Shader.vert", "Proiect_Shader.frag");
	glUseProgram(ProgramId);
}

//  Se initializeaza un Vertex Buffer Object (VBO) pentru tranferul datelor spre memoria placii grafice (spre shadere);
//  In acesta se stocheaza date despre varfuri (coordonate, culori, indici, texturare etc.);
void CreateVBO(void)
{
	//  Coordonatele varfurilor;
	GLfloat Vertices[] = {
		//	Varfuri pentru dreptunghi;
		//	1
		-800.0f, -300.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 800.0f, -300.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		 800.0f,  300.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-800.0f,  300.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,

		0.265f, 0.714f, 0.0f, 1.0f,
		0.195f, 0.695f, 0.0f, 1.0f,
		0.1914f, 0.68996f, 0.0f, 1.0f,
		0.1575f, 0.66845f, 0.0f, 1.0f,
		0.1532f, 0.66124f, 0.0f, 1.0f,
		0.1444f, 0.6149f, 0.0f, 1.0f,
		0.14446f, 0.4586f, 0.0f, 1.0f,
		0.12836f, 0.4483f, 0.0f, 1.0f,
		0.12615f, 0.4354f, 0.0f, 1.0f,
		0.1449f, 0.4401f, 0.0f, 1.0f,
		0.1447f, 0.3579f, 0.0f, 1.0f,
		0.142f, 0.3534f, 0.0f, 1.0f,
		0.142f, 0.336f, 0.0f, 1.0f,
		0.14477f, 0.32993f, 0.0f, 1.0f,
		0.1451f, 0.2861f, 0.0f, 1.0f,
		0.1433f, 0.2661f, 0.0f, 1.0f,
		0.1434f, 0.224f, 0.0f, 1.0f,
		0.144f, 0.214f, 0.0f, 1.0f,
		0.154f, 0.172f, 0.0f, 1.0f,
		0.1595f, 0.1533f, 0.0f, 1.0f,
		0.16574f, 0.1403f, 0.0f, 1.0f,
		0.175f, 0.1299f, 0.0f, 1.0f,
		0.1925f, 0.1237f, 0.0f, 1.0f,
		0.21605f, 0.1196f, 0.0f, 1.0f,
		0.2655f, 0.1161f, 0.0f, 1.0f,
		0.30722f, 0.1196f, 0.0f, 1.0f,
		0.33835f, 0.1237f, 0.0f, 1.0f,
		0.356f, 0.1299f, 0.0f, 1.0f,
		0.36556f, 0.1403f, 0.0f, 1.0f,
		0.37186f, 0.1533f, 0.0f, 1.0f,
		0.3767f, 0.172f, 0.0f, 1.0f,
		0.3864f, 0.214f, 0.0f, 1.0f,
		0.3877f, 0.224f, 0.0f, 1.0f,
		0.388f, 0.2661f, 0.0f, 1.0f,
		0.3863f, 0.2861f, 0.0f, 1.0f,
		0.3863f, 0.32993f, 0.0f, 1.0f,
		0.38955f, 0.336f, 0.0f, 1.0f,
		0.38965f, 0.3534f, 0.0f, 1.0f,
		0.3863f, 0.3579f, 0.0f, 1.0f,
		0.386f, 0.4401f, 0.0f, 1.0f,
		0.40726f, 0.4354f, 0.0f, 1.0f,
		0.4036f, 0.4483f, 0.0f, 1.0f,
		0.386f, 0.4586f, 0.0f, 1.0f,
		0.3867f, 0.6149f, 0.0f, 1.0f,
		0.37745f, 0.6612f, 0.0f, 1.0f,
		0.374f, 0.6684f, 0.0f, 1.0f,
		0.3398f, 0.6894f, 0.0f, 1.0f,
		0.3357f, 0.695f, 0.0f, 1.0f,

	};

	GLuint Indices[] = {
	  0, 1, 2, 3, 0,
	};

	//  Transmiterea datelor prin buffere;

	//  Se creeaza un buffer pentru VARFURI;
	glGenBuffers(1, &VboId);                                                        //  Generarea bufferului si indexarea acestuia catre variabila VboId;
	glBindBuffer(GL_ARRAY_BUFFER, VboId);                                           //  Setarea tipului de buffer - atributele varfurilor;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	//  Se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO;
	glGenVertexArrays(1, &VaoId);                                                   //  Generarea VAO si indexarea acestuia catre variabila VaoId;
	glBindVertexArray(VaoId);
	//  Punctele sunt "copiate" in bufferul curent;

   //	Se creeaza un buffer pentru INDICI;
	glGenBuffers(1, &EboId);														//  Generarea bufferului si indexarea acestuia catre variabila EboId;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);									//  Setarea tipului de buffer - atributele varfurilor;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


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
	//  Eliberarea atributelor din shadere (pozitie, culoare, texturare etc.);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//  Stergerea bufferelor pentru varfuri, culori;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EboId);
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
	//  Trecerea datelor de randare spre bufferul folosit de shadere;
	CreateShaders();							//  Initilizarea shaderelor;
	//	Instantierea variabilelor uniforme pentru a "comunica" cu shaderele;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projLocation = glGetUniformLocation(ProgramId, "projection");

	//viewLocation = glGetUniformLocation(ProgramId, "view");
	//projLocation = glGetUniformLocation(ProgramId, "projection");

	//	Pentru shaderul de fragmente;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
}

//  Functia de desenarea a graficii pe ecran;
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);			//  Se curata ecranul OpenGL pentru a fi desenat noul continut;

	//	Matrici pentru transformari;
	resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, 0.0)); // drum
	matrTransl1 = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, 0.0));// masini
	angle = 90.0f; // Setăm unghiul la 90 de grade (spre dreapta)
	matrRot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	//myMatrix = resizeMatrix;
	myMatrix = resizeMatrix * matrTransl * matrRot;

	CreateVBO();

	LoadTexture("drum.jpeg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//	Matricea pentru elementele care isi schimba pozitia;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);


	glDrawArrays(GL_QUADS, 0, 4);

	myMatrix = resizeMatrix * matrTransl1 * matrRot;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	



	glutSwapBuffers();	//	Inlocuieste imaginea deseneata in fereastra cu cea randata; 
	glFlush();			//  Asigura rularea tuturor comenzilor OpenGL apelate anterior;
}

//	Punctul de intrare in program, se ruleaza rutina OpenGL;
int main(int argc, char* argv[])
{
	//  Se initializeaza GLUT si contextul OpenGL si se configureaza fereastra si modul de afisare;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);					//	Se folosesc 2 buffere (unul pentru afisare si unul pentru randare => animatii cursive) si culori RGB;
	glutInitWindowSize(winWidth, winHeight);						//  Dimensiunea ferestrei;
	glutInitWindowPosition(100, 100);								//  Pozitia initiala a ferestrei;
	glutCreateWindow("Proiect_drum - OpenGL <<nou>>");		//	Creeaza fereastra de vizualizare, indicand numele acesteia;

	//	Se initializeaza GLEW si se verifica suportul de extensii OpenGL modern disponibile pe sistemul gazda;
	//  Trebuie initializat inainte de desenare;

	glewInit();

	Initialize();							//  Setarea parametrilor necesari pentru fereastra de vizualizare; 
	glutDisplayFunc(RenderFunction);		//  Desenarea scenei in fereastra;
	glutIdleFunc(Move);
	glutCloseFunc(Cleanup);					//  Eliberarea resurselor alocate de program;

	//  Bucla principala de procesare a evenimentelor GLUT (functiile care incep cu glut: glutInit etc.) este pornita;
	//  Prelucreaza evenimentele si deseneaza fereastra OpenGL pana cand utilizatorul o inchide;

	glutMainLoop();

	return 0;
}
