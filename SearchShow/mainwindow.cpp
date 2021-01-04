#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listsearch.h"
#include "QTime"
#include "QMessageBox"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("查找算法图形化");
    this->setFixedSize(1000,541);
    //初始将暂停按钮禁用
    ui->btnGroup->pause->setEnabled(false);
    ui->btnGroup_2->pause->setEnabled(false);
    ui->btnGroup_3->pause->setEnabled(false);

    //算法页面与对应的类型页面相匹配
    connect(ui->tabWidget,&QTabWidget::currentChanged,ui->stackedWidget,[=](){
       ui->stackedWidget->setCurrentIndex(ui->tabWidget->currentIndex());
       initChcek();
       terminate(ui->showBoard,ui->btnGroup);
       terminate(ui->showBoard_2,ui->btnGroup_2);
       terminate(ui->showBoard_3,ui->btnGroup_3);
    });

    //点击开始演示按钮时，发送信号
    connect(ui->btnGroup->start,&QPushButton::clicked,[=](){
        start(ui->showBoard,ui->btnGroup,1);
    });
    connect(ui->btnGroup_2->start,&QPushButton::clicked,[=](){
        start(ui->showBoard_2,ui->btnGroup_2,2);
    });
    connect(ui->btnGroup_3->start,&QPushButton::clicked,[=](){
        start(ui->showBoard_3,ui->btnGroup_3,3);
    });

    //点击暂停按钮
    connect(ui->btnGroup->pause,&QPushButton::clicked,[=](){
        pause(ui->showBoard,ui->btnGroup);
    });
    connect(ui->btnGroup_2->pause,&QPushButton::clicked,[=](){
        pause(ui->showBoard_2,ui->btnGroup_2);
    });
    connect(ui->btnGroup_3->pause,&QPushButton::clicked,[=](){
        pause(ui->showBoard_3,ui->btnGroup_3);
    });

    //点击生成数据按钮，生成数据
    connect(ui->generate,&QPushButton::clicked,[=](){
        generate();
        ui->btnGroup->start->setText("开始演示");
        ui->btnGroup_2->start->setText("开始演示");
        ui->btnGroup_3->start->setText("开始演示");
    });

    //点击终止按钮时
    connect(ui->btnGroup->terminate,&QPushButton::clicked,[=](){
        terminate(ui->showBoard,ui->btnGroup);
    });
    connect(ui->btnGroup_2->terminate,&QPushButton::clicked,[=](){
        terminate(ui->showBoard_2,ui->btnGroup_2);
    });
    connect(ui->btnGroup_3->terminate,&QPushButton::clicked,[=](){
        terminate(ui->showBoard_3,ui->btnGroup_3);
    });

    //点击重新初始化按钮
    connect(ui->btnGroup->reinitialize,&QPushButton::clicked,[=](){
       reinitialize(ui->showBoard,ui->btnGroup,1);
    });
    connect(ui->btnGroup_2->reinitialize,&QPushButton::clicked,[=](){
       reinitialize(ui->showBoard_2,ui->btnGroup_2,2);
    });
    connect(ui->btnGroup_3->reinitialize,&QPushButton::clicked,[=](){
       reinitialize(ui->showBoard_3,ui->btnGroup_3,3);
    });
}

//切换页面时清空所有算法的选择
void MainWindow::initChcek(){
    //不关掉自动排除，则无法修改按钮的checked值
    ui->binary->setAutoExclusive(false);
    ui->binary->setChecked(false);
    ui->binary->setAutoExclusive(true);

    ui->index->setAutoExclusive(false);
    ui->index->setChecked(false);
    ui->index->setAutoExclusive(true);

    ui->hash->setAutoExclusive(false);
    ui->hash->setChecked(false);
    ui->hash->setAutoExclusive(true);

    ui->balancedTree->setAutoExclusive(false);
    ui->balancedTree->setChecked(false);
    ui->balancedTree->setAutoExclusive(true);
}

