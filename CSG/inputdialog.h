#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include "point3d.h"

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = 0);
    Point3D getStartPoint();
    Point3D getEndPoint();
    ~InputDialog();

private:
    Ui::InputDialog *ui;
};

#endif // INPUTDIALOG_H
