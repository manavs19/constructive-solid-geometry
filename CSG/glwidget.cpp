#include "glwidget.h"
#include <GL/glut.h>
#include <QTextStream>
#include <cmath>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(100);
    ps = NULL;
    ps_temp = NULL;

    rangle = 0;
    az = 0.1;
    ay = ax = 0;
    u = 1;
}

void GLWidget::setVoxelSet(VoxelSet vs) {
    this->vs = vs;
}

void GLWidget::updateVoxelSet() {
    vs.FromPointset(*ps);
    vs.setScale();
}

void GLWidget::updateNewVoxels(pointset* temp) {
    vs.FromPointset(*temp);
    vs.setScale();
}

void GLWidget::saveObj(const char* filename) {
    vs.save(filename);
}

void GLWidget::initializeGL() {
    glClearColor(0.2,0.2,0.2,1);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = {1.0, 1.0, -2.0, 0.0 };
    glClearColor (0.2, 0.2, 0.2, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//    GLfloat light_position2[] = {1.0, -1.0, 2.0, 0.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position2);

}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 0, 0);
    gluLookAt(ax, ay, az, 0, 0, 0, 0, u, 0);
//    glRotatef(rangle, ax, ay, az);
      glBegin(GL_QUADS);

    if(vs.faces.size() == 0)
        glVertex3f(0,0,0);
    else {
//        QTextStream(stderr) << vs.scalex << " " << vs.scaley << " " << vs.scalez << endl;
        for (unsigned int i = 0; i < vs.faces.size(); ++i)
        {
            for (unsigned int j = 0; j < vs.faces[i].size(); ++j)
            {
                glVertex3f((vs.vertices[vs.faces[i][j]].x-vs.center.x)/vs.scalex, (vs.vertices[vs.faces[i][j]].y-vs.center.y)/vs.scaley,
                        (vs.vertices[vs.faces[i][j]].z-vs.center.z)/vs.scalez);
            }
        }
    }

    glEnd();

}

void GLWidget::resizeGL(int width, int height) {
    glClearColor(0.2,0.2,0.2,1);
    glViewport(0, 0, width, height);
}

void GLWidget::keyPressEvent(QKeyEvent* e) {
    double angle, tx, ty, tz;
    switch (e->key()) {
    case Qt::Key_Down:
        QTextStream(stderr) << "DOWN";
        angle = -0.02;
        tz = az;
        ty = ay;
        ay = ty*cos(angle)-tz*sin(angle);
        az = ty*sin(angle)+tz*cos(angle);
        if(tz*az < 0) {
            u = -u;
        }
        QTextStream(stderr) << " " << ax << " " << ay << " " << az << " " << (az*tz<0) << " " << sin(angle) << endl;
        break;
    case Qt::Key_Right:
        QTextStream(stderr) << "RIGHT";
        angle = 0.02f;
        tx = ax;
        tz = az;
        ax = tx*cos(angle)+tz*sin(angle);
        az = -tx*sin(angle)+tz*cos(angle);
        QTextStream(stderr) << " " << ax << " " << ay << " " << az << " " << (az*tz<0) << " " << sin(angle) << endl;
        if(tz*az < 0) {
            u = -u;
        }
        break;
    case Qt::Key_Up:
        QTextStream(stderr) << "UP";
        angle = 0.02f;
        tz = az;
        ty = ay;
        ay = ty*cos(angle)-tz*sin(angle);
        az = ty*sin(angle)+tz*cos(angle);
        if(tz*az < 0) {
            u = -u;
        }
        QTextStream(stderr) << " " << ax << " " << ay << " " << az << " " << (az*tz<0) << " " << sin(angle) << endl;
        break;
    case Qt::Key_Left:
        QTextStream(stderr) << "DOWN";
        angle = -0.02f;
        tx = ax;
        tz = az;
        ax = tx*cos(angle)+tz*sin(angle);
        az = -tx*sin(angle)+tz*cos(angle);
        if(tz*az < 0) {
            u = -u;
        }
        QTextStream(stderr) << " " << ax << " " << ay << " " << az << " " << (az*tz<0) << " " << sin(angle) << endl;
        break;
    case Qt::Key_A:
        QTextStream(stderr) << "OK";
        u = -u;
        break;
    }
}

void GLWidget::scaleTemp(double x, double y, double z) {
    ps_temp->scale(x, y, z);
}

void GLWidget::translateTemp(double x, double y, double z) {
    ps_temp->translate(x, y, z);
    pointset temp(*ps_temp);
    temp.union_(*ps);
    updateNewVoxels(&temp);

}

