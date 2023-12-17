
#ifndef CREATURE_H
#define CREATURE_H

#include <cmath>
#include <vector>

using namespace std;

// Estruturas para Bixinho, Monstrinho, Bala
typedef struct _bixinho {
	float radius;
	float x;
	float y;
	float theta;
	float r, g, b;
	bool exist;
	float vel;
} Bixinho;

typedef struct _bala {
	float x;
	float y;
	float theta;
	float r, g, b;
	bool exist;
} Bala;

extern vector<Bixinho> bixinhos;
extern vector<Bala> balas;
extern Bixinho player;

void drawBala(Bala bala);
void moveBala(Bala *bala);
void mataBixinho();
void deletaBalas();
void rotateBixinho(Bixinho *bixinho, float angle);
void moveBixinho(Bixinho *bixinho, float distance);
void segueBixinho(Bixinho *bixinho, float distance);
void drawBixinho(Bixinho bixinho);

#endif // CREATURE_H
