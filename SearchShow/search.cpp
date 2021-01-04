#include "search.h"
#include "QTime"
#include "QCoreApplication"
Search::Search(QWidget *parent) : QWidget(parent)
{

}

//重新绘画
void Search::repaint(){
    //将1.5s的绘画延迟重塑成15份0.1s,更加频繁的检测自身的状态,已解决程序未及时终止便开始新一轮绘图的情况
    for (int i = 0;i<15;i++) {
        Sleep(100);
        while(ispause){
            Sleep(100);
        }
        if(isterminate)
            return;
    }
    update();
}

//自定义的延时器函数
void Search::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
