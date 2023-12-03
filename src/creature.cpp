#include "creature.h"
#include "config.h"

vector<Bixinho> bixinhos;
vector<Bala> balas;
Bixinho player = {0.04, 0, 0, M_PI / 2, 0.8, 0, 0.8, true, 0.02};

// Implementações das funções para Bixinho, Monstrinho, Bala
void drawBala(Bala bala) {
	if (bala.exist) {
		float radius = 0.01;
		float x = bala.x;
		float y = bala.y;

		glColor3f(bala.r, bala.g, bala.b);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i += 5) {
			glVertex2d(radius * cos(i / 180.0 * M_PI) + x,
					   radius * sin(i / 180.0 * M_PI) + y);
		}
		glEnd();
	}
}

void moveBala(Bala *bala) {
	// Para mover para onde ele está olhando (na direção theta)
	bala->x = bala->x + 0.05 * cos(bala->theta);
	bala->y = bala->y + 0.05 * sin(bala->theta);

	// Impede que o wilson saia da tela
	bala->x = bala->x > 1 ? bala->exist = false : bala->x;
	bala->y = bala->y > 1 ? bala->exist = false : bala->y;
}

void mataBixinho() {

	for (int i = 0; i < int(balas.size()); i++) {
		for (int j = 0; j < int(bixinhos.size()); j++) {
			float dx = balas[i].x - bixinhos[j].x;
			float dy = balas[i].y - bixinhos[j].y;

			float dist = sqrt(dx * dx + dy * dy);
			if ((dist <= bixinhos[j].radius + 0.02f) &&
				bixinhos[j].exist == true && balas[i].exist == true) {
				bixinhos[j].exist = false;
				balas[i].exist = false;
				balas.erase(balas.begin() + i);
				bixinhos.erase(bixinhos.begin() + j);
			}

			// if(bixinhos[j].exist==false){
			//   bixinhos.erase(bixinhos.begin() + j);
			// }
		}
	}
}

void deletaBalas() {
	for (int i = 0; i < int(balas.size()); i++) {
		if (balas[i].x >= 0.98f || balas[i].x <= -0.98f ||
			balas[i].y >= 0.98f || balas[i].y <= -0.98f) {
			balas[i].exist = false;
		}
		if (balas[i].exist == false) {
			balas.erase(balas.begin() + i);
		}
	}
}

void rotateBixinho(Bixinho *bixinho, float angle) { bixinho->theta += angle; }

void moveBixinho(Bixinho *bixinho, float distance) {
	// Para mover para onde ele está olhando (na direção theta)
	bixinho->x = bixinho->x + distance * cos(bixinho->theta);
	bixinho->y = bixinho->y + distance * sin(bixinho->theta);
}

void segueBixinho(Bixinho *bixinho, float distance) {
	float dx = bixinho->x - player.x;
	float dy = bixinho->y - player.y;
	float dist = sqrt(dx * dx + dy * dy);

	if (dist < (bixinho->radius + player.radius)) {
		bixinho->x = bixinho->x;
		bixinho->y = bixinho->y;
	} else {
		// if(atan(dy/dx)!=0){
		//     bixinho->theta=atan(dy/dx);
		//     bixinho->x = bixinho->x + distance*cos(bixinho->theta);
		//     bixinho->y = bixinho->y + distance*sin(bixinho->theta);

		//}else{
		//    bixinho->theta=atan(dy/dx);
		//    bixinho->x = bixinho->x + distance*dy/dist;
		//    bixinho->y = bixinho->y + distance*dx/dist;

		//}
		bixinho->theta = atan(dy / dx);
		if (dx >= 0) {
			bixinho->x = bixinho->x - distance * cos(bixinho->theta);
			bixinho->y = bixinho->y - distance * sin(bixinho->theta);
		} else {
			bixinho->x = bixinho->x + distance * cos(bixinho->theta);
			bixinho->y = bixinho->y + distance * sin(bixinho->theta);
		}

		// bixinho->x = bixinho->x>1 ? -1 : bixinho->x;
		// bixinho->y = bixinho->y>1 ? -1 : bixinho->y;
	}
}

void drawBixinho(Bixinho bixinho) {
	if (bixinho.exist) {
		// Função para desenhar o bixinho
		float radius = bixinho.radius;
		float x = bixinho.x;
		float y = bixinho.y;
		float theta = bixinho.theta;
		//----- Desenha corpo do bixinho -----//
		glColor3f(bixinho.r, bixinho.g, bixinho.b); // Bixinho verde
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i += 5) {
			glVertex2d(radius * cos(i / 180.0 * M_PI) + x,
					   radius * sin(i / 180.0 * M_PI) + y);
		}
		glEnd();

		//----- Desenha olho direito do bixinho -----//
		float eyeRadius = radius / 8;
		float eyeDist = M_PI / 6;

		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i += 5) {
			float shiftX = radius / 2 * cos(theta - eyeDist);
			float shiftY = radius / 2 * sin(theta - eyeDist);
			glVertex2d(eyeRadius * cos(i / 180.0 * M_PI) + x + shiftX,
					   eyeRadius * sin(i / 180.0 * M_PI) + y + shiftY);
		}
		glEnd();

		//----- Desenha olho esquerdo do bixinho -----//
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i += 5) {
			float shiftX = radius / 2 * cos(theta + eyeDist);
			float shiftY = radius / 2 * sin(theta + eyeDist);

			glVertex2d(eyeRadius * cos(i / 180.0 * M_PI) + x + shiftX,
					   eyeRadius * sin(i / 180.0 * M_PI) + y + shiftY);
		}
		glEnd();
	}
}