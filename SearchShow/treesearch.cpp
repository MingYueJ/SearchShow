#include "treesearch.h"
#include "QPainter"
#include "math.h"
#include "QDebug"
#include <QQueue>
#include "math.h"
TreeSearch::TreeSearch(QWidget *parent) : Search(parent)
{

}

//绘图事件
void TreeSearch::paintEvent(QPaintEvent *){
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


    if(isbalancedTree){
        int x = this->width()/2;
        int y = r;
        bnode *n;
        //画根节点
        node.enqueue(root);
        painter.setPen(Qt::black);
        if(root->data==target){
            painter.setPen(Qt::blue);
        }
        if(root->data==currentIndex){
            painter.setPen(Qt::red);
        }
        painter.drawEllipse(QPoint(x,y),r,r);
        painter.drawText(QPoint(x-8,y+4),QString::number(root->data));
        q.enqueue(this->width()/2);
        q.enqueue(r);
        while(!q.isEmpty()){
            n = node.dequeue();
            x = q.dequeue();
            y = q.dequeue();
            paint(x,y,n);
        }
    }

    if(isBTree){
        Bnode* b = Broot;
        int x = this->width()/2 - w*b->keynum/2;
        int y = 0;
        int px = x;
        int py = y;
        maxLevel = 0;
        getMaxLevel(Broot);
        dfs(x,y,px,py,0,1,b);
    }

    painter.end();
}

//给当前节点画左右两个儿子节点的图
void TreeSearch::paint(int x,int y,bnode* n){
    QPainter painter;
    painter.begin(this);
    int lx = x-8*r/pow(2,n->level-1);
    int ly = y+(x-lx)*tan((angle+(n->level)*addangel)*3.1415/180);
    int rx = x+8*r/pow(2,n->level-1);
    int ry = ly;
    if(n->leftC!=NULL){
        painter.drawLine(QPoint(x,y),QPoint(lx,ly));
        if(n->leftC->data==target){
            painter.setPen(Qt::blue);
        }
        if(n->leftC->data==currentIndex){
            painter.setPen(Qt::red);
        }
        painter.drawEllipse(QPoint(lx,ly),r,r);
        painter.drawText(QPoint(lx-8,ly+4),QString::number(n->leftC->data));

        q.enqueue(lx);
        q.enqueue(ly);
        node.enqueue(n->leftC);
    }
    painter.setPen(Qt::black);
    if(n->rightC!=NULL){
        painter.drawLine(QPoint(x,y),QPoint(rx,ry));
        if(n->rightC->data==target){
            painter.setPen(Qt::blue);
        }
        if(n->rightC->data==currentIndex){
            painter.setPen(Qt::red);
        }
        painter.drawEllipse(QPoint(rx,ry),r,r);
        painter.drawText(QPoint(rx-8,ry+4),QString::number(n->rightC->data));
        q.enqueue(rx);
        q.enqueue(ry);
        node.enqueue(n->rightC);
    }
    painter.end();
}

//接收开始按钮的信号,根据参数决定演示哪种查找
void TreeSearch::getsignal(bool isBTree,bool isbalancedTree,int length,int target,int *nums){
    this->isbalancedTree = isbalancedTree;
    this->isBTree = isBTree;
    this->nums = nums;
    this->length = length;
    this->target = target;

    //平衡树
    if(this->isbalancedTree){
        update();
        balancedTreeSearch();
    }

    //B树
    if(this->isBTree){
        update();
        BTreeSearch();
    }
}

