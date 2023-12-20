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
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

std::stringstream ss;
    
#define windowWidth 1000
#define windowHeight 1000


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
int TIME=0;
int behavior=1;
int wave=0;
bool pause;
int tipo=1;
int populacao=1;
int IndexMelhor;
// Estrutura de um neurônio
struct Neuron {
    double value;
    vector<double> weights;
};

void initialize_weights(Neuron &neuron, int num_weights);

struct BixinhoEvolutivo {
    float radius;
    float x, y;
    float theta;
    float r, g, b;
    bool exist;
    float vel;
    float pontuacao;
    vector<vector<Neuron>> hidden_layers; // Múltiplas camadas ocultas
    vector<Neuron> output_layer;

    // Construtor para inicializar o tamanho dos vetores de neurônios
    BixinhoEvolutivo(size_t num_inputs, size_t num_hidden_neurons, size_t num_hidden_layers, size_t num_outputs) {
        radius = 0.04;
        x = -0.2 + (rand() % 400) / 1000.0f;
        y = -0.2 + (rand() % 400) / 1000.0f;
        theta = M_PI / 2;
        r = 0.8;
        g = 0;
        b = 0.8;
        exist = true;
        vel = 0.02;
        pontuacao = 0;

        // Inicializar as camadas ocultas
        hidden_layers.resize(num_hidden_layers);
        for (auto &layer : hidden_layers) {
            layer.resize(num_hidden_neurons); // Cada camada tem um número específico de neurônios
            for (auto &neuron : layer) {
                initialize_weights(neuron, num_inputs + 1); // +1 para o bias
            }
            num_inputs = num_hidden_neurons; // O número de entradas para a próxima camada é o número de neurônios na camada atual
        }

        // Inicializar a camada de saída
        output_layer.resize(num_outputs);
        for (auto &neuron : output_layer) {
            initialize_weights(neuron, num_hidden_neurons + 1); // +1 para o bias
        }
    }
};

//Variaveis globais
int num_inputs = 6;
int num_hidden_neurons = 4;
int num_outputs = 5;
int num_hidden_layers = 2;
BixinhoEvolutivo melhor(num_inputs, num_hidden_neurons, num_hidden_layers, num_outputs);


typedef struct _bixinho{
  float radius;
  float x;
  float y;
  float theta;
  float r,g,b;
  bool exist;
  float vel;
}Bixinho;
//
typedef struct _bala{
  float x;
  float y;
  float theta;
  float r,g,b;
  bool exist;
  int index;
}Bala;

Bixinho player =     {0.04,    0,0,M_PI/2,    0.8,0,0.8, true, 0.02};

// Estaremos utilizando um vector de bixinhos
vector<Bixinho> bixinhos;
vector<BixinhoEvolutivo>ListaEvolutiva;
vector <Bala> balas;

//---------- Protótipos de função ----------//
void draw();
void timer(int);
void mouse(int button, int state, int x, int y);// Função para receber os cliques do mouse
void rotateBixinho(Bixinho *bixinho, float angle);
void moveBixinho(Bixinho *bixinho, float distance);
void criaMonstro(int tipo);
void segueBixinho(Bixinho*bixinho,float distance);
void drawBixinho(Bixinho bixinho);

//EVOLUTIVAS
void drawBixinhoEvo(BixinhoEvolutivo bixinho);
void rotateBixinhoEvo(BixinhoEvolutivo *bixinho, float angle);
void moveBixinhoEvo(BixinhoEvolutivo *bixinho, float distance);

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