//开始按钮功能的封装
void MainWindow::start(Search* currentSearch,BtnGroup* btngroup,int page){
    setTarget();
    if(isready==false||isProper==false)
        return;
    currentSearch->ispause = false;
    currentSearch->isterminate = false;
    btngroup->startBegin();
    if(page==1)
        ui->showBoard->getsignal(ui->binary->isChecked(),ui->index->isChecked(),length,target,nums);
    if(page==2)
        ui->showBoard_2->getsignal(ui->BTree->isChecked(),ui->balancedTree->isChecked(),length,target,nums);
    if(page==3)
        ui->showBoard_3->getsignal(ui->hash->isChecked(),length,target,nums);
    btngroup->startEnd();
}

//暂停按钮功能封装
void MainWindow::pause(Search* currentSearch,BtnGroup* btngroup){
    currentSearch->ispause = !currentSearch->ispause;
    if(currentSearch->ispause==true)
        btngroup->pause->setText("继续演示");
    else
        btngroup->pause->setText("暂停演示");
}

//终止按钮功能封装
void MainWindow::terminate(Search* currentSearch,BtnGroup* btngroup){
        currentSearch->isterminate = true;
        currentSearch->ispause = false;
        btngroup->pause->setText("暂停演示");
        btngroup->pause->setEnabled(false);
        btngroup->start->setEnabled(true);
        btngroup->reinitialize->setEnabled(true);
        //以白板覆盖,达到终止演示的效果
        currentSearch->update();
}

//重新初始化功能封装
void MainWindow::reinitialize(Search* currentSearch,BtnGroup* btngroup,int page){
    terminate(currentSearch,btngroup);
    generate();
    btngroup->reinitialize->setEnabled(false);
    btngroup->start->setText("开始演示");
    start(currentSearch,btngroup,page);
}

//随机生成数据
void MainWindow::generate(){
    isready = true;
    //初始化最大最小值，方便后面查询随机数据的最大最小值
    min = randMax;
    max = randMin;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    //随机生成数据长度
    length = qrand()%(randLengthMax-randLengthMin)+randLengthMin;
    nums = new int[length];
    //初始化数组
    for (int i = 0;i<length;i++) {
        nums[i] = -1;
    }
    //随机生成数据,并对数据进行去重
    int i = 0;
    while(i<length){
        bool isSame = false;
        int num = qrand()%(randMax-randMin)+randMin;
        for (int k = 0;nums[k]!=-1;k++) {
            if(num==nums[k]){
                isSame = true;
                break;
            }
        }
        if(isSame==true){
            continue;
        }
        nums[i] = num;
        if(max<nums[i])
            max = nums[i];
        if(min>nums[i])
            min = nums[i];
        i++;
    }
    //确定目标值
    target = nums[qrand()%(length-1)];

    //显示随机数据的一些信息
    ui->target->setText(QString::number(target));
    ui->length->setText(QString::number(length));
    ui->max->setText(QString::number(max));
    ui->min->setText(QString::number(min));
}

//通知查询结果的消息窗
void MainWindow::result(int index,int count){
    QMessageBox msgBox;
    if(index==1){
        msgBox.setText("查询失败！请重新设置查找目标！");
        msgBox.exec();
        return;
    }
    QString s = "查询完毕!";
    s.append("\n");
    if(index!=0){
        s.append("目标第一次出现的下标为：");
        s.append(QString::number(index));
        s.append("   ");
        s.append("\n");
    }
    s.append("查询次数为：");
    s.append(QString::number(count));
    s.append("   ");
    s.append("\n");
    msgBox.setText(s);
    msgBox.setWindowTitle("查询结果");
    msgBox.exec();
}

//获取用户输入的查找值
void MainWindow::setTarget(){
    isProper = false;
    QString str = ui->target->text();
    if(str=="")
        return;
    int target = str.toUInt();
    if(target>0)
        isProper = true;
    if(isProper == true)
        this->target = target;
    else{
        QMessageBox msgBox;
        QString s = "您输入的查找目标不合法!";
        s.append('\n');
        s.append("查找目标值必须大于0!");
        msgBox.setText(s);
        msgBox.setWindowTitle("查询错误");
        msgBox.exec();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