//平衡二叉树查找数据处理
void TreeSearch::balancedTreeSearch(){
    MainWindow m;
    root = new bnode;
    root->data = nums[0];
    root->level = 1;
    root->bf = 0;
    bnode* t;
    count = 1;
    int l;

    //根据数据生成二叉树
    for (int i = 1;i<length;i++) {
        bnode* begin = NULL;
        bnode* newNode = new bnode;
        newNode->data = nums[i];
        l = SearchNode(t,nums[i]);
        if(nums[i]>t->data){
            t->rightC = newNode;
        }else{
            t->leftC = newNode;
        }
        newNode->level = l+1;
        setBF(root);
        getBf(begin);
        if(begin!=NULL){
            correct(begin,newNode);
        }
    }
    //更新所有节点的层次
    getLevel(root);
    update();
    //查找开始
    currentIndex = root->data;
    repaint();
    if(isterminate==true){
        return;
    }
    bnode* n = root;
    while(target!=currentIndex&&n!=NULL){
        if(target>n->data){
            n = n->rightC;
        }else if(target<n->data){
            n = n->leftC;
        }
        if(n==NULL){
            currentIndex = FalseSearch;
        }else{
            currentIndex = n->data;
        }
        count++;
        repaint();
        if(isterminate==true){
            return;
        }
    }
    if(currentIndex==FalseSearch&&isterminate==false){
        m.result(1,count);
    }
    if(currentIndex!=FalseSearch&&isterminate==false){
        m.result(0,count);
    }
}

//查找到data数据的父节点
int TreeSearch::SearchNode(bnode* &target,int data){
    bnode* t = root;
    int level = 1;
    while(t!=NULL&&t->data!=data){
        target = t;
        if(data>t->data){
            t = t->rightC;
        }else{
            //如果节点值与数据相等，统一放右边
            t = t->leftC;
        }
        level++;
    }
    return level;
}

//获取以当前节点为根的二叉树的高度
int TreeSearch::getHeight(bnode* t){
    if(t==NULL){
        return 0;
    }
    return qMax(getHeight(t->leftC)+1,getHeight(t->rightC)+1);
}

//更新整棵树每个节点的平衡因子
void TreeSearch::setBF(bnode* t){
    if(t!=NULL){
        t->bf = getHeight(t->leftC) - getHeight(t->rightC);
        setBF(t->leftC);
        setBF(t->rightC);
    }
}

//获取平衡因子绝对值超过1的最高层次的那个节点（注意，这里我没有采用找层次最低的平衡点，因为有个BUG实在无法定位修复，所以采取了消耗更大的找层次最高的平衡点）
void TreeSearch::getBf(bnode* &target){
    bnode* t;
    node_2.enqueue(root);
    while(!node_2.isEmpty()){
        t = node_2.dequeue();
        if(qAbs(t->bf)>1){
            target = t;
        }
        if(t->leftC!=NULL)
            node_2.enqueue(t->leftC);
        if(t->rightC!=NULL)
            node_2.enqueue(t->rightC);
    }
}

//判断当前错误属于什么型，并执行相应的修复函数
void TreeSearch::correct(bnode* begin,bnode* target){
    if(begin->leftC==NULL)
        qDebug()<<"";
    if(target->data > begin->data){
        if(begin->rightC!=NULL && target->data > begin->rightC->data){
            RR(begin);
        }else{
            RL(begin);
        }
    }else{
        if(begin->leftC!=NULL && target->data < begin->leftC->data){
            LL(begin);
        }else{
            LR(begin);
        }
    }
}

void TreeSearch::LL(bnode* t){
    bnode* begin = t;
    bnode* mid = begin->leftC;
    if(mid->rightC!=NULL){
        begin->leftC = mid->rightC;
        mid->rightC = begin;
    }else{
        begin->leftC = NULL;
        mid->rightC = begin;
    }
    //如果根节点参与了旋转，需要跳转root的指向
    if(begin==root){
        root = mid;
    }else{
        bnode* p;
        SearchNode(p,begin->data);
        if(p->leftC==begin)
            p->leftC = mid;
        else
            p->rightC = mid;
    }
}

void TreeSearch::RR(bnode* t){
    bnode* begin = t;
    bnode* mid = begin->rightC;
    if(mid->leftC!=NULL){
        begin->rightC = mid->leftC;
        mid->leftC = begin;
    }else{
        begin->rightC = NULL;
        mid->leftC = begin;
    }
    //如果根节点参与了旋转，需要跳转root的指向
    if(begin==root){
        root = mid;
    }else{
        bnode* p;
        SearchNode(p,begin->data);
        if(p->leftC==begin)
            p->leftC = mid;
        else
            p->rightC = mid;
    }
}

