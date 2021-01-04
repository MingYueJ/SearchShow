#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include "QTime"
#define FalseSearch -2147483648

class Search : public QWidget
{
    Q_OBJECT
public:
    explicit Search(QWidget *parent = nullptr);
    bool ispause = false;
    bool isterminate = false;
    void repaint();//更新绘画
    void Sleep(int msec);//自定义延时器

signals:

};

#endif // SEARCH_H
