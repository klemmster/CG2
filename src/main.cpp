#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include "GLWidget.hpp"

#include "offLoader.hpp"
#include "vertex.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    OffLoader loader;

    std::string fileName;
    if(argc > 1){
        fileName = argv[1];
    }else{
        fileName = "torus.off";
    }
    VertexList vertices = loader.readOff(fileName);
    GLWidget window;
    window.resize(800,600);
    window.show();

    return app.exec();
}



