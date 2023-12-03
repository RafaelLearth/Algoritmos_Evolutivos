
#include "menu_handler.h"
#include "creature.h"
#include "config.h"
#include "utility.h"

// Implementações das funções de manipulação de menu

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