/*int Save() {
  FILE *arq = fopen("ArquivoEvolutivoSave1.bin", "wb");
  if (!arq) {
      printf("Erro ao abrir o arquivo para escrita\n");
      return 0;
  }

  // Salvando variáveis globais
  fwrite(&populacao, sizeof(int), 1, arq);
  fwrite(&IndexMelhor, sizeof(int), 1, arq);
  // Adicione aqui a escrita de outras variáveis globais, se necessário

  // Salvando o tamanho da lista evolutiva
  int sizeListaEvolutiva = ListaEvolutiva.size();
  fwrite(&sizeListaEvolutiva, sizeof(int), 1, arq);

  // Salvando os Bixinhos Evolutivos
  for (const auto& bixinho : ListaEvolutiva) {
      // Salvando dados do Bixinho Evolutivo
      fwrite(&bixinho.radius, sizeof(float), 1, arq);
      fwrite(&bixinho.x, sizeof(float), 1, arq);
      fwrite(&bixinho.y, sizeof(float), 1, arq);
      fwrite(&bixinho.theta, sizeof(float), 1, arq);
      fwrite(&bixinho.r, sizeof(float), 1, arq);
      fwrite(&bixinho.g, sizeof(float), 1, arq);
      fwrite(&bixinho.b, sizeof(float), 1, arq);
      fwrite(&bixinho.exist, sizeof(bool), 1, arq);
      fwrite(&bixinho.vel, sizeof(float), 1, arq);
      fwrite(&bixinho.pontuacao, sizeof(float), 1, arq);

      // Salvando a hidden_layer
      int sizeHiddenLayer = bixinho.hidden_layer.size();
      fwrite(&sizeHiddenLayer, sizeof(int), 1, arq);
      for (const auto& neuron : bixinho.hidden_layer) {
          int sizeWeights = neuron.weights.size();
          fwrite(&sizeWeights, sizeof(int), 1, arq);
          for (const auto& weight : neuron.weights) {
              fwrite(&weight, sizeof(double), 1, arq);
          }
      }

      // Salvando a output_layer
      int sizeOutputLayer = bixinho.output_layer.size();
      fwrite(&sizeOutputLayer, sizeof(int), 1, arq);
      for (const auto& neuron : bixinho.output_layer) {
          int sizeWeights = neuron.weights.size();
          fwrite(&sizeWeights, sizeof(int), 1, arq);
          for (const auto& weight : neuron.weights) {
              fwrite(&weight, sizeof(double), 1, arq);
          }
      }
  }

  fclose(arq);
  return 1;
}
int Load() {
  FILE *arq = fopen("ArquivoEvolutivoSave1.bin", "rb");
  if (!arq) {
      printf("Erro ao abrir o arquivo para leitura\n");
      return 0;
  }

  // Lendo variáveis globais
  fread(&populacao, sizeof(int), 1, arq);
  fread(&IndexMelhor, sizeof(int), 1, arq);
  // Adicione aqui a leitura de outras variáveis globais, se necessário

  // Lendo o tamanho da lista evolutiva
  int sizeListaEvolutiva;
  fread(&sizeListaEvolutiva, sizeof(int), 1, arq);

  ListaEvolutiva.clear(); // Limpar a lista atual antes de carregar os novos dados

  // Lendo os Bixinhos Evolutivos
  for (int i = 0; i < sizeListaEvolutiva; ++i) {
      BixinhoEvolutivo bixinho(num_hidden_neurons, num_outputs); // Cria um novo BixinhoEvolutivo

      // Lendo dados do Bixinho Evolutivo
      fread(&bixinho.radius, sizeof(float), 1, arq);
      fread(&bixinho.x, sizeof(float), 1, arq);
      fread(&bixinho.y, sizeof(float), 1, arq);
      fread(&bixinho.theta, sizeof(float), 1, arq);
      fread(&bixinho.r, sizeof(float), 1, arq);
      fread(&bixinho.g, sizeof(float), 1, arq);
      fread(&bixinho.b, sizeof(float), 1, arq);
      fread(&bixinho.exist, sizeof(bool), 1, arq);
      fread(&bixinho.vel, sizeof(float), 1, arq);
      fread(&bixinho.pontuacao, sizeof(float), 1, arq);

      // Lendo a hidden_layer
      int sizeHiddenLayer;
      fread(&sizeHiddenLayer, sizeof(int), 1, arq);
      bixinho.hidden_layer.resize(sizeHiddenLayer);
      for (auto& neuron : bixinho.hidden_layer) {
          int sizeWeights;
          fread(&sizeWeights, sizeof(int), 1, arq);
          neuron.weights.resize(sizeWeights);
          for (auto& weight : neuron.weights) {
              fread(&weight, sizeof(double), 1, arq);
          }
      }

      // Lendo a output_layer
      int sizeOutputLayer;
      fread(&sizeOutputLayer, sizeof(int), 1, arq);
      bixinho.output_layer.resize(sizeOutputLayer);
      for (auto& neuron : bixinho.output_layer) {
          int sizeWeights;
          fread(&sizeWeights, sizeof(int), 1, arq);
          neuron.weights.resize(sizeWeights);
          for (auto& weight : neuron.weights) {
              fread(&weight, sizeof(double), 1, arq);
          }
      }

      ListaEvolutiva.push_back(bixinho); // Adiciona o BixinhoEvolutivo carregado à lista
  }

  fclose(arq);
  return 1;
}*/

