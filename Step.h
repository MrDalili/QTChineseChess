#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();
    //移动的棋子
    int _moveid;
    //杀掉的棋子
    int _killid;
    //上一步
    int _rowFrom;
    int _colFrom;
    //下一步
    int _rowTo;
    int _colTo;

signals:

public slots:
};

#endif // STEP_H
