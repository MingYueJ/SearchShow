#ifndef BTNGROUP_H
#define BTNGROUP_H

#include <QWidget>
#include "qpushbutton.h"
namespace Ui {
class BtnGroup;
}

class BtnGroup : public QWidget
{
    Q_OBJECT

public:
    explicit BtnGroup(QWidget *parent = nullptr);
    ~BtnGroup();
    void startBegin();
    void startEnd();
    QPushButton* start;
    QPushButton* pause;
    QPushButton* terminate;
    QPushButton* reinitialize;

private:
    Ui::BtnGroup *ui;
};

#endif // BTNGROUP_H
