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
#include "RayCaster.hpp"
#include "MarchingCube.hpp"

#define SHIFT_SPEED 0.05f

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent){
    setMouseTracking(true);
        showTree = 0;
    setFocus();
    m_k = 1;
    m_radius = 1.0f;
}

void GLWidget::setFilename(const std::string& fileName,float scale,
        float radius, int dims) {
    m_fileName = fileName;
	m_scale = scale;
    m_radius = radius;
    m_dims = dims;
}

QPoint lastPos;
GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat rotationZ = 0.0f;
GLfloat positionZ = 0.0f;
GLfloat positionY = 0.0f;
GLfloat positionX = 0.0f;
GLfloat camDistance = 5.0f;
GLfloat scale = 1;
GLfloat eyeDirX = 0;
GLfloat eyeDirY = 0;
GLfloat eyeDirZ = 0;
GLfloat camAlpha = 0;
GLfloat camBeta = 0;
GLfloat zoom = 0.6f;
GLfloat modelOffsetX = 0.0f;
GLfloat modelOffsetY = 0.0f;
GLfloat modelOffsetZ = 0.0f;
GLfloat screenRatio;
bool useAlpha = false;
bool drawCloud = true;

RayCaster rayCaster;
int doRayCasting = -1;
bool locRepaint = false;

unsigned int kNearest = 50;
float radius = 40;
size_t vrtxID = 0;
vec3f highlightColor(0.0, 1.0, 0.0);

int idI = 0;
int idJ = 0;
int idK = 0;

void GLWidget::initializeGL() {

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

//    // Somewhere in the initialization part of your programâ¦
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
//
//    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
   // GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 0.0f, 0.0f };
//
//    // Assign created components to GL_LIGHT1
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
   // glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, position);

    OffLoader loader;
    Stopwatch readTimer("ParseFile");
    vertices = loader.readOff(m_fileName,m_scale);
    readTimer.stop();
    Stopwatch treeTimer("GenTree");
    tree = KDTree(vertices, 2);
    treeTimer.stop();
    grid = Grid3D(tree, m_dims,m_dims,m_dims, m_radius);
    marchingCubes = MarchingCubes(grid, m_dims, m_dims, m_dims);
    m_m = 5;
    m_n = 5;
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    screenRatio = (GLfloat)w / (GLfloat)h;
	rayCaster.refreshViewport();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL() {

	if(locRepaint)
		return;

	//grid.getVertex(idI,idJ,idK)->m_Color = vec3f(1,1,0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,0,0);

	//clamp camera values
    if(zoom>1)
		zoom = 1;
    if(zoom<0)
		zoom = 0;

	if(camDistance<0.02f)
		camDistance = 0.02f;
	if(camDistance>10000)
		camDistance = 10000;

	if(camBeta>3.1f/2)
		camBeta = 3.1f/2;
	if(camBeta<-3.1f/2)
		camBeta = -3.1f/2;

	if(scale < 0.005f)
		scale = 0.005f;
	if(scale > 30)
		scale = 30;

	//Zoom
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	float uZoom = zoom*zoom;
    gluPerspective(1+uZoom*70, screenRatio, 0.1f, 1000.0f);


	//Camera position/angle
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	eyeDirX = sin(camAlpha) * cos(camBeta);
	eyeDirY = sin(camBeta);
	eyeDirZ = -cos(camAlpha) * cos(camBeta);

	float eyeX = eyeDirX*camDistance+positionX;
	float eyeY = eyeDirY*camDistance+positionY;
	float eyeZ = eyeDirZ*camDistance+positionZ;

    gluLookAt(
		eyeX, eyeY, eyeZ,
		positionX, positionY, positionZ,
		0, 1, 0
		);

	//Call ray casting
	if(doRayCasting>0) {

		locRepaint = true;
		rayCaster.cast(&grid,doRayCasting,eyeX,eyeY,eyeZ,scale);
		glEnable(GL_LIGHTING);
		doRayCasting = -1;
		locRepaint = true;
		return;
	}

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    glTranslatef(modelOffsetX, modelOffsetY, modelOffsetZ);

	glScalef(scale,scale,scale);



//    if (showTree)
//    {
//        tree.draw();
//    }
    grid.draw(useAlpha);
    //glScalef(20, 20, 20);

    marchingCubes.draw();

    glDisable(GL_LIGHTING);
	if(drawCloud) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glPointSize(3);
		glBegin(GL_POINTS);
		    for(VertexPtr vertex : vertices)
		    {
		        vertex->draw();
		    }
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		for (VertexPtr vertex : vertices)
		{
		    NormalPtr normal = vertex->getNormal();
		    glVertex3fv((*vertex)._v);
		    glVertex3fv(((*normal) / -30 + (*vertex))._v);
		}
		glEnd();
	}

    glEnable(GL_LIGHTING);

}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
    setFocus();
}

void camShift(float shiftX,float shiftY) {
	float eyeCrossLength = sqrt(eyeDirX*eyeDirX + eyeDirZ*eyeDirZ);
	float eyeUpX = -eyeDirZ / eyeCrossLength;
	float eyeUpY = 0;
	float eyeUpZ = eyeDirX / eyeCrossLength;
	float crossUpX = eyeUpY * eyeDirZ - eyeUpZ * eyeDirY;
	float crossUpY = eyeUpZ * eyeDirX - eyeUpX * eyeDirZ;
	float crossUpZ = eyeUpX * eyeDirY - eyeUpY * eyeDirX;
	positionX += shiftX * eyeUpX + shiftY * crossUpX;
	positionY += shiftX * eyeUpY + shiftY * crossUpY;
	positionZ += shiftX * eyeUpZ + shiftY * crossUpZ;
}

