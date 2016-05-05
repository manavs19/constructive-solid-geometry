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
    vs.FromPointset(*ps, false);
    ps->resetColor();
    vs.setScale();
}

void GLWidget::updateNewVoxels(pointset* temp) {
    vs.FromPointset(*temp, true);
    vs.setScale();
}

void GLWidget::saveObj(const char* filename) {
    vs.save(filename);
}

void GLWidget::initializeGL() {
    glClearColor(0.0,0.0,0.0,1);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_DIFFUSE);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat light_position[] = {0.1, 0.1, 0.1, 0.1 };
    glClearColor (0.2, 0.2, 0.2, 0.0);
    glShadeModel (GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    float black[4]={0,0,0,0};
    float gray[4]={0.4, 0.4, 0.4, 0};
    glMaterialfv(GL_FRONT,GL_AMBIENT,gray);
    glMaterialfv(GL_FRONT,GL_SPECULAR,black);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

Point3D GLWidget::findNormal(int i) {
    double a = (-vs.vertices[vs.faces[i][0]].y+vs.vertices[vs.faces[i][1]].y)*(vs.vertices[vs.faces[i][2]].z-vs.vertices[vs.faces[i][1]].z)-(-vs.vertices[vs.faces[i][0]].z+vs.vertices[vs.faces[i][1]].z)*(vs.vertices[vs.faces[i][2]].y-vs.vertices[vs.faces[i][1]].y);
    double b = -(-vs.vertices[vs.faces[i][0]].x+vs.vertices[vs.faces[i][1]].x)*(vs.vertices[vs.faces[i][2]].z-vs.vertices[vs.faces[i][1]].z)+(-vs.vertices[vs.faces[i][0]].z+vs.vertices[vs.faces[i][1]].z)*(vs.vertices[vs.faces[i][2]].x-vs.vertices[vs.faces[i][1]].x);
    double c = (-vs.vertices[vs.faces[i][0]].x+vs.vertices[vs.faces[i][1]].x)*(vs.vertices[vs.faces[i][2]].y-vs.vertices[vs.faces[i][1]].y)-(-vs.vertices[vs.faces[i][0]].y+vs.vertices[vs.faces[i][1]].y)*(vs.vertices[vs.faces[i][2]].x-vs.vertices[vs.faces[i][1]].x);
    double norm = sqrt(a*a+b*b+c*c);
    a /= norm;
    b /= norm;
    c /= norm;
    return Point3D(a, b, c);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0.7, 0.7, 0.7);
    gluLookAt(ax, ay, az, 0, 0, 0, 0, u, 0);

    glBegin(GL_QUADS);

    if(vs.faces.size() == 0)
        glVertex3f(0,0,0);
    else {
        for (unsigned int i = 0; i < vs.faces.size(); ++i)
        {
            Point3D normal = findNormal(i);
            if(vs.fcolor[i])
                glColor3f(0.7, 0, 0);
            for (unsigned int j = 0; j < vs.faces[i].size(); ++j)
            {
                glVertex3f((vs.vertices[vs.faces[i][j]].x-vs.center.x)/vs.scalex, (vs.vertices[vs.faces[i][j]].y-vs.center.y)/vs.scaley,
                        (vs.vertices[vs.faces[i][j]].z-vs.center.z)/vs.scalez);
            }
            glColor3f(0.7, 0.7, 0.7);
            glNormal3f(normal.x, normal.y, normal.z);
        }
    }

    glEnd();

}

void GLWidget::resizeGL(int width, int height) {
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glClearColor(0,0,0,1);
    glViewport(0, 0, width, height);
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void GLWidget::keyPressEvent(QKeyEvent* e) {
    double angle, tx, ty, tz;
    switch (e->key()) {
    case Qt::Key_Down:
        angle = -0.02f;
        tz = az;
        ty = ay;
        ay = ty*cos(angle)-tz*sin(angle);
        az = ty*sin(angle)+tz*cos(angle);
        break;
    case Qt::Key_Right:
        angle = 0.02f;
        tx = ax;
        tz = az;
        ax = tx*cos(angle)+tz*sin(angle);
        az = -tx*sin(angle)+tz*cos(angle);
        break;
    case Qt::Key_Up:
        angle = 0.02f;
        tz = az;
        ty = ay;
        ay = ty*cos(angle)-tz*sin(angle);
        az = ty*sin(angle)+tz*cos(angle);
        break;
    case Qt::Key_Left:
        angle = -0.02f;
        tx = ax;
        tz = az;
        ax = tx*cos(angle)+tz*sin(angle);
        az = -tx*sin(angle)+tz*cos(angle);
        break;
    case Qt::Key_A:
        QTextStream(stderr) << "K";
        angle = 0.02f;
        tx = ax;
        ty = ay;
        ax = tx*cos(angle)-ty*sin(angle);
        ay = tx*sin(angle)+ty*cos(angle);
        break;
    case Qt::Key_S:
        angle = -0.02f;
        tx = ax;
        ty = ay;
        ax = tx*cos(angle)-ty*sin(angle);
        ay = tx*sin(angle)+ty*cos(angle);
        break;
    }
}

void GLWidget::rotateTemp(double rx, double ry, double rz) {
    QTextStream(stderr) << rx << " " << ry << " " << rz << endl;
    ps_temp->rotate(rx, ry, rz);
    pointset temp(*ps_temp);
    temp.union_(*ps);
    updateNewVoxels(&temp);
}

void GLWidget::translateTemp(double x, double y, double z) {
    ps_temp->translate(x, y, z);
    pointset temp(*ps_temp);
    temp.union_(*ps);
    updateNewVoxels(&temp);

}

