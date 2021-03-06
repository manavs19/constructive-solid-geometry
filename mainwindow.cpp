#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "surfaceToVoxels.h"
#include "voxelset.h"
#include "pointset.h"
#include "inputdialog.h"
#include <iostream>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QDialog>
#include <set>
#include <QDoubleValidator>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sx->setValidator(new QDoubleValidator(0, 360, 2));
    ui->sy->setValidator(new QDoubleValidator(0, 360, 2));
    ui->sz->setValidator(new QDoubleValidator(0, 360, 2));
    ui->tx->setValidator(new QDoubleValidator(-100, 100, 2));
    ui->ty->setValidator(new QDoubleValidator(-100, 100, 2));
    ui->tz->setValidator(new QDoubleValidator(-100, 100, 2));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"~",tr("OBJ files (*.obj)"));
    open(fileName);

}

void MainWindow::open(QString fileName) {
    VoxelSet vs;
    bool color = (ui->glpanel->ps != NULL);
    vs.ReadFromFile(fileName.toStdString().c_str());
    pointset temp(vs.createPointSet(color));

    if(ui->glpanel->ps == NULL) {
        ui->glpanel->ps = new pointset(temp);
        ui->glpanel->updateVoxelSet();
    }

    else if(ui->glpanel->ps_temp == NULL) {
        ui->glpanel->ps_temp = new pointset(temp);
        temp.union_(*ui->glpanel->ps);
        ui->glpanel->updateNewVoxels(&temp);
        ui->groupBox->setEnabled(true);
        ui->scalePanel->setEnabled(true);
        ui->translatePanel->setEnabled(true);

    }

    else {
        ui->glpanel->ps_temp = new pointset(temp);
        temp.union_(*ui->glpanel->ps);
        ui->glpanel->updateNewVoxels(&temp);
        ui->groupBox->setEnabled(true);
        ui->scalePanel->setEnabled(true);
        ui->translatePanel->setEnabled(true);
    }
}

void MainWindow::on_applyButton_clicked()
{
    QTextStream(stderr) << "OK\n";
    if(ui->glpanel->ps != NULL && ui->glpanel->ps_temp != NULL) {
        if(ui->unionRadio->isChecked()) {
            ui->glpanel->ps->union_(*(ui->glpanel->ps_temp));
            ui->glpanel->updateVoxelSet();
            ui->glpanel->ps_temp = NULL;
//            QTextStream(stderr) << "OK2\n";
            ui->groupBox->setEnabled(false);
        }

        else if(ui->intersectRadio->isChecked()) {
            ui->glpanel->ps->intersection_(*(ui->glpanel->ps_temp));
            ui->glpanel->updateVoxelSet();
            ui->glpanel->ps_temp = NULL;
//            QTextStream(stderr) << "OK\n";
            ui->groupBox->setEnabled(false);
        }

        else if(ui->abdiffRadio->isChecked()) {
            ui->glpanel->ps->difference_(*(ui->glpanel->ps_temp));
            ui->glpanel->updateVoxelSet();
            ui->glpanel->ps_temp = NULL;
//            QTextStream(stderr) << "OK\n";
            ui->groupBox->setEnabled(false);
        }

        else if(ui->badiffRadio->isChecked()) {
            ui->glpanel->ps_temp->difference_(*(ui->glpanel->ps));
            ui->glpanel->ps = ui->glpanel->ps_temp;
            ui->glpanel->updateVoxelSet();
            ui->glpanel->ps_temp = NULL;
//            QTextStream(stderr) << "OK\n";
            ui->groupBox->setEnabled(false);
        }
        ui->scalePanel->setEnabled(false);
        ui->translatePanel->setEnabled(false);
    }

    else {
        QTextStream(stderr) << "OK5\n";
    }
}

void MainWindow::on_action_Save_Result_triggered()
{
    ui->glpanel->saveObj("output.obj");
}

void MainWindow::on_action3D_Line_triggered()
{
    InputDialog *inpdiag = new InputDialog();
    inpdiag->setModal(true);
    int response = inpdiag->exec();
    if (response == QDialog::Accepted) {
        int x1, x2, y1, y2, z1, z2;
//        x1 = inpdiag->ui->x1label->text();

    }
    else {
        QTextStream(stderr) << "Cancelled";
    }
}

void MainWindow::on_scaleButton_clicked()
{
    double pi180 = atan(1.0)/45;
    double sx = ui->sx->text().toDouble()*pi180;
    double sy = ui->sy->text().toDouble()*pi180;
    double sz = ui->sz->text().toDouble()*pi180;
    ui->glpanel->rotateTemp(sx, sy, sz);
}

void MainWindow::on_translateButton_clicked()
{
    double tx = ui->tx->text().toDouble();
    double ty = ui->ty->text().toDouble();
    double tz = ui->tz->text().toDouble();
    ui->glpanel->translateTemp(tx, ty, tz);
}

void MainWindow::on_actionCuboid_triggered()
{
    open("objfiles/cube.obj");
}

void MainWindow::on_actionCylinder_triggered()
{
    open("objfiles/cylinder.obj");
}

void MainWindow::on_actionCone_triggered()
{
    open("objfiles/cone.obj");
}

void MainWindow::on_actionSphere_triggered()
{
    open("objfiles/sphere.obj");
}

void MainWindow::on_rotateButton_clicked() {

}

void MainWindow::on_actionOpen_Triangle_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"~",tr("OBJ files (*.obj)"));
    set<Point3D> tempset = surfaceToVoxels(fileName.toStdString());

    VoxelSet vs;
    bool color = (ui->glpanel->ps != NULL);
    pointset temp2(tempset);
    vs.FromPointset(temp2, false);
    pointset temp(vs.createPointSet(color));

    if(ui->glpanel->ps == NULL) {
        ui->glpanel->ps = new pointset(temp);
        ui->glpanel->updateVoxelSet();
    }

    else if(ui->glpanel->ps_temp == NULL) {
        ui->glpanel->ps_temp = new pointset(temp);
        temp.union_(*ui->glpanel->ps);
        ui->glpanel->updateNewVoxels(&temp);
        ui->groupBox->setEnabled(true);
        ui->scalePanel->setEnabled(true);
        ui->translatePanel->setEnabled(true);
    }

    else {
        ui->glpanel->ps_temp = new pointset(temp);
        temp.union_(*ui->glpanel->ps);
        ui->glpanel->updateNewVoxels(&temp);
        ui->groupBox->setEnabled(true);
        ui->scalePanel->setEnabled(true);
        ui->translatePanel->setEnabled(true);
    }
}
