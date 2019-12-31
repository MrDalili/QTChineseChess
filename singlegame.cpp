#include "SingleGame.h"
#include <QTimer>
#include <QDebug.h>
void SingleGame::click(int id , int row ,int col){
    //防止黑瞎走
    if(!this->_redTime){
        return;
    }

    Board::click(id,row,col);

    //加判断看是人走还是电脑走
    if(!this->_redTime){
        //启动一个定时器0.1秒
        //让棋盘刷新一下，再开始思考如何去走
        QTimer::singleShot(100, this , SLOT(computerMove()));
    }
}

//记录所有可能移动的棋子
void SingleGame::getAllPossibleMove(QVector<Step*>& steps){
    //在不同的人玩的时候计算的分数不一样
    //红走
    int min = 16 ,max = 32;
    //黑走
    if(!this->_redTime){
        min = 0 ;
        max = 16;
    }
    for (int i = min ; i< max; ++i) {
        //死棋不可移动
        if(_list[i]._die) continue;
        //对于每一个棋子来说，都去看一下对应的剩余棋盘地方能不能移动
        for (int row = 0 ; row <= 9 ; ++row) {
            for (int col = 0 ; col <= 8; ++col) {
                //看看目标行列上有没有棋子
                int killId = this->selectChecked(row,col);
                if(killId != -1){
                    //如果颜色相同
                    if(_list[i]._red == _list[killId]._red ){
                        //那就con'tintue
                        continue;
                    }
                }
                //如果可以移动
                if(canMove(i,row,col,killId)){
                    //存起来
                    saveStep(i , killId, row ,col ,steps);
                }
            }
        }
    }
}

Step * SingleGame::getBestMove(){
    //使用最大最小值算法计算
    /*
        原理：
            先计算机自己走，算出来自己所有棋子里面的最大值，再去计算人走，最好的那一步
    */
    return getBestMoveByMaxMin();
}

//最大最小值
Step * SingleGame::getBestMoveByMaxMin(){
    //看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    //试着走一下
    //初始化一个最大的得分
    int maxScore = -1000000;
    //记录当前得分最高的步骤
    Step* good = nullptr;
    while (steps.count()) {
        //防止内存溢出
        Step * step = steps.back();
        steps.removeLast();


        //程序走一步看看局面
        fakeMove(step);
        //计算局面分
        int score = getMinScore(_level,maxScore);
        //把棋子移动回来
        unfakeMove(step);


        //把分数记录下来,并且记录步骤
        if(maxScore < score){
            maxScore = score;
            if(good) delete good;
            good = step;
        }else {
            delete step;
        }

    }
    //评估一下走的结果
    //区最好的结果
    return good;
}
//
int SingleGame::getMinScore(int level ,int curMaxScore){
    //如果不需要计算机了，那就直接计算当前局面分
    if(level == 0) return calcScore();
    //看看可以走哪一些步骤
    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int minScore = 1000000;
    while (steps.count()) {
        Step * step = steps.back();
        steps.removeLast();
        //程序走一步看看局面
        fakeMove(step);
        //计算局面分
        int score = getMaxScore(level - 1, minScore);
        //把棋子移动回来
        unfakeMove(step);

        //剪枝优化
        if(score <= curMaxScore){
            while (steps.count()) {
                delete step;
                step = steps.back();
                steps.removeLast();
            }
            return score;
        }
        //把分数记录下来,并且记录步骤
        if(score < minScore){
            minScore = score;
        }
        delete step;
    }
    return minScore;

}

int SingleGame::getMaxScore(int level, int curMinScore){
    //如果不需要计算了，那就直接计算当前局面分
    if(level == 0) return calcScore();
    //看看可以走哪一些步骤
    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int maxScore = -1000000;
    while (steps.count()) {
        Step * step = steps.back();
        steps.removeLast();
        //程序走一步看看局面
        fakeMove(step);
        //计算局面分
        int score = getMinScore(level - 1, maxScore);
        //把棋子移动回来
        unfakeMove(step);


        //剪枝优化
        if(score >= curMinScore){
            while (steps.count()) {
                delete step;
                step = steps.back();
                steps.removeLast();
            }
            return score;
        }


        //把分数记录下来,并且记录步骤
        if(score > maxScore){
            maxScore = score;
        }
        delete step;
    }
    return maxScore;
}
//暴力
Step * SingleGame::getBestMoveByForce(){
    //看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    //试着走一下
    //初始化一个最大的得分
    int maxScore = -1000000;
    //记录当前得分最高的步骤
    Step* good;
    for (QVector<Step*>::iterator it = steps.begin(); it != steps.end() ; ++it) {
        Step* step = *it;
        //程序走一步看看局面
        fakeMove(step);
        //计算局面分
        int score = calcScore();
        //把棋子移动回来
        unfakeMove(step);
        //把分数记录下来,并且记录步骤
        if(maxScore < score){
            maxScore = score;
            good = step;
        }
    }
    //评估一下走的结果
    //区最好的结果
    return good;
}

//储存步骤
void SingleGame::saveStep(int moveId ,int killId,int row ,int col ,QVector<Step*>& steps){
    int row1,col1;
    //获取当前棋子的坐标
    this->getRowCol(moveId,row1,col1);
    Step * step = new Step;
    step->_colFrom = col1;
    step->_rowFrom = row1;
    step->_colTo = col;
    step->_rowTo = row;
    step->_moveid = moveId;
    step->_killid = killId;
    //添加进队列当中
    steps.append(step);
}

//内部走一步
void SingleGame::fakeMove(Step* step){
    //杀死该杀死的棋子
    killChess(step->_killid);
    //移动该移动的棋子
    moveChess(step->_moveid,step->_rowTo,step->_colTo);
}
//内部返回
void SingleGame::unfakeMove(Step* step){
    reliveChess(step->_killid);
    moveChess(step->_moveid,step->_rowFrom,step->_colFrom);
}
//计算当前局面得分
int SingleGame::calcScore(){
    int redScore = 0;
    int blackScore = 0;
    /*按照棋子的种类来分类*/
    //enum TYPE{ JIANG, CHE, PAO, MA, BING, SHI, XIANG};
    static int chessScore[] = {1500,100,50,50,20,10,10};
    //黑棋分的总数减去红棋分的总数
    //红棋的分数
    for (int i = 16 ; i < 32 ; i++) {
        if(_list[i]._die) continue;
        redScore += chessScore[_list->_type];
    }
    //黑棋的分数
    for (int i = 0 ; i < 16 ; i++) {
        if(_list[i]._die) continue;
        blackScore += chessScore[_list->_type];
    }
    //得分
    return blackScore-redScore;
}
//让电脑开始走棋
void SingleGame::computerMove(){
    qDebug() << "开始了";
    //该电脑走
    Step* step = getBestMove();
    moveChess(step->_moveid,step->_killid,step->_rowTo,step->_colTo);
    delete step;
    update();
}
