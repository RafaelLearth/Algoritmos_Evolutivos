#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>
#include <sstream>
using namespace std;

std::stringstream ss;
    
#define windowWidth 1000
#define windowHeight 1000
#define nMaxgeracoes 100
//----------------------Variaveis RedeNeural
#define n_cam_entrada 3
#define n_cam_1 4
#define n_cam_2 4
#define n_cam_saida 3
// Opções do menu principal
#define RESET 0
#define NEWGAME 1
#define GAMEOVER 2
#define PAUSE 3
// Opções do menu de cor
#define RED 1
#define GREEN 2
#define BLUE 3
#define PINK 4
#define PURPLE 5
// Opções do menu de velocidade
#define INCREASE 1
#define DECREASE 2
// Opção de agressivo e passivo
#define AGRESSIVE 1
#define PASSIVE 0

//VARIAVEIS GLOBAIS
float clickx=0;
float clicky=0;
float TIME=0;
int behavior=0;
int wave=0;
bool pause;

int TamPop = 100;
int nMonstros=100;
double ind[100];
double fit[100];
int gen=0; //geracao
int idmelhor;
double TaxMut=0.05;


 
typedef struct RedeNeural{
  
  float w1[n_cam_entrada][n_cam_1];
  float w2[n_cam_1][n_cam_2];
  float w3[n_cam_2][n_cam_saida];
  RedeNeural(){
    for(int i=0;i<n_cam_entrada;i++){
        for(int j=0;j<n_cam_1;j++){
            w1[i][j]=rand()%101;
        }
    }
    for(int i=0;i<n_cam_1;i++){
        for(int j=0;j<n_cam_2;j++){
            w2[i][j]=rand()%101;
        }
    }
    for(int i=0;i<n_cam_2;i++){
        for(int j=0;j<n_cam_saida;j++){
            w3[i][j]=rand()%101;
        }
    }    
  }
  RedeNeural(float **w1,float **w2, float **w3){
    for(int i=0;i<n_cam_entrada;i++){
        for(int j=0;j<n_cam_1;j++){
            this->w1[i][j]=w1[i][j];
        }
    }
    for(int i=0;i<n_cam_1;i++){
        for(int j=0;j<n_cam_2;j++){
            this->w2[i][j]=w2[i][j];
        }
    }
    for(int i=0;i<n_cam_2;i++){
        for(int j=0;j<n_cam_saida;j++){
            this->w3[i][j]=w3[i][j];
        }
    }    
  }
}RedeNeural;

typedef struct _bixinho2{
  RedeNeural Rede;
  float radius;
  float x;
  float y;
  float theta;
  float r,g,b;
  bool exist;
  float vel;
  _bixinho2(float radius,float x,float y,float theta,float r,float g,float b,bool exist,float vel){
    this->radius=radius;
    this->x=x;
    this->y=y;
    this->theta=theta;
    this->r=r;
    this->g=g;
    this->b=b;
    this->exist=exist;
    this->vel=vel;
  }

}Monstrinho;

typedef struct _bixinho{
  float radius;
  float x;
  float y;
  float theta;
  float r,g,b;
  bool exist;
  float vel;
}Bixinho;

typedef struct _bala{
  float x;
  float y;
  float theta;
  float r,g,b;
  bool exist;
}Bala;

// Vetor de bixinhos e monstros
vector<Monstrinho> monstros;
vector<Monstrinho> individuos;
//Monstrinho melhor; //melhor individuo

Bixinho player =     {0.04,    0,0,M_PI/2,    0.8,0,0.8, true, 0.02};

// Estaremos utilizando um vector de bixinhos
vector<Bixinho> bixinhos;

vector <Bala> balas;
//-----------RedeNeural---------------------//
void initpop();
void Genocidio();
void jogar();
//---------- Protótipos de função ----------//
void draw();
void timer(int);
void mouse(int button, int state, int x, int y);// Função para receber os cliques do mouse
void rotateBixinho(Bixinho *bixinho, float angle);
void moveBixinho(Bixinho *bixinho, float distance);

void segueBixinho(Bixinho*bixinho,float distance);
void drawBixinho(Bixinho bixinho);

void circle(float x, float y, float radius);
void strangePolygon(float x, float y);
void anotherStrangePolygon(float x, float y);
void keyboard(unsigned char key, int x, int y);
void drawBala(Bala bala); //Desenhar bala
void moveBala(Bala *bala);// Mover bala
void mataBixinho();
void deletaBalas();
void gameover();

void tabelaInfo();
void displayText(string s,float dado, float x, float y);

