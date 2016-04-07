#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_File_triggered();

    void on_applyButton_clicked();

    void on_action_Save_Result_triggered();

    void on_action3D_Line_triggered();

    void on_scaleButton_clicked();

    void on_translateButton_clicked();

    void on_rotateButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
