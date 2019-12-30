#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "Chess.h"
class Board : public QWidget
{
    Q_OBJECT


public:
    explicit Board(QWidget *parent = nullptr);
    ~Board();
    Chess _list[32];//存放所有的棋子的数组
    int _selectChess;//被选中的棋子
    int _d ;//棋子的直径
    int _r;//棋子的半径
    bool _redTime;//是否该红棋走
    //根据棋盘中的行列转换为像素的行列
    QPoint center(int row , int col);
    QPoint center(int i);
    //通过坐标转化为棋盘中的像素,点在棋盘外面返回false
    bool getRowCol(QPoint pt , int& row,int& col);
    //获得一个棋子的横纵坐标
    void getRowCol(int chessId, int& row , int& col);
    //计算棋子移动的时候棋子横纵坐标的特征值
    int relationNum(int row , int row1 , int col ,int col1);
    //获得2个棋子之间的象棋个数
    int getChessNumWithTwoChess(int moveId,int row,int col);
    int getChessNumWithTwoChess(int row,int row1,int col,int col1);
    //点击棋子
    virtual void click(int id , int row ,int col);
    //查找是哪一个棋子被选中了，返回棋子的编号
    int selectChecked(int row,int col);
    //绘制棋盘
    void paintEvent(QPaintEvent *);
    //绘制棋子
    void drawChess(QPainter& painter, int id);
    //鼠标事件,进行鼠标点击操作
    void mouseReleaseEvent(QMouseEvent * ev);
    //是否可以移动棋子
    void moveChess(int moveId,int row,int col);
    void moveChess(int moveid, int killid, int row, int col);
    bool canMove(int moveId,int row,int col,int killId);
    bool canMoveChe(int moveId,int row,int col,int killId);
    bool canMoveMa(int moveId,int row,int col,int killId);
    bool canMoveXiang(int moveId,int row,int col,int killId);
    bool canMoveShi(int moveId,int row,int col,int killId);
    bool canMoveJiang(int moveId,int row,int col,int killId);
    bool canMovePao(int moveId,int row,int col,int killId);
    bool canMoveBing(int moveId,int row,int col,int killId);
    //杀死棋子
    void killChess(int killId);
    //复活棋子
    void reliveChess(int reliveId);

signals:

public slots:

};

#endif // BOARD_H
