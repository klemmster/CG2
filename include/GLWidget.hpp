#ifndef _GLWIDGET_H
#define _GLWIDGET_H


#include <QtOpenGL/QGLWidget>
#include "vertex.hpp"
#include "tree.hpp"

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    GLWidget(QWidget *parent = NULL);
    void setFilename(const std::string& fileName);
    
public slots:
    void showKDTree(bool show);

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
    
private:
    bool showTree;
};

#endif  /* _GLWIDGET_H */
