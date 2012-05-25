#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif


#include "GLWidget.hpp"
#include "offLoader.hpp"
#include "stopwatch.hpp"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent){
    setMouseTracking(true);
        showTree = 0;
    setFocus();
}

void GLWidget::setFilename(const std::string& fileName) {
    m_fileName = fileName;
}

QPoint lastPos;
GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat rotationZ = 0.0f;
GLfloat positionZ = -30.0f;
GLfloat positionY = 0.0f;
GLfloat positionX = 0.0f;
GLfloat modelOffsetX = 0.0f;
GLfloat modelOffsetY = 0.0f;
GLfloat modelOffsetZ = 0.0f;

unsigned int kNearest = 50;
float radius = 40;
size_t vrtxID = 0;
vec3f highlightColor(0.0, 1.0, 0.0);

void GLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

    OffLoader loader;
    Stopwatch readTimer("ParseFile");
    vertices = loader.readOff(m_fileName);
    readTimer.stop();
    Stopwatch treeTimer("GenTree");
    tree = KDTree(vertices, 2);
    treeTimer.stop();
    grid = Grid(tree.getMinVertices(), tree.getMaxVertices(), 16, 16);
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1f, 10000.0f);
    gluLookAt(0, 0, 1, 0, 0, 0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    glLoadIdentity();

    gluLookAt(positionX, positionY, positionZ, 0, 0, 0, 0, 1, 0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    glTranslatef(modelOffsetX, modelOffsetY, modelOffsetZ);
    if (showTree)
    {
        tree.draw();
    }
    grid.draw();
    //glScalef(20, 20, 20);
    glPointSize(3);
    glBegin(GL_POINTS);

    for(auto vertex : vertices)
    {
        vertex->draw();
    }
    //glBegin(GL_POLYGON);
    //glVertex2f(0,0);
    //glVertex2f(100,500);
    //glVertex2f(500,100);
    glEnd();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
    setFocus();
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

void GLWidget::wheelEvent(QWheelEvent* event){
    positionZ += event->delta()/10.0;
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        break;
    case Qt::Key_Left:
        positionX -= 1.0;
        updateGL();
        break;
    case Qt::Key_Right:
        positionX += 1.0;
        updateGL();
        break;
    case Qt::Key_Up:
        positionY += 1.0;
        updateGL();
        break;
    case Qt::Key_Down:
        positionY -= 1.0;
        updateGL();
        break;
    case Qt::Key_W:
        modelOffsetY += 1.0;
        updateGL();
        break;
    case Qt::Key_S:
        modelOffsetY -= 1.0;
        updateGL();
        break;
    case Qt::Key_A:
        modelOffsetX -= 1.0;
        updateGL();
        break;
    case Qt::Key_D:
        modelOffsetX += 1.0;
        updateGL();
        break;
    case Qt::Key_Q:
        modelOffsetZ -= 1.0;
        updateGL();
        break;
    case Qt::Key_E:
        modelOffsetZ += 1.0;
        updateGL();
        break;
    default:
        event->ignore();
        break;
    }
}

void GLWidget::sigShowKDTree(bool show) {
    showTree = show;
    updateGL();
}

void resetVertexColors(VertexList src){
    for(auto elem: src){
        elem->resetColor();
    }
}

void GLWidget::sigFindKNearest(){
    resetVertexColors(vertices);
    auto result = tree.findKNearestNeighbours(vertices.at(vrtxID), kNearest);
    vertices.at(vrtxID)->highlight(highlightColor);
    updateGL();
}

void GLWidget::sigFindInRadius(){
    resetVertexColors(vertices);
    auto result = tree.findInRadius(vertices.at(vrtxID), radius);
    vertices.at(vrtxID)->highlight(highlightColor);
    updateGL();
}

void GLWidget::sigSetRadius(double r){
    if(r>0){
        radius = (float)r;
        cout << "Set Radius to: " << radius << "\n";
    }
}

void GLWidget::sigSetKNearest(int k){
    if(k < 0)
        return;
    if(k >= (int)vertices.size()){
        kNearest = vertices.size()-1;
    }else{
        kNearest = k;
    }
}

void GLWidget::sigSelectPixel(int pxID){
    if(pxID >=0 && pxID < (int)vertices.size()){
        vrtxID = pxID;
    }
    resetVertexColors(vertices);
    vertices.at(vrtxID)->highlight(highlightColor);
    updateGL();
}
