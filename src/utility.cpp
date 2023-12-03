
#include "utility.h"
#include "creature.h"
#include "config.h"

float clickx = 0.0;
float clicky = 0.0;
float TIME = 0.0;
int behavior = 0;
int wave = 0;
bool pause;
unordered_set<unsigned char> pressedKeys;

void draw(){
  glClear(GL_COLOR_BUFFER_BIT);
  if(player.exist==true){
	glClearColor(1.0, 1.0, 1.0, 1.0); //azul= 0.5 0.5 1.0 1.0

  //  circle(-0.4, -0.5, 0.2);
  //  circle(0.4, 0.5, 0.3);
  //  circle(0.4, -0.5, 0.3);
	for(double i=-0.9;i<1;i=i+0.01){
	  circle(i,0.0,0.005);
	  circle(0.0,i,0.005);
	}
  //  strangePolygon(0, 0);
  //  anotherStrangePolygon(0.5, 0.5);

	drawBixinho(player);
	// Desenha cada bixinho do vetor
	for(int i=0;i<int(bixinhos.size());i++){
	  drawBixinho(bixinhos[i]);
	}
	for(int i=0;i<int(balas.size());i++){
	  drawBala(balas[i]);
	}
	tabelaInfo();
  }else{
	glClearColor(0.0, 0.0, 0.0, 0.0); //azul= 0.5 0.5 1.0 1.0
	gameover();
  }

  glutSwapBuffers();
}

void timer(int){
	updateKeyboard();
  // Move cada bixinho do vetor
  if(pause==false){
	TIME++;
	if(behavior==0){
		for(int i=0;i<int(bixinhos.size());i++){
			moveBixinho(&bixinhos[i], bixinhos[i].vel);
			rotateBixinho(&bixinhos[i], 0.02);
		}
	}else if(behavior==1){
		for(int i=0;i<int(bixinhos.size());i++){
			segueBixinho(&bixinhos[i],bixinhos[i].vel);
		}
	}
	//Move balas
	for(int i=0;i<int(balas.size());i++){
		moveBala(&balas[i]);
	}

	mataBixinho();
	deletaBalas();
  }
  glutPostRedisplay();
  glutTimerFunc(1000/60, timer, 0);
}

void mouse(int button, int state, int x, int y){
  if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
  {
	float mouseX, mouseY;
	// Converte a entrada para pontos (x,y) na tela
	mouseX = 2*((float(x)/windowWidth)-0.5);
	mouseY = -2*((float(y)/windowHeight)-0.5);

	clickx=mouseX;
	clicky=mouseY;

	for(int i=0;i<50;i++){
	  float radius = 0.01 + (rand()%10)/1000.0f;
	  float theta = (rand()%628)/100.0f;
	  float r = (rand()%100)/100.0f;
	  float g = (rand()%100)/100.0f;
	  float b = (rand()%100)/100.0f;
	  float v= 0.005;
	  Bixinho newBixinho = {radius, mouseX, mouseY, theta, r, g, b,true,v};

	  bixinhos.push_back(newBixinho);

	}
  }
}

void keyDown(unsigned char key, int x, int y) {
	pressedKeys.insert(key);
}

void keyUp(unsigned char key, int x, int y) {
	pressedKeys.erase(key);
}

void updateKeyboard() {
	for (unsigned char key : pressedKeys) {
		switch (key) {
			case 119: // W
				moveBixinho(&player, 0.02);
				break;
			case 97: // A
				rotateBixinho(&player, 0.1);
				break;
			case 115: // S
				moveBixinho(&player, -0.02);
				break;
			case 100: // D
				rotateBixinho(&player, -0.1);
				break;
			case 32: // Space
				balas.push_back({player.x, player.y, player.theta, player.r, player.g, player.b, true});
				break;
			// Adicione mais casos conforme necessário
		}
	}
}

void circle(float x, float y, float radius){
  // Um circulo é desenhado como um polígono de muitos pontos

  glColor3f(0.5, 0.2, 0);// Define que será desenhar um objeto verde
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=5) {
	// Adicionada cada vértice 2D do circulo
	glVertex2d( radius*cos(i/180.0*M_PI) + x, radius*sin(i/180.0*M_PI) + y);
  }
  glEnd();
}

void strangePolygon(float x, float y){

  glColor3f(0.5, 0.2, 0);// Define que será desenhar um objeto preto

  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=10) {
	glVertex2d( 0.6*cos(i/180.0*M_PI) + x + cos(i)*0.15,  0.9*sin(i/180.0*M_PI) + y + sin(i)*0.15);
  }
  glEnd();

}

void anotherStrangePolygon(float x, float y){
  glColor3f(0.5, 0.2, 0);// Define que será desenhar um objeto amarelo

  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=10) {
	glVertex2d( 0.3*cos(i/180.0*M_PI) + x,  0.3*sin(i/180.0*M_PI) + y);
	i+=10;
	glVertex2d( 0.1*cos(i/180.0*M_PI) + x,  0.1*sin(i/180.0*M_PI) + y);
  }
  glEnd();
}

void displayText(string text1,float dado, float x, float y){
	glColor3f(0.0f, 0.0f, 0.0f); //cor
	glRasterPos2f(x, y); // posicao

	// Convert the data to a string
	std::stringstream ss;
	ss << text1 << dado;
	std::string combinedText = ss.str();

	for (const char &c : combinedText) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

}

void tabelaInfo(){

	displayText("Versao ",8,-1.0f,0.95f);
	displayText("Populacao:",0,-1.0f,0.9f);
	displayText("Indivíduos Vivos:",bixinhos.size(),-1.0f,0.85f);
	displayText("Melhor Individuo:",0,-1.0f,0.8f);
	displayText("Rede Neural do melhor inviduo:",0,-1.0f,0.75f);
	displayText("Numero de balas:",balas.size(),-1.0f,0.7f);
	displayText("Velocidade:",bixinhos.size()>0?bixinhos[0].vel:0,-1.0f,0.65f);
	displayText("Click foi feito em x:",clickx,-1.0f,0.60f);
	displayText("Click foi feito em y:",clicky,-1.0f,0.55f);
	displayText("Wave ",wave, 0.75f,0.95f);
	displayText("Tempo:",TIME,0.75f,0.90f);
}

void gameover(){
  glColor3f(1.0f, 0.0f, 0.0f); //cor
  glRasterPos2f(-0.2, 0.0); // posicao
  string gameover= "G a m e   O v e r";
  for (const char &c : gameover) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}