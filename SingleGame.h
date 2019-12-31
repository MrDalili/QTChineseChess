#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"
#include "Step.h"

class SingleGame : public Board
{
    Q_OBJECT
public:
    SingleGame(){
        _level = 6;
    }
    //电脑需要考虑的层次
    int _level;

    //重载click函数
    void click(int id , int row ,int col);
    //实现电脑走
    Step * getBestMove();
    //所有可以走的步骤
    void getAllPossibleMove(QVector<Step*>& steps);
    //向指定的数组中存储指定的步骤
    void saveStep(int moveId ,int killId,int row ,int col ,QVector<Step*>& steps);
    //内部走一步
    void fakeMove(Step* step);
    //内部返回
    void unfakeMove(Step* step);
    //计算当前局面得分
    int calcScore();
    //简单粗暴，只判断自己的局面分
    Step * getBestMoveByForce();
    //最大最小值算法
    Step * getBestMoveByMaxMin();
    int getMinScore(int level, int curMaxScore);
    int getMaxScore(int level, int curMinSocer);
public slots:
    void computerMove();
};

#endif // SINGLEGAME_H
