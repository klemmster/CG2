#ifndef _GLWIDGET_H
#define _GLWIDGET_H


#include <QtOpenGL/QGLWidget>
#include "normal.hpp"
#include "vertex.hpp"
#include "tree.hpp"
#include "Grid3D.hpp"
#include "MarchingCube.hpp"

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    GLWidget(QWidget *parent = NULL);
    void setFilename(const std::string& fileName,float scale, float radius);

public slots:
    void sigShowKDTree(bool show);
    void sigShowWLS(bool show);
    void sigShowBezier(bool show);
    void sigShowRepeated(bool show);

    void sigFindInRadius();
    void sigSetRadius(double r);
    void sigSetH(double h);
    void sigSetK(int k);
    void sigSetN(int n);
    void sigSetM(int m);

    void sigSetKNearest(int k);
    void sigFindKNearest();

    void sigSelectPixel(int pxID);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    std::string m_fileName;
    NormalList normals;
    VertexList vertices;
    KDTree tree;
    Grid3D grid;
    MarchingCubes marchingCubes;
    size_t m_k;
    size_t m_n;
    size_t m_m;
	float m_scale;
    float m_radius;

private:
    bool showTree;
};

#endif  /* _GLWIDGET_H */
