#ifndef _GLWIDGET_H
#define _GLWIDGET_H


#include <QtOpenGL/QGLWidget>
#include "vertex.hpp"
#include "tree.hpp"

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    GLWidget(const std::string& fileName, QWidget *parent = NULL);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    std::string m_fileName;
    VertexList vertices;
    KDTree tree;
};

#endif  /* _GLWIDGET_H */
