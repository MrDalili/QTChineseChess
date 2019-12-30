#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"

class SingleGame : public Board
{
    Q_OBJECT
public:
    SingleGame()
    {
        _level = 6;
    }
    virtual void click(int id, int row, int col);

    Step* getBestMove();
    void getAllPossibleMove(QVector<Step*>& steps);

    void fakeMove(Step* step);
    void unfakeMove(Step* step);
    int calcScore();

    int getMinScore(int level, int curMaxScore);
    int getMaxScore(int level, int curMinScore);

    int _level;

public slots:
    void computerMove();
};

#endif // SINGLEGAME_H
