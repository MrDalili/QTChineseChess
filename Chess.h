#ifndef CHESS_H
#define CHESS_H
#include <QString>

class Chess
{
public:
    Chess();
    ~Chess();

    enum TYPE{ JIANG, CHE, PAO, MA, BING, SHI, XIANG};

    int _row;//行
    int _col;//列
    int _id;//编号
    bool _die;//是否死亡
    bool _red;//是蓝色方还是红色方
    TYPE _type;//类型

    void init(int id){

        //一个棋盘的图用来表明黑色方的每一课棋子在棋盘中对应的位置
        struct{
            int row,col;
            Chess::TYPE type;
        }pos[16] = {
            {0,0,Chess::CHE},
            {0,1,Chess::MA},
            {0,2,Chess::XIANG},
            {0,3,Chess::SHI},
            {0,4,Chess::JIANG},
            {0,5,Chess::SHI},
            {0,6,Chess::XIANG},
            {0,7,Chess::MA},
            {0,8,Chess::CHE},
            {2,1,Chess::PAO},
            {2,7,Chess::PAO},
            {3,0,Chess::BING},
            {3,2,Chess::BING},
            {3,4,Chess::BING},
            {3,6,Chess::BING},
            {3,8,Chess::BING},
        };

        _id = id;
        _die = false;
        _red = 15<id;
        //如果小于16为黑色方的棋子
        if(_id <= 15){
            _row = pos[id].row;
            _col = pos[id].col;
            _type = pos[id].type;
        }else {
            _row = 9 - pos[id-16].row;
            _col = 8 - pos[id-16].col;
            _type = pos[id-16].type;
        }
    }

    QString getText(){
        switch(this->_type){
            case CHE:
                return "车";
            case JIANG:
                return "将";
            case PAO:
                return "炮";
            case MA:
                return "马";
            case BING:
                return "兵";
            case SHI:
                return "士";
            case XIANG:
                return "象";
        }
    }
};

#endif // CHESS_H
