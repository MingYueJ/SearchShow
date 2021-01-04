#ifndef HASHSEARCH_H
#define HASHSEARCH_H

#include <QWidget>
#include "mainwindow.h"

class HashSearch : public Search
{
    Q_OBJECT
    struct node{
      int data = NULL;
      node* next = NULL;
      bool isSearched = false;
    };
    struct bucket{
      int index = NULL;
      node* node = NULL;
      bucket* next = NULL;
    };
public:
    explicit HashSearch(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void hashSearch();
    void getsignal(bool isHashSearch,int length,int target,int *nums);//接收信号，决定演示哪种算法
    void searchBucket(bucket* &t,int index);//根据index搜寻对应的bucket
private:
    bool isfirst = true;//是否是第一次绘图
    bool ishash = false;
    int *nums;
    int buckedtLength = 7;
    int bucketIndex;
    int length;
    int target;
    int currentIndex;
    int count;//查询次数
    bucket *firstBucket;

signals:

};

#endif // HASHSEARCH_H
