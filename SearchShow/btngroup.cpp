#include "btngroup.h"
#include "ui_btngroup.h"

BtnGroup::BtnGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BtnGroup)
{
    ui->setupUi(this);
    start = ui->start;
    pause = ui->pause;
    terminate = ui->terminate;
    reinitialize = ui->reinitialize;
}

void BtnGroup::startBegin(){
    ui->pause->setText("暂停演示");
    ui->start->setText("重新演示");
    ui->reinitialize->setEnabled(false);
    ui->start->setEnabled(false);
    ui->pause->setEnabled(true);
}

void BtnGroup::startEnd(){
    ui->start->setEnabled(true);
    ui->pause->setEnabled(false);
    ui->reinitialize->setEnabled(true);
}

BtnGroup::~BtnGroup()
{
    delete ui;
}