void camShiftZ(float shift) {
	positionX -= shift * eyeDirX;
	positionY -= shift * eyeDirY;
	positionZ -= shift * eyeDirZ;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {

    GLfloat dx = (GLfloat)(event->x() - lastPos.x()) / width();
    GLfloat dy = (GLfloat)(event->y() - lastPos.y()) / height();

    if (event->buttons() & Qt::LeftButton) {
		locRepaint = false;
        camAlpha += 4 * dx;
        camBeta += 3 * dy;
        updateGL();
    }

    if (event->buttons() & Qt::RightButton) {
		locRepaint = false;
        camDistance += 3.5f * dy;
        updateGL();
    }

    if (event->buttons() & Qt::MiddleButton) {
		locRepaint = false;
		camShift(dx,dy);
        updateGL();
    }

    lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event){
	locRepaint = false;
    //positionZ += event->delta()/100.0;
    zoom -= event->delta()*0.00015f;
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {

    switch(event->key()) {
    case Qt::Key_Escape:
        break;

	case Qt::Key_Plus:
		scale += 0.1f;
		updateGL();
		break;
	case Qt::Key_Minus:
		scale -= 0.1f;
		updateGL();
		break;
    case Qt::Key_Left:
        rotationY -= 4.0;
        updateGL();
        break;
    case Qt::Key_Right:
        rotationY += 4.0;
        updateGL();
        break;
    case Qt::Key_Up:
        rotationX += 4.0;
        updateGL();
        break;
    case Qt::Key_Down:
        rotationX -= 4.0;
        updateGL();
        break;
    case Qt::Key_W:
        camShift(0,SHIFT_SPEED);
        updateGL();
        break;
    case Qt::Key_S:
        camShift(0,-SHIFT_SPEED);
        updateGL();
        break;
    case Qt::Key_A:
        camShift(SHIFT_SPEED,0);
        updateGL();
        break;
    case Qt::Key_D:
        camShift(-SHIFT_SPEED,0);
        updateGL();
        break;
    case Qt::Key_I:
        idI++;
        updateGL();
        break;
    case Qt::Key_J:
        idJ++;
        updateGL();
        break;
    case Qt::Key_K:
        idK++;
        updateGL();
        break;
    case Qt::Key_Q:
        camShiftZ(-SHIFT_SPEED);
        updateGL();
        break;
    case Qt::Key_E:
        camShiftZ(SHIFT_SPEED);
        updateGL();
        break;
	case Qt::Key_L:
		useAlpha ^= 1;
		updateGL();
		break;
	case Qt::Key_O:
		drawCloud ^= 1;
		updateGL();
		break;
	case Qt::Key_C:
		positionX = 0;
		positionY = 0;
		positionZ = 0;
		camDistance = 5;
		camAlpha = 0;
		camBeta = 0;
		zoom = 0.6f;
		rotationX = 0;
		rotationY = 0;
		rotationZ = 0;
		modelOffsetX = 0;
		modelOffsetY = 0;
		modelOffsetZ = 0;
		scale = 1;
		updateGL();
		break;
	case Qt::Key_R:
		doRayCasting = RC_FIRSTTOUCH;
		updateGL();
		break;
	case Qt::Key_T:
		doRayCasting = RC_TRANSPARENT;
		updateGL();
		break;
    default:
        event->ignore();
        return;
    }

	//locRepaint = false;
}

void GLWidget::sigShowKDTree(bool show) {
    showTree = show;
    updateGL();
}

void GLWidget::sigShowWLS(bool show) {
    grid.disableQuads();
    updateGL();
}

void GLWidget::sigShowBezier(bool show) {
    if(show){
    grid.enableQuads();
    grid.approximateTensor(m_k);
    }else{
        grid.disableQuads();
        grid.reapproximateWLS();
    }
    updateGL();
}

void GLWidget::sigShowRepeated(bool show) {
    if(show){
        grid.enableQuads();
        grid.repeatedApproximation(m_k);
    }else{
        grid.disableQuads();
        grid.reapproximateWLS();
    }
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
        grid.setRadius(radius);
    }

    grid.m_k = m_k;
    grid.reapproximateWLS();
    updateGL();
}

void GLWidget::sigSetH(double h) {
    if (h > 0){
        //radius = (float)h;
        cout << "Set H to: " << h << "\n";
        grid.setH( (float) h );
    }

    grid.m_k = m_k;
    grid.reapproximateWLS();
    updateGL();
}

void GLWidget::sigSetK(int k) {
    if (k > 0){
        cout << "Set K to: " << k << "\n";
        m_k = k;
    }
    updateGL();
}

void GLWidget::sigSetN(int n) {
    if (n > 0){
        cout << "Set N to: " << n << "\n";
        m_n = n;
        //grid = Grid(tree, m_m, m_n);
    }
    updateGL();
}

void GLWidget::sigSetM(int m) {
    if (m > 0){
        cout << "Set M to: " << m << "\n";
        m_m = m;
        //grid = Grid(tree, m_m, m_n);
    }
    updateGL();
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
