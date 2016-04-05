#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "voxelset.h"
#include "pointset.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <QTimer>
#include <QKeyEvent>

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    VoxelSet vs;
    QTimer timer;
    double rangle, ax, ay, az, u;
public:
    pointset *ps;
    pointset *ps_temp;
    explicit GLWidget(QWidget *parent = 0);
    void setVoxelSet(VoxelSet);
    void updateVoxelSet();
    void updateNewVoxels(pointset*);
    void initializeGL();
    void paintGL();
    virtual void keyPressEvent(QKeyEvent*);
    void resizeGL(int, int);
    void saveObj(const char*);
    void scaleTemp(double x, double y, double z);
    void translateTemp(double x, double y, double z);


};

#endif // GLWIDGET_H
