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
	float scale;
    if(argc > 1){
        fileName = argv[1];
    }else{
        fileName = "cat.off";
    }

    if(argc > 2){
        scale = atof(argv[2]);
    }else{
        scale = 1;
    }

    ui.glWidget->setFilename(fileName,scale);
    window->show();
    //GLWidget window;

    //window.resize(800,600);
    //window.show();

    return app.exec();
}