// Handlers de menu
void menuInit();// Inicia os menus
void mainMenuHandler(int choice);// Menu principal
void colorMenuHandler(int choice);// Menu secundário
void velocityMenuHandler(int choice);// Menu secundário
void BehaviorMenuHandler(int choice);

//------------------ Main -----------------//
int main(int argc, char** argv){
  srand(42);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Versao 8");
  glClearColor(0.0, 1.0, 1.0, 1.0); //azul= 0.5 0.5 1.0 1.0
  glutDisplayFunc(draw);
  menuInit();// Inicializa o menu de seleção
  glutKeyboardFunc(keyboard); 
  glutTimerFunc(0, timer, 0);
  glutMouseFunc(mouse);// É chamada quando ocorre cliques na tela
  glutMainLoop();
    
  return 0;
}
//------------------ Menu -----------------//
void menuInit(){

  // Create the menu options
  int colorSubMenu = glutCreateMenu(colorMenuHandler);
  glutAddMenuEntry("Red", RED);
  glutAddMenuEntry("Green", GREEN);
  glutAddMenuEntry("Blue", BLUE);
  glutAddMenuEntry("Pink", PINK);
  glutAddMenuEntry("Purple", PURPLE);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  int velocitySubMenu = glutCreateMenu(velocityMenuHandler);
  glutAddMenuEntry("Increase", INCREASE);
  glutAddMenuEntry("Decrease", DECREASE);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  int BehaviorSubMenu= glutCreateMenu(BehaviorMenuHandler);
  glutAddMenuEntry("Passive", PASSIVE);
  glutAddMenuEntry("Agressive", AGRESSIVE);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutCreateMenu(mainMenuHandler);
  glutAddSubMenu("Color", colorSubMenu);
  glutAddSubMenu("Velocity", velocitySubMenu);
  glutAddSubMenu("Behavior",BehaviorSubMenu);
  glutAddMenuEntry("GameOver", GAMEOVER);
  glutAddMenuEntry("New Game",NEWGAME);
  glutAddMenuEntry("Reset", RESET);
  glutAddMenuEntry("Pause", PAUSE);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void BehaviorMenuHandler(int choice){
    behavior=choice;
}
void mainMenuHandler(int choice){
  if(choice == RESET){
    for(int i=0;i<int(bixinhos.size());i++){
        bixinhos[i].x=0;
        bixinhos[i].y=0;
    }
  }
  else if(choice == GAMEOVER){
    player.exist=0;
    while(int(bixinhos.size())!=0){
      for(int i=0;i<int(bixinhos.size());i++){
        bixinhos[i].exist=0;
        bixinhos.erase(bixinhos.begin()+i);
      }
    }
    while(int(balas.size())!=0){
      for(int i=0;i<int(balas.size());i++){
        balas[i].exist=0;
        balas.erase(balas.begin() + i);
      }
    }
  }else if(choice==NEWGAME){
    TIME=0;
    player.exist=1;
    player.x=0;
    player.y=0;
    while(int(bixinhos.size())!=0){
      for(int i=0;i<int(bixinhos.size());i++){
        bixinhos[i].exist=0;
        bixinhos.erase(bixinhos.begin()+i);
      }
    }
    while(int(balas.size())!=0){
      for(int i=0;i<int(balas.size());i++){
        balas[i].exist=0;
        balas.erase(balas.begin() + i);
      }
    }
  }else if(choice== PAUSE){
    if(pause==false){
        pause=true;
    }else{
        pause=false;
    }
  }
}

void colorMenuHandler(int choice) {
  // Muda a cor dos bixinhos para a selecionada
  float r=0,g=0,b=0;
	switch(choice) {
    case RED:
      r = 1;
      g = 0;
      b = 0;
      break;
		case GREEN:
      r = 0;
      g = 1;
      b = 0;
			break;
    case BLUE:
      r = 0;
      g = 0;
      b = 1;
			break;
    case PINK:
      r = 1;
      g = 0;
      b = 1;
			break;
    case PURPLE:
      r = 0.6;
      g = 0;
      b = 0.6;
			break;
	}
for(int i=0;i<int(bixinhos.size());i++){
        bixinhos[i].r=r;
        bixinhos[i].g=g;
        bixinhos[i].b=b;
    
    }
}

void velocityMenuHandler(int choice) {
  // Aumenta ou diminui a velocidade dos bixinhos
	switch(choice) {
    case INCREASE:
    for(int i=0;i<int(bixinhos.size());i++){
        bixinhos[i].vel*=1.2;
    }
    break;
	case DECREASE:
    for(int i=0;i<int(bixinhos.size());i++){
        bixinhos[i].vel*=0.8;
    }
	break;
	}
}



void drawBala(Bala bala){
  if(bala.exist){
    float radius=0.01;
    float x=bala.x;
    float y=bala.y;
  
    glColor3f(bala.r, bala.g, bala.b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i+=5) {
      glVertex2d( radius*cos(i/180.0*M_PI) + x, radius*sin(i/180.0*M_PI) + y);
    }
    glEnd();

  }
  
}
void moveBala(Bala *bala){
    // Para mover para onde ele está olhando (na direção theta)
  bala->x = bala->x + 0.05*cos(bala->theta);
  bala->y = bala->y + 0.05*sin(bala->theta);

  // Impede que o wilson saia da tela
  bala->x = bala->x>1 ? bala->exist=false: bala->x;
  bala->y = bala->y>1 ? bala->exist=false: bala->y;

}

//------------------ KEYBOARD -----------------//
void keyboard(unsigned char key, int x, int y){
    //W
  if(key==119){
    moveBixinho(&player,0.02);
  }
  //D
  if(key==97){
    rotateBixinho(&player,0.1); 
  }
  //S
  if(key==115){
    moveBixinho(&player,-0.02);
  }
  //A
  if(key==100){
    rotateBixinho(&player,-0.1);
  }
  if(key==32){
    balas.push_back({player.x,player.y,player.theta,player.r,player.g,player.b,true});
  }

  //TECLA PRESSIONADA:tecla pressionada aparece no terminal
  //printf("%d\n",key); 
}
void mataBixinho(){
  
  for(int i=0;i<int(balas.size());i++)
  {
    for(int j=0;j<int(bixinhos.size());j++){
      float dx=balas[i].x-bixinhos[j].x;
      float dy=balas[i].y-bixinhos[j].y;
    
      float dist=sqrt(dx*dx+dy*dy);
      if((dist <= bixinhos[j].radius+0.02f) && bixinhos[j].exist==true && balas[i].exist==true){
        bixinhos[j].exist=false;
        balas[i].exist=false;
        balas.erase(balas.begin() + i);
        bixinhos.erase(bixinhos.begin()+j);
      }

      //if(bixinhos[j].exist==false){
      //  bixinhos.erase(bixinhos.begin() + j);
      //}

    }
    
  }
}
void deletaBalas(){
  for(int i=0;i<int(balas.size());i++){
    if(balas[i].x >= 0.98f || balas[i].x <= -0.98f || balas[i].y >= 0.98f || balas[i].y <= -0.98f){
      balas[i].exist=false;
    }
    if(balas[i].exist==false){
      balas.erase(balas.begin() + i);
    }
  }
}


//------------------ Draw -----------------//
void draw(){
  glClear(GL_COLOR_BUFFER_BIT);
    for(int i=0;i<nMaxgeracoes;i++){
        wave=0;
        initpop();
        for(int j=0;j<4;j++){
            jogar();
            wave++;
            //jogar();
            //avalia();
            //elitismo();
            //printf("Melhor eh individuo:%d, valor:%f\n",idmelhor,melhor);
        }
    }   
  
}
void jogar(){
    if(TIME < 3000){
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
//------------------ Timer -----------------//
void timer(int){
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

//------------------ Mouse -----------------//
void mouse(int button, int state, int x, int y)
{
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

//----------------------------------------------//
//----------- Funções para o bixinho -----------//
//----------------------------------------------//
void rotateBixinho(Bixinho *bixinho, float angle){
    bixinho->theta+=angle;
}

void moveBixinho(Bixinho *bixinho, float distance){
  // Para mover para onde ele está olhando (na direção theta)
  bixinho->x = bixinho->x + distance*cos(bixinho->theta);
  bixinho->y = bixinho->y + distance*sin(bixinho->theta);
}
void segueBixinho(Bixinho*bixinho, float distance){
    float dx=bixinho->x-player.x;
    float dy=bixinho->y-player.y;
    float dist=sqrt(dx*dx+dy*dy);

    if(dist< (bixinho->radius + player.radius)){
        bixinho->x=bixinho->x;
        bixinho->y=bixinho->y;
    }else{
        //if(atan(dy/dx)!=0){
        //    bixinho->theta=atan(dy/dx);
        //    bixinho->x = bixinho->x + distance*cos(bixinho->theta);
        //    bixinho->y = bixinho->y + distance*sin(bixinho->theta);
            
        //}else{
        //    bixinho->theta=atan(dy/dx);
        //    bixinho->x = bixinho->x + distance*dy/dist;
        //    bixinho->y = bixinho->y + distance*dx/dist;
            
        //}
        bixinho->theta=atan(dy/dx);
        if(dx>=0){
          bixinho->x = bixinho->x - distance*cos(bixinho->theta);
          bixinho->y = bixinho->y - distance*sin(bixinho->theta);
        }else{
          bixinho->x = bixinho->x + distance*cos(bixinho->theta);
          bixinho->y = bixinho->y + distance*sin(bixinho->theta);
        }
        
        //bixinho->x = bixinho->x>1 ? -1 : bixinho->x;
        //bixinho->y = bixinho->y>1 ? -1 : bixinho->y;
        
    }

}
void drawBixinho(Bixinho bixinho){
if(bixinho.exist){
  // Função para desenhar o bixinho
    float radius = bixinho.radius;
    float x = bixinho.x;
    float y = bixinho.y;
    float theta = bixinho.theta;
    //----- Desenha corpo do bixinho -----//
    glColor3f(bixinho.r, bixinho.g, bixinho.b);// Bixinho verde
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i+=5) {
      glVertex2d( radius*cos(i/180.0*M_PI) + x, radius*sin(i/180.0*M_PI) + y);
    }
    glEnd();

    //----- Desenha olho direito do bixinho -----//
    float eyeRadius = radius/8;
    float eyeDist = M_PI/6;

    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i+=5) {
      float shiftX = radius/2*cos(theta-eyeDist);
      float shiftY = radius/2*sin(theta-eyeDist);
      glVertex2d( eyeRadius*cos(i/180.0*M_PI) + x + shiftX, eyeRadius*sin(i/180.0*M_PI) + y + shiftY);
    }
    glEnd();

    //----- Desenha olho esquerdo do bixinho -----//
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i+=5) {
      float shiftX = radius/2*cos(theta+eyeDist);
      float shiftY = radius/2*sin(theta+eyeDist);

      glVertex2d( eyeRadius*cos(i/180.0*M_PI) + x + shiftX, eyeRadius*sin(i/180.0*M_PI) + y + shiftY);
    }
    glEnd();
  } 
  
}


//------------------------------//
//----------- CIRCLE -----------//
//------------------------------//

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

//------------------------------//
//----------- STRANGE ----------//
//------------------------------//
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


//Rede Neural
void initpop(){
    Genocidio();

    for(int i=0;i<TamPop;i++){
      float radius = 0.01 + (rand()%10)/1000.0f;
      float theta = (rand()%628)/100.0f;
      float r = (rand()%100)/100.0f;
      float g = (rand()%100)/100.0f;
      float b = (rand()%100)/100.0f;
      float v= 0.005;
      
      Monstrinho newBixinho = {radius, 0.0, 0.0, theta, r, g, b,true,v};
      individuos.push_back(newBixinho);
    }
    for(int i=0;i<nMonstros;i++){
      float radius = 0.01 + (rand()%10)/1000.0f;
      float theta = (rand()%628)/100.0f;
      float r = (rand()%100)/100.0f;
      float g = (rand()%100)/100.0f;
      float b = (rand()%100)/100.0f;
      float v= 0.005;
      float x;
      float y;
      switch(wave){
        case 0: x=0.85f+(rand()%10/100)-0.2;y=0.85f+(rand()%10/100)-0.2;
        break;
        case 1: x=-0.85f+(rand()%10/100)-0.2;y=0.85f+(rand()%10/100)-0.2;
        break;
        case 2: x=-0.85f+(rand()%10/100)-0.2;y=-0.85f+(rand()%10/100)-0.2;
        break;
        case 3: x=0.85f+(rand()%10/100)-0.2;y=-0.85f+(rand()%10/100)-0.2;
        break;
        default: x=(rand()%10/100)-0.2;y=(rand()%10/100)-0.2;
        break;
      }
      Monstrinho newBixinho = {radius, x, y, theta, r, g, b,true,v};
      monstros.push_back(newBixinho);
    }
    TIME=0;
}
//MataTodos
void Genocidio(){
    while(int(monstros.size())!=0){
      for(int i=0;i<int(monstros.size());i++){
        monstros[i].exist=0;
        monstros.erase(monstros.begin()+i);
      }
    }
    while(int(individuos.size())!=0){
      for(int i=0;i<int(individuos.size());i++){
        individuos[i].exist=0;
        individuos.erase(individuos.begin()+i);
      }
    }
    while(int(balas.size())!=0){
      for(int i=0;i<int(balas.size());i++){
        balas[i].exist=0;
        balas.erase(balas.begin() + i);
      }
    }
}
