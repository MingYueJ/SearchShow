#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "search.h"
#include "btngroup.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void generate();//生成数据函数
    void result(int index,int count);//查询结果消息弹窗
    void start(Search* currentSearch,BtnGroup* btngroup,int page);
    void pause(Search* currentSearch,BtnGroup* btngroup);
    void terminate(Search* currentSearch,BtnGroup* btngroup);
    void reinitialize(Search* currentSearch,BtnGroup* btngroup,int page);
    void initChcek();//切换页面时初始化所有算法的选择
    void setTarget();//获取用户输入的查找值
private:
    Ui::MainWindow *ui;
    int randMax = 40;//随机数据的范围最大值
    int randMin = 1;//随机数据的范围最小值
    int max;//随机数据中的最小值
    int min;//随机数据中的最大值
    int randLengthMax = 20;//随机数据长度的范围最大值
    int randLengthMin = 15;//随机数据长度的范围最小值
    int length;//数据长度
    int target = randMax+1;//数据目标
    int *nums;//存放数据的数组
    int isready = false;//是否已生成数据
    bool isProper = false;//查找目标是否合法
};
#endif // MAINWINDOW_H
