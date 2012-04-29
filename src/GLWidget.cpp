#include <QtGui/QMouseEvent>

#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif


#include "GLWidget.hpp"
#include "offLoader.hpp"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
}

QPoint lastPos;
GLfloat rotationX;
GLfloat rotationY;
GLfloat rotationZ;

void GLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

    OffLoader loader;
    vertices = loader.readOff("cow.off");
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    gluLookAt(0, 0, 1, 0, 0, 0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    glLoadIdentity();
    glTranslatef(0, 0, -15);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    //glScalef(20, 20, 20);
    glBegin(GL_POINTS);

    for (size_t i = 0; i < vertices.size(); i++)
    {
        glLoadName(i);
        glVertex3fv(vertices.at(i)->_v);
    }
    //glBegin(GL_POLYGON);
    //glVertex2f(0,0);
    //glVertex2f(100,500);
    //glVertex2f(500,100);
    glEnd();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    //printf("%d, %d\n", event->x(), event->y());

    GLfloat dx = (GLfloat)(event->x() - lastPos.x()) / width();
    GLfloat dy = (GLfloat)(event->y() - lastPos.y()) / height();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        updateGL();
    }

    lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}

