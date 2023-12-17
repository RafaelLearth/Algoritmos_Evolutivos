
#ifndef CONFIG_H
#define CONFIG_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <sstream>
#include <string.h>
#include <vector>
using namespace std;

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

#endif // CONFIG_H
