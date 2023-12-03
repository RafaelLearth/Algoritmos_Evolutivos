
#include "config.h"
#include "creature.h"
#include "menu_handler.h"
#include "utility.h"

int main(int argc, char** argv) {
    // Configuração e inicialização do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Versao 8");
    glClearColor(0.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(draw);
    menuInit();
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutTimerFunc(0, timer, 0);
    glutMouseFunc(mouse);
    glutMainLoop();
    
    return 0;
}
