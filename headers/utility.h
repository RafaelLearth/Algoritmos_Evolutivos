
#ifndef UTILITY_H
#define UTILITY_H

#include <map>
#include <string>
#include <unordered_set>

using namespace std;

void gameover();
void tabelaInfo();
void draw();
void timer(int);
void mouse(int button, int state, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keyDown(unsigned char key, int x, int y);
void updateKeyboard();
void circle(float x, float y, float radius);
void strangePolygon(float x, float y);
void anotherStrangePolygon(float x, float y);
void displayText(std::string text, float data, float x, float y);

// VARIAVEIS GLOBAIS
extern float clickx;
extern float clicky;
extern float TIME;
extern int behavior;
extern int wave;
extern bool pause;

#endif // UTILITY_H
