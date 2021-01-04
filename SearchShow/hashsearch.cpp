#include "hashsearch.h"
#include "QPainter"
#include "QDebug"
HashSearch::HashSearch(QWidget *parent) : Search(parent)
{

}

//获取信号
void HashSearch::getsignal(bool isHashSearch,int length,int target,int *nums){
    ishash = isHashSearch;
    currentIndex = -1;
    count = 0;
    this->nums = nums;
    this->length = length;
    this->target = target;
    //散列表查找
    if(ishash){
        update();
        hashSearch();
    }
}

//散列表查找对数据的处理
void HashSearch::hashSearch(){
    MainWindow m;
    firstBucket = new bucket;
    bucket* p = firstBucket;
    bucket* last = NULL;
    for (int i = 0;i<buckedtLength;i++) {
        if(p!=firstBucket)
            p = new bucket;
        p->index = i;
        p->node = new node;
        p->node->isSearched = false;
        if(last!=NULL)
            last->next = p;
        last = p;
        p = p->next;
    }
    node* t;
    node* currentNode;
    bucket* n;
    //数据处理,为每个散列项加上后面的节点
    for (int i = 0;i<length;i++) {
        int bucketIndex = nums[i];
        while(bucketIndex>buckedtLength-1){
            bucketIndex = bucketIndex%buckedtLength;
        }
        t = new node;
        t->isSearched = false;
        t->data = nums[i];
        searchBucket(n,bucketIndex);
        currentNode = n->node;
        while(currentNode->next!=NULL){
            currentNode = currentNode->next;
        }
        currentNode->next = t;
    }
    //确定目标所在的buckedIndex
    bucketIndex = target;
    while(bucketIndex>buckedtLength-1){
        bucketIndex = bucketIndex % buckedtLength;
    }
    //第一次绘图，打上第一次绘图的标记
    isfirst = true;
    repaint();

    //这里的currentIndex代表当前指向的节点
    searchBucket(n,bucketIndex);
    node* index = n->node;
    currentIndex = index->data;
    count++;
    while(index->data!=target){
        index = index->next;
        count++;
        if(index==NULL)
            break;
        currentIndex = index->data;
        repaint();
    }
    if(index==NULL){
        currentIndex = FalseSearch;
    }
    if(currentIndex==FalseSearch&&isterminate==false){
        m.result(1,count);
    }
    if(currentIndex!=FalseSearch&&isterminate==false){
        m.result(0,count);
    }
}

//从buckets中找到指定的节点
void HashSearch::searchBucket(bucket* &t,int index){
    bucket* n = firstBucket;
    while(n!=NULL){
        if(n->index==index){
            t = n;
            return;
        }
        n = n->next;
    }
}

//绘画事件
void HashSearch::paintEvent(QPaintEvent *){
    QPainter painter;
    painter.begin(this);
    //手动绘制边框
    painter.setPen(QColor(139, 139, 139));
    painter.drawLine(0, 0, this->width() - 1, 0);
    painter.drawLine(0, 0, 0, this->height() - 1);
    painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
    painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);

    //用于终止演示
    if(isterminate){
        return;
    }

    if(ishash){
        int height = this->height()/buckedtLength;
        int width = height;
        int lineLength = width/2;
        int x = 0;
        int y = 0;
        bucket * n;
        //判断当前节点是否为头节点
        bool ishead = true;
        //判断当前行是否已经完成当前节点的指向转移
        bool isget = false;
        //每循环一次绘制一行
        for (int i = 0;i<buckedtLength;i++) {
            searchBucket(n,i);
            node* t = n->node;
            //每循环一次画一个矩形
            while(t!=NULL){
                QRect rect(x,y,width,height);
                painter.save();
                if(t->next!=NULL)
                    painter.drawLine(QPointF(x+width,y+height/2), QPointF(x+width+lineLength,y+height/2));
                if(target==t->data)
                    painter.fillRect(rect,Qt::SolidPattern);
                if(currentIndex==t->data&&isfirst==false&&isget==false&&t->data!=NULL&&t->isSearched==false){
                    painter.fillRect(rect,QColor("#00DDDD"));
                    t->isSearched = true;
                    isget = true;
                }
                if(bucketIndex==i&&isfirst){
                    isfirst = false;
                    painter.fillRect(rect,QColor("#00DDDD"));
                }
                if(ishead){
                    painter.drawText(x+width/2-5,y+height/2+5,QString::number(n->index));
                    painter.setPen(QColor("#FFBB00"));
                    ishead = false;
                }else{
                    painter.drawText(x+width/2-5,y+height/2+5,QString::number(t->data));
                }
                painter.drawRect(rect);
                painter.restore();
                t = t->next;
                x = x+width+lineLength;
            }
            ishead = true;
            isget = false;
            x = 0;
            y = y+height;
        }
    }
    painter.end();
}


