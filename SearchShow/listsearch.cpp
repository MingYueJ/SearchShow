#include "listsearch.h"
#include "QPainter"
#include "QDebug"
#include "mainwindow.h"
ListSearch::ListSearch(QWidget *parent) : Search(parent)
{

}
//绘图事件
void ListSearch::paintEvent(QPaintEvent *){
     QPainter painter;
     painter.begin(this);
     //手动绘制边框
     painter.setPen(QColor(139, 139, 139));
     painter.drawLine(0, 0, this->width() - 1, 0);
     painter.drawLine(0, 0, 0, this->height() - 1);
     painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
     painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);

     //用于终止演示时
     if(isterminate){
         return;
     }
     //计算每个节点在图上出现的位置
     int width = this->width()/length;
     int height = width/2;
     int x = 0;
     int y = this->height()/2-height/2;

     //当选择为二分查找算法时并开始运行时，开始绘图
     if(isBinary){
        //绘图
        for(int i = 0;i<length;i++){
            QRect rect(x,y,width,height);
            painter.drawRect(rect);
            if(target==nums[i])
                painter.fillRect(rect,Qt::SolidPattern);
            if(begin==i)
                painter.fillRect(rect,QColor("#33FFFF"));
            if(end==i)
                painter.fillRect(rect,QColor("#FFCC22"));
            //数据绘图
            painter.drawText(x+width/2-5,y+height/2+5,QString::number(nums[i]));
            x += width;
        }
     }

     //索引表法的绘图
     if(isIndexed){
         int index = 0;
         if(indexList[0]>10)
            index++;
         for(int i = 0;i<length;i++){
             QRect rect(x,y,width,height);
             painter.drawRect(rect);
             if(target==nums[i])
                 painter.fillRect(rect,Qt::SolidPattern);
             if(i==currentIndex)
                 painter.fillRect(rect,QColor("#33FFFF"));
             //数据绘图
             painter.drawText(x+width/2-5,y+height/2+5,QString::number(nums[i]));
             //对应某个索引项的开始索引值的时候，画椭圆圈起来
             if(i==starts[index]){
                 int h = width*lengths[index]/2;
                 int w = width*lengths[index];
                 int y = this->height()/2-h/2;
                 painter.drawEllipse(x,y,w,h);
                 index++;
             }
             x += width;
         }

     }
     painter.end();
}

//接收开始按钮的信号,根据参数决定演示哪种查找
void ListSearch::getsignal(bool binary,bool indexed,int length,int target,int *nums){
    isBinary = binary;
    isIndexed = indexed;
    this->nums = nums;
    this->length = length;
    this->target = target;
    //对数据进行排序，这里采用冒泡排序
    for (int i = 0;i<length;i++) {
        for (int k = 0;k<length-i-1;k++) {
            if(nums[k]>nums[k+1]){
                int t = nums[k];
                nums[k] = nums[k+1];
                nums[k+1] = t;
            }
        }
    }
    //二分查找
    if(isBinary){
        update();
        binarySearch();
    }
    //索引表查询
    if(isIndexed){
        update();
        indexedSearch();
    }
}

//索引表查找的数据处理
void ListSearch::indexedSearch(){
    MainWindow m;
    //判断查找数据是否合法
    currentIndex = 0;
    indexList = new int[length];
    starts = new int[length];
    lengths = new int[length];
    count = 0;
    for (int k = 0;k<length;k++) {
        lengths[k] = 0;
    }
    int t;
    int last = 0;
    int index = 0;

    //索引表分表
    for (int i = 0;i<length;i++) {
        t = nums[i];
        if(t<10){
            starts[index] = 0;
            indexList[index] = nums[i];
            lengths[index]++;
            continue;
        }
        t = t/10;
        if(t!=last){
            index++;
            starts[index] = i;
            indexList[index] = nums[i];
            lengths[index]++;
        }else{
            indexList[index] = nums[i];
            lengths[index]++;
        }
        last = t;
    }

    if(target<nums[0]||target>nums[length-1]){
        m.result(1,0);
        return;
    }

    //搜寻合适的索引项
    for (int i = 0;i<length;i++) {
        currentIndex = starts[i];
        count++;
        if(nums[currentIndex]>target){
            m.result(1,0);
            return;
        }
        repaint();
        if(isterminate)
            return;
        if(target<=indexList[i]){
            break;
        }
    }

    //如果一开始就找到了，直接输出
    if(target==nums[currentIndex]&&isterminate==false){
        m.result(currentIndex,count);
    }

    //在某个索引项中开始查找目标
    while(target!=nums[currentIndex]){
        currentIndex++;
        count++;
        if(nums[currentIndex]>target){
            m.result(1,0);
            return;
        }
        repaint();
        if(isterminate)
            return;
        if(target==nums[currentIndex]&&isterminate==false){
            m.result(currentIndex,count);
            break;
        }
    }
}

//二分查找的数据处理
void ListSearch::binarySearch(){
    begin = 0;
    end = length-1;
    count = 0;
    MainWindow m;
    if(target>nums[end]||target<nums[begin]){
        m.result(1,0);
        return;
    }
    while(begin<=end){
        count++;
        repaint();
        if(isterminate)
            return;
        if(target==nums[begin]){
            m.result(begin,count);
            break;
        }
        if(target==nums[end]){
            m.result(end,count);
            break;
        }
        if(target>nums[(end+begin)/2]){
            begin = (end+begin)/2+1;
            update();
            continue;
        }
        if(target<nums[(end+begin)/2]){
            end = (end+begin)/2-1;
            update();
            continue;
        }
        if(target==nums[(end+begin)/2]){
            m.result((end+begin)/2,count);
            break;
        }
    }
    if(begin>end){
        m.result(1,0);
    }
}