void SelecionaMelhor(){
  bool escolheuNovoMelhor = false;
  for(int i=0;i<(int)ListaEvolutiva.size();i++){
    if(ListaEvolutiva[i].pontuacao>=melhor.pontuacao){
      IndexMelhor=i;
      escolheuNovoMelhor = true;
    }
  }
  if(escolheuNovoMelhor) {
    melhor=ListaEvolutiva[IndexMelhor];
  }  
}
int QuantidadeVivos(){
  int vivos=0;
    for(int i=0;i<(int)ListaEvolutiva.size();i++){
      if(ListaEvolutiva[i].exist==true){
        vivos++;
      }
    }
    return vivos;
}
void Reproducao(BixinhoEvolutivo &player, const BixinhoEvolutivo &melhor) {
    // Certifique-se de que o tamanho das camadas ocultas do 'player' e do 'melhor' são iguais
    if (player.hidden_layers.size() != melhor.hidden_layers.size()) {
        return; // Encerrar a função se os tamanhos não coincidem
    }

    for (size_t l = 0; l < player.hidden_layers.size(); ++l) {
        if (player.hidden_layers[l].size() != melhor.hidden_layers[l].size()) {
            continue; // Pula para a próxima camada se os tamanhos não coincidem
        }

        for (size_t i = 0; i < player.hidden_layers[l].size(); ++i) {
            if (player.hidden_layers[l][i].weights.size() != melhor.hidden_layers[l][i].weights.size()) {
                continue; // Pula para o próximo neurônio se os tamanhos não coincidem
            }

            for (size_t j = 0; j < player.hidden_layers[l][i].weights.size(); ++j) {
                // Adiciona aleatoriedade na média dos pesos do 'player' e do 'melhor'
                player.hidden_layers[l][i].weights[j] = (0.2 * ((double)rand() / RAND_MAX)) + 0.8 * (0.3 * player.hidden_layers[l][i].weights[j] + 0.7 * melhor.hidden_layers[l][i].weights[j]);
            }
        }
    }

    // Média aritmética dos pesos na camada de saída (output_layer)
    for (size_t i = 0; i < player.output_layer.size(); ++i) {
        if (player.output_layer[i].weights.size() != melhor.output_layer[i].weights.size()) {
            continue; // Pula para o próximo neurônio se os tamanhos não coincidem
        }

        for (size_t j = 0; j < player.output_layer[i].weights.size(); ++j) {
            // Adiciona aleatoriedade na média dos pesos do 'player' e do 'melhor'
            player.output_layer[i].weights[j] = (0.2 * ((double)rand() / RAND_MAX)) + 0.8 * (0.3 * player.output_layer[i].weights[j] + 0.7 * melhor.output_layer[i].weights[j]);
        }
    }
}

// Função de ativação sigmoide
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

// Derivada da função sigmoide
double sigmoid_derivative(double x) {
    return x * (1 - x);
}

// Inicializa os pesos de um neurônio com valores aleatórios
void initialize_weights(Neuron &neuron, int num_weights) {
    neuron.weights.resize(num_weights);
    for (auto &weight : neuron.weights) {
        weight = ((double)rand() / RAND_MAX)*2 - 1; // Valores aleatórios entre -1 e 1
    }
}

// Propaga um único neurônio
double propagate(Neuron &neuron, const vector<double> &inputs) {
    double activation = neuron.weights.back(); // bias
    for (size_t i = 0; i < inputs.size(); ++i) {
        activation += neuron.weights[i] * inputs[i];
    }
    return activation;
}

/*void Inicializador(BixinhoEvolutivo &player){
    // Inicializar pesos
    for (auto &neuron : player.hidden_layer) {
        initialize_weights(neuron, num_inputs + 1); // +1 para o bias
    }
    for (auto &neuron : player.output_layer) {
        initialize_weights(neuron, num_hidden_neurons + 1); // +1 para o bias
    }
    
}*/

vector<double> OutputLayer(BixinhoEvolutivo &player, vector<double>& inputs) {
    vector<double> current_outputs = inputs;

    if(TIME%30==0) Reproducao(player, player);

    for (auto &layer : player.hidden_layers) {
        vector<double> next_outputs(layer.size());
        for (size_t i = 0; i < layer.size(); ++i) {
            next_outputs[i] = sigmoid(propagate(layer[i], current_outputs));
        }
        current_outputs = next_outputs;
    }

    // Propagação na camada de saída
    vector<double> outputs(player.output_layer.size());
    for (size_t i = 0; i < player.output_layer.size(); ++i) {
        outputs[i] = sigmoid(propagate(player.output_layer[i], current_outputs));
    }

    return outputs;
}


