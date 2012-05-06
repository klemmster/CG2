#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include "GLWidget.hpp"

#include "offLoader.hpp"
#include "tree.hpp"
#include "vertex.hpp"
#include "ui_mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow *window = new QMainWindow;
    Ui::MainWindow ui;
    ui.setupUi(window);
    
    std::string fileName;
    if(argc > 1){
        fileName = argv[1];
    }else{
        fileName = "torus.off";
    }

    ui.glWidget->setFilename(fileName);
    window->show();
    //GLWidget window;
    
    //window.resize(800,600);
    //window.show();

    return app.exec();
}



