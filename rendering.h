#ifndef RENDERING_H
#define RENDERING_H

typedef void (*VoidFuncType)();
typedef void (*keyboardFuncType)(unsigned char key, int mouseX, int mouseY);

class Rendering {
  public:
    static void drawAxis();
    static void drawCube(float x, float y, float z, float r, float g, float b);
    static void prepareView(int x, int y, int dx, int dy);
    static void display();
    static void resize(int w, int h);
    static void keyboard ( unsigned char key, int mouseX, int mouseY );
    static void initialize(VoidFuncType drawWorldF, VoidFuncType drawDominantSpeciesF);
};

#endif
