#ifndef RENDERING_H
#define RENDERING_H

typedef void (*DrawFuncType)();

class Rendering {
  public:
    static void drawCube(float x, float y, float z, float r, float g, float b);
    static void display();
    static void keyboard ( unsigned char key, int mouseX, int mouseY );
    static void initialize(DrawFuncType func);
};

#endif