void TreeSearch::LR(bnode* t){
    bnode* begin = t;
    bnode* mid = begin->leftC;
    bnode* end = mid->rightC;
    if(end->leftC!=NULL){
        begin->leftC = end;
        mid->rightC = end->leftC;
        end->leftC = mid;
    }else{
        begin->leftC = end;
        mid->rightC = NULL;
        end->leftC = mid;
    }
    LL(begin);
}

void TreeSearch::RL(bnode* t){
    bnode* begin = t;
    bnode* mid = begin->rightC;
    bnode* end = mid->leftC;
    if(end->rightC!=NULL){
        begin->rightC = end;
        mid->leftC = end->rightC;
        end->rightC = mid;
    }else{
        begin->rightC = end;
        mid->leftC = NULL;
        end->rightC = mid;
    }
    RR(begin);
}

//更新所有节点的层次数
void TreeSearch::getLevel(bnode* t){
    if(t==NULL){
        return;
    }
    int level = 1;
    bnode* curNode = root;
    while(curNode->data!=t->data){
        if(t->data>curNode->data){
            curNode = curNode->rightC;
        }else{
            curNode = curNode->leftC;
        }
        level++;
    }
    t->level = level;
    getLevel(t->leftC);
    getLevel(t->rightC);
}

//B树的数据处理
void TreeSearch::BTreeSearch(){
    Broot = new Bnode;
    Broot->key[1] = nums[0];
    Broot->keynum = 1;
    for (int i = 1;i<length;i++) {
        Bnode* b = Broot;
        for (int j = 0;j<=ORDER+1;j++) {
            if(((nums[i] < b->key[j+1]) || (nums[i] > b->key[j+1] && b->key[j+1]==-1)) && b->ptr[j]!=NULL){
                b = b->ptr[j];
                j = -1;
                continue;
            }
            if(nums[i] < b->key[j+1]&&b->ptr[j]==NULL){
                setIndex(b,nums[i]);
                break;
            }
            if(b->key[j]==-1 && j!=0){
                b->key[j]=nums[i];
                b->keynum++;
                break;
            }
        }
        if(b->keynum == Max+1){
            reset(b);
        }
    }
    update();
    //在B树中开始搜索
    Bnode* b = Broot;
    MainWindow m;
    count = 0;

    for (int j = 0;j<=ORDER+1;j++) {
        if(b==NULL){
            currentIndex = -999;
            repaint();
            if(isterminate==true){
                return;
            }
            m.result(1,count);
            return;
        }
        if( b->key[j] < target && target < b->key[j+1] ){
            currentIndex = b->key[j+1];
            count++;
            repaint();
            if(isterminate==true){
                return;
            }
            b = b->ptr[j];
            j = -1;
            continue;
        }
        if( b->key[j+1] < target && ( b->key[j+2]==-1 || j+1==ORDER )){
            currentIndex = -b->key[j+1]-1;
            count++;
            repaint();
            if(isterminate==true){
                return;
            }
            b = b->ptr[j+1];
            j = -1;
            continue;
        }
        if(target==b->key[j+1]){
            currentIndex = target;
            count++;
            repaint();
            if(isterminate==true){
                return;
            }
            m.result(0,count);
            return;
        }
    }


}

//给新关键字找到合适的位置插入
void TreeSearch::setIndex(Bnode* &b,int num){
    for (int i = 1;i<ORDER+1;i++) {
        if(b->key[i]>num&&b->key[i]!=-1){
            for (int k = ORDER;k>=i;k--) {
                b->key[k] = b->key[k-1];
            }
            b->key[i] = num;
            break;
        }
        if(b->key[i]==-1){
            b->key[i] = num;
            break;
        }
    }
    b->keynum++;
}

//重置调整B树
void TreeSearch::reset(Bnode* &b){
    Bnode* newChild = new Bnode;
    Bnode* parent;
    if(b->parent==NULL){
        parent = new Bnode;
        parent->key[1] = b->key[ORDER/2+1];
        parent->keynum = 1;
        SplitBnode(b,newChild);
        parent->ptr[0] = b;
        b->parent = parent;
        parent->ptr[1] = newChild;
        newChild->parent = parent;
        Broot = parent;
    }else{
        parent = b->parent;
        setIndex(parent,b->key[ORDER/2+1]);
        SplitBnode(b,newChild);
        for (int i = 0;i<ORDER+1;i++) {
            if(parent->ptr[i]==b){
                for (int k = ORDER; k>i; k--) {
                    parent->ptr[k+1] = parent->ptr[k];
                }
                parent->ptr[i+1] = newChild;
                newChild->parent = parent;
                break;
            }
        }
        if(parent->keynum==Max+1){
            reset(parent);
        }
    }
}