//------------------ Main -----------------//
int main(int argc, char** argv){
  srand((unsigned)time(NULL)); // Inicialização da semente aleatória
  //if(Load()!=1){
    for(int i=0;i<10;i++){
      BixinhoEvolutivo individuo(num_inputs, num_hidden_neurons, num_hidden_layers, num_outputs);
      ListaEvolutiva.push_back(individuo);
    }
  //}
    
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Versao 12");
  glClearColor(0.0, 1.0, 1.0, 1.0); //azul= 0.5 0.5 1.0 1.0
  glutDisplayFunc(draw);
  menuInit();// Inicializa o menu de seleção
  //glutKeyboardFunc(keyboard); 
  glutTimerFunc(0, timer, 0);
  glutMouseFunc(mouse);// É chamada quando ocorre cliques na tela
  criaMonstro(tipo);
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
/*void keyboard(unsigned char key, int x, int y){
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
}*/
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
        ListaEvolutiva[balas[i].index].pontuacao+=500;//kill:20bichinhos=2000 tempo do jogo=250s, time*10=2500
        balas.erase(balas.begin() + i);
        bixinhos.erase(bixinhos.begin()+j);
      }
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
  if(player.exist==true){
    glClearColor(1.0, 1.0, 1.0, 1.0); //azul= 0.5 0.5 1.0 1.0
    for(double i=-1;i<1.01;i=i+0.01){
      circle(i,0.0,0.005);
      circle(0.0,i,0.005);
    }
    for(int i=0;i<(int)ListaEvolutiva.size();i++){
      drawBixinhoEvo(ListaEvolutiva[i]);
    }

    //drawBixinho(player);
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
    vector<int>QtdBalas(ListaEvolutiva.size());
    if((int)TIME%100){
      for(int i=0;i<(int)ListaEvolutiva.size();i++){
        QtdBalas[i]+=50;
      }
    }
    if(((int)TIME%250)==0 || QuantidadeVivos()==0){
      
      TIME=0;
      bixinhos.clear();
      for(int i=0;i<(int)ListaEvolutiva.size();i++){
        if(ListaEvolutiva[i].exist==true){
          ListaEvolutiva[i].pontuacao+=1250;
        } else {
          ListaEvolutiva[i].pontuacao-=1000;
        }
      }
      
      tipo++;
      wave++;
      
      if(tipo==5){
        tipo=1;
      }
      if(wave==5){
        SelecionaMelhor();
        for(int i=0;i<(int)ListaEvolutiva.size();i++){
          if(i!=IndexMelhor){
            Reproducao(ListaEvolutiva[i], melhor);
          }
          ListaEvolutiva[i].exist=true;
          ListaEvolutiva[i].x=-0.2+(rand()%400)/1000.0f;
          ListaEvolutiva[i].y=-0.2+(rand()%400)/1000.0f;
          QtdBalas[i]=100;
          ListaEvolutiva[i].pontuacao=0;
        }
        wave=1;
        populacao++;
      }else{
        SelecionaMelhor();
        for(int i=0;i<(int)ListaEvolutiva.size();i++){
          ListaEvolutiva[i].exist=true;
          QtdBalas[i]=500;
          ListaEvolutiva[i].x=-0.2+(rand()%400)/1000.0f;
          ListaEvolutiva[i].y=-0.2+(rand()%400)/1000.0f;
        }
      }
      criaMonstro(tipo);
      
    }
    float menordx[(int)ListaEvolutiva.size()] = {999.0};
    float menordy[(int)ListaEvolutiva.size()] = {999.0};
    
    if(bixinhos.size()!=0){
      float dx[(int)ListaEvolutiva.size()][(int)bixinhos.size()];
      float dy[(int)ListaEvolutiva.size()][(int)bixinhos.size()];
      
      for(int i=0;i<(int)ListaEvolutiva.size();i++){
        for (int j = 0; j < (int)bixinhos.size(); j++) {
          dx[i][j] = bixinhos[j].x - ListaEvolutiva[i].x;
          dy[i][j] = bixinhos[j].y - ListaEvolutiva[i].y;
          if(dx[i][j] * dy[i][j] < menordy[i] * menordx[i]){
            menordy[i] = dy[i][j];
            menordx[i] = dx[i][j];
          }
        }
      }
    }
    //double maior;
    vector<int> actions;
    for(int i=0;i<(int)ListaEvolutiva.size();i++){
      if(ListaEvolutiva[i].exist){
        vector<double> inputs = {(double)bixinhos.size(), menordx[i], menordy[i], ListaEvolutiva[i].x, ListaEvolutiva[i].y, ListaEvolutiva[i].theta};
          //maior=0;
          if(i == 0) cout << "Output"<< endl;
          vector<double> output = OutputLayer(ListaEvolutiva[i],inputs);
          // Defina limiares para ações opostas
          for (size_t j = 0; j < output.size(); ++j) {
            if(i == 0)  cout << output[j]<<endl;
            if(output[j] > 0.9) {
            // Verifique se o número oposto (1 para 0 e 3 para 4) não está no vetor
              if((j == 1 && std::find(actions.begin(), actions.end(), 0) == actions.end()) ||               
                 (j == 4 && std::find(actions.begin(), actions.end(), 3) == actions.end())) {
                  actions.push_back(j);
              } else {
                  // Para os números 2, apenas insira
                  actions.push_back(j);
              }
            }
          }
          for(int action : actions) {
            switch(action){
          case 0:
            moveBixinhoEvo(&ListaEvolutiva[i],0.02);
            break;
          case 1:
            moveBixinhoEvo(&ListaEvolutiva[i],-0.02);  
            break;
          case 2:
            if(QtdBalas[i]>0 && TIME%2 == 0){
              balas.push_back({ListaEvolutiva[i].x,ListaEvolutiva[i].y,ListaEvolutiva[i].theta,ListaEvolutiva[i].r,ListaEvolutiva[i].g,ListaEvolutiva[i].b,true,i});
              QtdBalas[i]--;
            }
            break;
          case 3:
            rotateBixinhoEvo(&ListaEvolutiva[i],0.1);
            break;
          case 4:
            rotateBixinhoEvo(&ListaEvolutiva[i],-0.1);
            break;
        }
          }
        
      }
    }
    //Move balas
    for(int i=0;i<int(balas.size());i++){
        moveBala(&balas[i]);
    }
    float x,y,dist;
    for(int j=0;j<(int)ListaEvolutiva.size();j++){
      for(int i=0;i<int(bixinhos.size());i++){
        x=(bixinhos[i].x)-ListaEvolutiva[j].x;
        y=(bixinhos[i].y)-ListaEvolutiva[j].y;
        dist=sqrt(x*x+y*y);
        if(dist <= bixinhos[i].radius+ListaEvolutiva[j].radius && ListaEvolutiva[j].exist==true){
          ListaEvolutiva[j].exist=false;
          ListaEvolutiva[j].pontuacao+=TIME*5;
          
        }
      }
    }
    
    mataBixinho();
    deletaBalas();
    /*if(populacao%50==0){
      Save();
    }*/
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
void criaMonstro(int tipo){
  float X,Y;
  for(int i=0;i<1000;i++){
    switch(tipo){
      case 1:
      X=0.4+(rand()%300)/1000.0f;//(0.4:0.6)
      Y=0.4+(rand()%300)/1000.0f;
      break;
      case 2:
      X=0.4+(rand()%300)/1000.0f;
      Y=-0.4-(rand()%300)/1000.0f;//(-0.4:-0.6)
      break;
      case 3:
      X=-0.4-(rand()%300)/1000.0f;
      Y=0.4+(rand()%300)/1000.0f;
      break;
      case 4:
      X=-0.4-(rand()%300)/1000.0f;
      Y=-0.4-(rand()%300)/1000.0f;
      break;
    }    
    float radius = 0.01 + (rand()%10)/1000.0f;
    float theta = (rand()%628)/100.0f;
    float r = (rand()%100)/100.0f;
    float g = (rand()%100)/100.0f;
    float b = (rand()%100)/100.0f;
    float v= 0.02;
    Bixinho newBixinho = {radius, X, Y, theta, r, g, b,true,v};

    bixinhos.push_back(newBixinho);
  
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
  bixinho->x+ distance*cos(bixinho->theta) >-1 && bixinho->x + distance*cos(bixinho->theta)<1 ? bixinho->x = bixinho->x + distance*cos(bixinho->theta): bixinho->x=bixinho->x;
  bixinho->y + distance*sin(bixinho->theta)>-1 && bixinho->y + distance*sin(bixinho->theta)<1 ? bixinho->y = bixinho->y + distance*sin(bixinho->theta):bixinho->y=bixinho->y;
}
void moveBixinhoEvo(BixinhoEvolutivo *bixinho, float distance){
  // Para mover para onde ele está olhando (na direção theta)
  bixinho->x+ distance*cos(bixinho->theta) >-1 && bixinho->x + distance*cos(bixinho->theta)<1 ? bixinho->x = bixinho->x + distance*cos(bixinho->theta): bixinho->x=bixinho->x;
  bixinho->y + distance*sin(bixinho->theta)>-1 && bixinho->y + distance*sin(bixinho->theta)<1 ? bixinho->y = bixinho->y + distance*sin(bixinho->theta):bixinho->y=bixinho->y;
}

void rotateBixinhoEvo(BixinhoEvolutivo *bixinho, float angle){
    bixinho->theta+=angle;
}

void segueBixinho(Bixinho*bixinho, float distance){
    float dx,dy,dist,Menordist;
    int index=0,vivos=0;
    
    for(int i=0;i<(int)ListaEvolutiva.size();i++){
      if(ListaEvolutiva[i].exist==true){
        vivos++;
      }
    }
    if(vivos==0){
      return;
    }
    dx=999;
    dy=999;
    dist=sqrt(dx*dx+dy*dy);
    Menordist=dist;
    
    for(int i=0;i<(int)ListaEvolutiva.size();i++){
      if(ListaEvolutiva[i].exist){
        dx=bixinho->x-ListaEvolutiva[i].x;
        dy=bixinho->y-ListaEvolutiva[i].y;
        dist=sqrt(dx*dx+dy*dy);
        if(dist<Menordist){
          Menordist=dist;
          index=i;
        }
      }
    }

    dx=bixinho->x-ListaEvolutiva[index].x;
    dy=bixinho->y-ListaEvolutiva[index].y;
    dist=sqrt(dx*dx+dy*dy);
   
    if(dist< (bixinho->radius + ListaEvolutiva[index].radius)){
        bixinho->x=bixinho->x;
        bixinho->y=bixinho->y;
    }else{
        
        bixinho->theta=atan(dy/dx);
        if(dx>=0){
          bixinho->x - distance*cos(bixinho->theta) >-1 && bixinho->x - distance*cos(bixinho->theta)<1 ? bixinho->x = bixinho->x - distance*cos(bixinho->theta): bixinho->x=bixinho->x;
          bixinho->y - distance*sin(bixinho->theta)>-1 && bixinho->y - distance*sin(bixinho->theta)<1 ? bixinho->y = bixinho->y - distance*sin(bixinho->theta):bixinho->y=bixinho->y;
        }else{
          bixinho->x+ distance*cos(bixinho->theta) >-1 && bixinho->x + distance*cos(bixinho->theta)<1 ? bixinho->x = bixinho->x + distance*cos(bixinho->theta): bixinho->x=bixinho->x;
          bixinho->y + distance*sin(bixinho->theta)>-1 && bixinho->y + distance*sin(bixinho->theta)<1 ? bixinho->y = bixinho->y + distance*sin(bixinho->theta):bixinho->y=bixinho->y;
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
void drawBixinhoEvo(BixinhoEvolutivo bixinho){
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

  glColor3f(0.0, 0.0, 0);// Define que será desenhar um objeto verde 0.5/0.2
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
    int vivos=QuantidadeVivos();
    displayText("Versao ",12,-1.0f,0.95f);
    
    displayText("Populacao:",populacao,-1.0f,0.9f);
    
    displayText("Indivíduos Vivos:",bixinhos.size(),-1.0f,0.85f);
    
    displayText("Pontuacao do melhor Individuo:",melhor.pontuacao,-1.0f,0.8f);
    
    //displayText("Indice da Rede Neural do melhor inviduo:",IndexMelhor,-1.0f,0.75f);

    displayText("Numero de balas:",balas.size(),-1.0f,0.7f);

    displayText("Velocidade:",bixinhos.size()>0?bixinhos[0].vel:0,-1.0f,0.65f);
    
    displayText("Click foi feito em x:",clickx,-1.0f,0.60f);
    
    displayText("Click foi feito em y:",clicky,-1.0f,0.55f);
    
    displayText("Redes Evolutivas vivas:",vivos,-1.0f,0.50f);
    
    
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