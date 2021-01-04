
#ifndef TREESEARCH_H
#define TREESEARCH_H
#define ORDER 3
#include <QWidget>
#include "mainwindow.h"
#include "search.h"
#include "QTime"
#include <QQueue>



class TreeSearch : public Search
{

    Q_OBJECT
    struct bnode{
      int data;
      bnode* leftC = NULL;
      bnode* rightC = NULL;
      int level = 0;//节点的层数
      int bf = 0;//平衡因子
    };


    const int Max=ORDER-1; //结点的最大关键字数量
    const int Min=(ORDER-1)/2;  //结点的最小关键字数量

    struct Bnode{                //B树和B树结点类型
        int keynum;                 //结点关键字个数
        int key[ORDER+1];              //关键字数组，key[0]不使用
        struct Bnode *parent;            //双亲结点指针
        struct Bnode *ptr[ORDER+1];         //孩子结点指针数组
        Bnode(){
            for (int i = 0; i < ORDER+1 + 1; i++)
            {
                ptr[i] = NULL;
                key[i] = -1;
            }
            keynum = 0;
            parent = NULL;
        }
    };
public:
    explicit TreeSearch(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);//绘图事件
    void balancedTreeSearch();//平衡二叉树查找数据处理
    void BTreeSearch();//B树查找数据处理
    void getsignal(bool isBTree,bool isbalancedTree,int length,int target,int *nums);//接收信号，决定演示哪种算法
    int SearchNode(bnode* &target,int data);//查找到data数据的父节点
    void paint(int x,int y,bnode* n);//给当前节点画左右两个儿子节点的图
    void Bpaint(int x,int y,int px,int py,int index,Bnode* b);//给B树的当前节点画儿子节点的图
    int getHeight(bnode* t);//获取以当前节点为根的二叉树的高度
    void setBF(bnode* t);//更新整棵树每个节点的平衡因子
    void getBf(bnode* &target);//获取平衡因子绝对值超过1的最高层次的那个节点（注意，这里我没有采用找层次最低的平衡点，因为有个BUG实在无法定位修复，所以采取了消耗更大的找层次最高的平衡点）
    void correct(bnode* begin,bnode* target);//判断当前错误属于什么型，并执行相应的修复函数
    void getLevel(bnode* t);//更新所有节点的层次数
    void setIndex(Bnode* &b,int num);//给新关键字找到合适的位置插入
    void reset(Bnode* &b);//重置调整B树
    void SplitBnode(Bnode *&p,Bnode *&q);//将结点p分裂成两个结点,前一半保留,后一半移入结点q
    void dfs(double x,double y,double px,double py,int index,int level,Bnode* b);//深度优先遍历B树,并调用绘画
    void getMaxLevel(Bnode* b);//获取B树的最大深度
    void LL(bnode* t);
    void RR(bnode* t);
    void LR(bnode* t);
    void RL(bnode* t);
private:
    bnode* root;//二叉树的根节点
    Bnode* Broot;//B树根节点
    bool isbalancedTree = false;
    bool isBTree = false;
    int length;
    int target;
    int currentIndex;
    int *nums;
    int count;//查询次数
    int r = 20;//圆的半径
    double angle = 30;//初始偏转角度
    double addangel = 7.5;//每一层改变的偏转角度量
    QQueue<int> q;//存储每个节点的x,y坐标
    QQueue<bnode*> node;//存储节点
    QQueue<bnode*> node_2;//存储节点，用于广度优先遍历
    int w = 26;//B树关键字节点的宽度
    int h = 20;//B树关键字节点的高度
    int maxLevel;//B树的最大深度
};

#endif // TREESEARCH_H
