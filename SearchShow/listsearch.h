#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <QWidget>
#include "mainwindow.h"

class ListSearch : public Search
{
    Q_OBJECT
public:
    explicit ListSearch(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void binarySearch();
    void indexedSearch();
    void getsignal(bool binary,bool indexed,int length,int target,int *nums);//接收信号，决定演示哪种算法
private:
    bool isBinary = false;
    bool isIndexed = false;
    int length;
    int target;
    int *indexList;//每个索引项其中的最大值
    int *starts;//每个索引项的起始索引值
    int *lengths;//每个索引项的长度
    int *nums;//数据组
    int begin;
    int end;
    int currentIndex = 0;//当前选中的索引
    int count;//查询次数
};

#endif // BINARYSEARCH_H
