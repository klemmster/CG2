#ifndef _GLWIDGET_H
#define _GLWIDGET_H


#include <QtOpenGL/QGLWidget>
#include "vertex.hpp"
#include "tree.hpp"
#include "Grid.hpp"

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    GLWidget(QWidget *parent = NULL);
    void setFilename(const std::string& fileName);

public slots:
    void sigShowKDTree(bool show);
    void sigShowWLS(bool show);
    void sigShowBezier(bool show);

    void sigFindInRadius();
    void sigSetRadius(double r);
    void sigSetH(double h);
    void sigSetK(int k);

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
    VertexList vertices;
    KDTree tree;
    Grid grid;
    size_t m_k;

private:
    bool showTree;
};

#endif  /* _GLWIDGET_H */
