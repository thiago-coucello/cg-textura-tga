#include "glut.h"
#include "tgaload.h"
#include <string>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535897
#define NO_TEXTURES 3	// Numero de texturas utilizadas

using namespace std;

// vetor com os números das texturas
GLuint texture_id[NO_TEXTURES];

int textureNumber = 0, stopAnimation = 1;
GLfloat angle, fAspect, rotationAngle = 0;
	
// **********************************************************************
//  void initTexture(void)
//		Define a textura a ser usada 
//
// **********************************************************************
void initTexture (void)
{

	// Habilita o uso de textura 
	glEnable ( GL_TEXTURE_2D );

	// Define a forma de armazenamento dos pixels na textura (1= alihamento por byte)
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	// Define quantas texturas ser?o usadas no programa 
	glGenTextures (1, texture_id);  // 1 = uma textura;
									// texture_id = vetor que guardas os n?meros das texturas

	// Define o n?mero da textura do cubo.
	texture_id[0] = 1001;
	texture_id[1] = 1010;
	texture_id[2] = 1011;

	// Define que tipo de textura será usada
	// GL_TEXTURE_2D ==> define que será usada uma textura 2D (bitmaps)
	// texture_id[CUBE_TEXTURE]  ==> define o número da textura 
	glBindTexture ( GL_TEXTURE_2D, texture_id[0] );

	// carrega a uma imagem TGA 
	image_t temp_image;
	tgaLoad  ( "earth.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );
	
	glBindTexture( GL_TEXTURE_2D, texture_id[1]);
	tgaLoad  ( "tartaruga.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );
	
	glBindTexture( GL_TEXTURE_2D, texture_id[2]);
	tgaLoad  ( "Tree.tga", &temp_image, TGA_FREE | TGA_LOW_QUALITY );
	
	glDisable(GL_TEXTURE_2D);
}

void drawCircle(float radius) {
	glBegin(GL_TRIANGLE_FAN);
		for (float i = 0.0; i <= 2 * PI; i += 0.001) {
			point(cos(i) * radius, sin(i) * radius, 0);
		}
	glEnd();
}

void drawWireSquare(float size, float lineWidth) {
	float squareSize = size/2;
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
		point(-squareSize, squareSize, 0);
		point(squareSize, squareSize, 0);
		point(squareSize, -squareSize, 0);
		point(-squareSize, -squareSize, 0);
	glEnd();
	glLineWidth(1.0);
}

void drawSquare(float size, int withBorder) {
	float squareSize = size/2;
	glBegin(GL_QUADS);
		point(-squareSize, squareSize, 0);
		point(squareSize, squareSize, 0);
		point(squareSize, -squareSize, 0);
		point(-squareSize, -squareSize, 0);
	glEnd();
	
	if(withBorder) {
		float prev[4];
		squareSize = size/2;
		getPreviousColors(prev);
		color(0,0,0);
		drawWireSquare(size, 10);
		setColor(prev);
	}
}

void drawTexturedSquare(float size, int textureNumber, int withBorder) {
	float squareSize = size/2;
	glBindTexture ( GL_TEXTURE_2D, texture_id[textureNumber] );
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		point(-squareSize, squareSize, 0);
		glTexCoord2f(1.0f, 1.0f);
		point(squareSize, squareSize, 0);
		glTexCoord2f(1.0f, 0.0f);
		point(squareSize, -squareSize, 0);
		glTexCoord2f(0.0f, 0.0f);
		point(-squareSize, -squareSize, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	if(withBorder) {
		float prev[4];
		squareSize = size/2;
		getPreviousColors(prev);
		color(0,0,0);
		drawWireSquare(size, 7.5);
		setColor(prev);
	}
}

void drawTexturedCircle(float radius, int textureNumber) {
	glBindTexture ( GL_TEXTURE_2D, texture_id[textureNumber] );
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0, 0);	
		for (float i = 0.0; i <= 2 * PI; i += 0.001) {
			glTexCoord2f(cos(i) * radius, sin(i) * radius);
			point(cos(i) * radius, sin(i) * radius, 0);
		}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
		        
// Função callback chamada para fazer o desenho
void Desenha(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Limpa a janela e o depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 1.0f);

	// Desenha o teapot com a cor corrente (solid)
	glPushMatrix();
		// Tela
		glPushMatrix();
			color(1, 1, 1);
			glTranslatef(0, 15, 1.0);
			// Borda
			drawSquare(15, 1);
			
			// Quadro com Textura
			glPushMatrix();
				color(0, 1, 0);
				glTranslatef(0, 0, 0.5);
				drawTexturedSquare(13, textureNumber, 1);
				//drawTexturedCircle(8, textureNumber);
			glPopMatrix();
		glPopMatrix();
		
		// Armário
		glPushMatrix();
			color(0.6, 0.4, 0);
			glTranslatef(0, -10, 0);
			// Parte central
			drawSquare(20, 1);
			
			// Parte de cima
			glPushMatrix();
				glScalef(2.2, 0.2, 1);
				glTranslatef(0, 50, 1.0);
				drawSquare(10, 1);
			glPopMatrix();
			
			// Porta da direita
			glPushMatrix();
				color(0, 0, 0);
				glTranslatef(-5, -1, 1.0);
				glScalef(0.9, 1.7, 1);
				drawWireSquare(10, 5);
				
				// Maçaneta
				glPushMatrix();
					color(0.9, 0.1, 0.1);
					glTranslatef(3, 0, 0);
					drawCircle(0.75);
				glPopMatrix();
					
			glPopMatrix();
			
			// Porta da esquerda
			glPushMatrix();
				color(0, 0, 0);
				glTranslatef(5, -1, 1.0);
				glScalef(0.9, 1.7, 1);
				drawWireSquare(10, 5);
				
				// Maçaneta
				glPushMatrix();
					color(0.9, 0.1, 0.1);
					glTranslatef(-3, 0, 0);
					drawCircle(0.75);
				glPopMatrix();
					
			glPopMatrix();
			
			// Pé direito
			glPushMatrix();
				color(0.6, 0.4, 0);
				glTranslatef(-8, -12.5, 0);
				glScalef(0.2, 0.4, 1);
				drawSquare(10, 1);
			glPopMatrix();
			
			// Pé esquerdo
			glPushMatrix();
				color(0.6, 0.4, 0);
				glTranslatef(8, -12.5, 0);
				glScalef(0.2, 0.4, 1);
				drawSquare(10, 1);
			glPopMatrix();
			
		glPopMatrix();
		
		color(0.8, 0.8, 0.3);
		glScalef(1, 2, 1);
		drawSquare(30, 1);
	glPopMatrix();
	
	glutSwapBuffers();
}

// Inicializa par?metros de rendering
void Inicializa (void) { 

	// Especifica que a cor de fundo da janela
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
	// Habilida o blend
	glEnable(GL_BLEND);
	
	// Colorização Gouraund
	glShadeModel(GL_SMOOTH);	
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Carrega a matriz identidade
	glLoadIdentity();
	// Utiliza projeção ortogonal
	glOrtho(-50, 50, -50, 50, -20, 20);
	
	// Limpa os buffers
	glClear(GL_COLOR_BUFFER_BIT);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	EspecificaParametrosVisualizacao();
}

void Teclado(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
			exit(0);
			break;
		case '1':
			textureNumber = 0;
			break;
		case '2':
			textureNumber = 1;
			break;
		case '3':
			textureNumber = 2;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

/*
Funcao principal do programa.
*/
int main(int argc, char** argv)
{
	//Inicia a Telinha
	glutInit(&argc,argv);
	
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 900);
	glutCreateWindow("Atividade Textura");
	
	Inicializa();
	initTexture();
	
	glutDisplayFunc(Desenha);
	glutKeyboardFunc(Teclado);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMainLoop();
  return 0;
}

