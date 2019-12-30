#include "SingleGame.h"
#include <QTimer>
void SingleGame::click(int id, int row, int col)
{
    if(!this->_bRedTurn)
        return;

    Board::click(id, row, col);

    if(!this->_bRedTurn)
    {
        /* 启动0.1秒定时器，在0.1秒后电脑再思考 */
        QTimer::singleShot(100, this, SLOT(computerMove()));

    }
}
void SingleGame::computerMove()
{
    Step* step = getBestMove();
    moveStone(step->_moveid, step->_killid,
              step->_rowTo, step->_colTo);
    delete step;
    update();
}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    int min=16, max=32;
    if(this->_bRedTurn)
    {
        min = 0, max = 16;
    }
    for(int i=min; i<max; ++i)
    {
        if(_s[i]._dead) continue;

        for(int row=0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                int killid = this->getStoneId(row, col);
                if(sameColor(killid, i)) continue;

                if(canMove(i, killid, row, col))
                {
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
}

void SingleGame::fakeMove(Step* step)
{
    killStone(step->_killid);
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

void SingleGame::unfakeMove(Step* step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

/* 评价局面分 */
int SingleGame::calcScore()
{
    int redTotalScore = 0;
    int blackTotalScore = 0;
    //enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    static int chessScore[] = {100, 50, 50, 20, 1500, 10, 10};

    // 黑棋分的总数 - 红旗分的总数
    for(int i=0; i<16; ++i)
    {
        if(_s[i]._dead) continue;
        redTotalScore += chessScore[_s[i]._type];
    }
    for(int i=16; i<32; ++i)
    {
        if(_s[i]._dead) continue;
        blackTotalScore += chessScore[_s[i]._type];
    }

    return blackTotalScore - redTotalScore;
}

int SingleGame::getMaxScore(int level, int curMinScore)
{
    if(level == 0) return calcScore();

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);   // 是红旗的possiblemove

    int maxScore = -100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMinScore(level-1, maxScore);
        unfakeMove(step);
        delete step;

        if(score >= curMinScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if(score > maxScore)
        {
            maxScore = score;
        }

    }
    return maxScore;
}

int SingleGame::getMinScore(int level, int curMaxScore)
{


    if(level == 0) return calcScore();

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);   // 是红旗的possiblemove

    int minScore = 100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMaxScore(level-1, minScore);
        unfakeMove(step);
        delete step;

        if(score <= curMaxScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }

        if(score < minScore)
        {
            minScore = score;
        }

    }
    return minScore;
}

Step* SingleGame::getBestMove()
{
    /*
        2.试着走一下
        3.评估走的结果
        4.取最好的结果作为参考
    */

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    // 2.试着走一下
    // 3.评估走的结果
    int maxScore = -100000;
    Step* ret = NULL;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMinScore(_level-1, maxScore);
        unfakeMove(step);

        if(score > maxScore)
        {
            maxScore = score;
            if(ret) delete ret;
            ret = step;
        }
        else
        {
            delete step;
        }
    }

    // 4.取最好的结果作为参考
    return ret;
}