//将结点p分裂成两个结点,前一半保留,后一半移入结点q
void TreeSearch::SplitBnode(Bnode *&p,Bnode *&q){
    int index = 0;
    p->key[ORDER/2+1] = -1;
    p->keynum--;
    for (int i = ORDER/2+2;i<ORDER+1;i++) {
        if(p->key[i]!=-1){
            q->key[index+1] = p->key[i];
            q->keynum++;
            q->ptr[index] = p->ptr[i-1];
            p->ptr[i-1] = NULL;
            p->key[i] = -1;
            p->keynum--;
            index++;
        }
    }
    q->ptr[index] = p->ptr[ORDER];
    p->ptr[ORDER] = NULL;
    //将新节点的儿子节点的父亲节点重置为新结点自己
    for (int i = 0;i<ORDER;i++) {
        if(q->ptr[i]!=NULL)
            q->ptr[i]->parent = q;
    }
}

//给B树的当前节点画儿子节点的图
void TreeSearch::Bpaint(int x,int y,int px,int py,int index,Bnode* b){
    QPainter painter;
    painter.begin(this);

    int ww;
    int wh = h/2;
    int wx;
    int wy = y+h;
    int nums = b->keynum+1;
    ww = w*b->keynum/nums;
    wx = x;
    if(b!=Broot){
        int piancha = (w*b->parent->keynum/(b->parent->keynum+1))/2;
        painter.drawLine(px+w*b->parent->keynum/(b->parent->keynum+1)*(index+1)-piancha,py+h*5/4,x+w*b->keynum/2,y);
    }
    bool flag = false;
    for (int i = 0;i<nums;i++) {
        if(i!=nums-1){
            QRect rect(x,y,w,h);
            painter.drawRect(rect);
            QString s = "#FFCC22";
            if(target==currentIndex)
                s = "#33FFFF";
            if(target==b->key[i+1])
                painter.fillRect(rect,QColor(s));
            painter.drawText(x+6,y+h-4,QString::number(b->key[i+1]));
            x += w;
        }
        QRect rect(wx,wy,ww,wh);
        painter.drawRect(rect);
        wx += ww;
        if(target!=currentIndex){
            if(currentIndex==b->key[i+1])
               painter.fillRect(rect,QColor("#33FFFF"));
            if(currentIndex + b->key[i+1] == -1){
               flag = true;
               continue;
            }
            if(flag){
                painter.fillRect(rect,QColor("#33FFFF"));
            }
        }
    }


    painter.end();
}

//深度优先遍历B树,并调用绘画
void TreeSearch::dfs(double x,double y,double px,double py,int index,int level,Bnode* b){
    if(b==NULL){
        return;
    }
    Bpaint(x,y,px,py,index,b);
    px = x;
    py = y;
    y += 4*h;
    double newX;
    double change;
    level++;
    if(maxLevel==3){
        if(level==2){
            change = (double)7.5*w;
        }
        if(level==3){
            change = (double)2.5*w;
        }
    }
    if(maxLevel==4){
        if(level==2){
            change = (6+(double)7/12)*w;
        }
        if(level==3){
            change = (3+(double)1/4)*w;
        }
        if(level==4){
            change = (2+(double)1/6)*w;
        }
    }
    for (int i = 0;i<b->keynum+1;i++) {
        newX = x;
        if(b->keynum==1){
            if(i == 0){
                newX = newX-change;
            }
            if(i == 1){
                newX = newX+change;
            }
        }else{
            if(i == 0){
                newX = newX-change;
            }
            if(i == 2){
                newX = newX+change;
            }
        }
        dfs(newX,y,px,py,i,level,b->ptr[i]);
    }

}

//获取B树的最大深度
void TreeSearch::getMaxLevel(Bnode* b){
    if(b!=NULL){
        maxLevel++;
        getMaxLevel(b->ptr[0]);
    }
}




