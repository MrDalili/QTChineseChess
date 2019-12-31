#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Board::Board(QWidget *parent) : QWidget(parent)
{
    qDebug()<<"初始化";
    _d = 40;
    _r = 20;
    _selectChess = -1;
    //初始化象棋
    for (int i = 0 ; i < 32; i++) {
        _list[i].init(i);
    }
    _redTime = true;
}


void Board::paintEvent(QPaintEvent *){
    QPainter painter(this);

    // 画10横线
    for (int i = 1; i <= 10 ; ++i) {
        painter.drawLine(QPoint(_d , i*_d) , QPoint(9*_d , i*_d));
    }
    //画竖线
    for (int i = 1; i <= 9 ; ++i) {
        if(i == 1 || i == 9)
            painter.drawLine(QPoint(i*_d , _d) , QPoint(i*_d , 10*_d));
        else{
            painter.drawLine(QPoint(i*_d , _d) , QPoint(i*_d , 5*_d));
            painter.drawLine(QPoint(i*_d , 6*_d) , QPoint(i*_d , 10*_d));
        }
    }
    //画9宫格
    painter.drawLine(QPoint(4*_d , 1*_d) , QPoint(6*_d , 3*_d));
    painter.drawLine(QPoint(6*_d , 1*_d) , QPoint(4*_d , 3*_d));
    painter.drawLine(QPoint(4*_d , 8*_d) , QPoint(6*_d , 10*_d));
    painter.drawLine(QPoint(4*_d , 10*_d) , QPoint(6*_d , 8*_d));

    //绘制32个棋子
    for (int i = 0 ; i < 32 ; ++i) {
        drawChess(painter,i);
    }
};

void Board::drawChess(QPainter &painter, int id){
    //如果棋子死亡，那么就直接退出
    if(_list[id]._die)
        return;
    //根据棋盘的行列转换为棋盘的横纵坐标
    QPoint point = center(id);
    //设置背景颜色，使用画刷
    if(id == _selectChess)
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));
    //设置黑色画笔
    painter.setPen(Qt::black);
    //根据返回来的point的坐标画棋子
    painter.drawEllipse(point, _r,_r);
    //画字

    //如果棋子是红色棋子就换成红色的画笔
    if(_list[id]._red)
        painter.setPen(Qt::red);
    //设置字体的大小颜色
        painter.setFont(QFont("黑体",_r - 5,1800));
    //根据中心点坐标推算出写字的左上角坐标
    QRect rect = QRect(point.x() - _r,point.y() - _r, _d ,_d);
    painter.drawText(rect ,_list[id].getText(), QTextOption(Qt::AlignCenter));
}

//通过棋子在棋盘中的横纵计算对应的像素
QPoint Board::center(int row ,int col){
    QPoint point;
    //计算棋子对应的坐标
    point.setX((col + 1) * _d);
    point.setY((row + 1) * _d);
    return point;
}
//重载一下让计算好看一些
QPoint Board::center(int id){
    return center(_list[id]._row,_list[id]._col);
}

void Board::mouseReleaseEvent(QMouseEvent *ev){

    qDebug() << _selectChess;
    //获取点击的位置
    QPoint pt = ev->pos();
    //判断是不是有象棋被点住
    //先将point转化为象棋的行列值，再判断有没有棋子在上面
    int row,col;
    bool bRet = getRowCol(pt,row,col);
    qDebug() << row ;
    qDebug() << col ;
    if(bRet == false)//点在棋盘外面
        return;
    //每一次点击记录一下当前点击的棋子id
    int clickedId = -1;
    clickedId = selectChecked(row,col);

    click(clickedId,row,col);
//    if(_selectChess == -1){
//        //小于32表明，未遍历完就有一个棋子匹配上了，将编号记住
//        if(clickedId < 32 && clickedId != -1){
//            //判断一下该不该当前颜色的棋子被选定
//            if(_redTime == _list[clickedId]._red){
//                _selectChess = clickedId;
//                update();
//            }
//        }
//    }else {
//        //先判断一下能不能走
//        if(canMove(_selectChess,row,col,clickedId)){
//            //将点击的棋子移动过去
//            _list[_selectChess]._row = row;
//            _list[_selectChess]._col = col;
//            //如果有棋子被覆盖
//            if(clickedId != -1){
//                //覆盖的棋子死亡
//                _list[clickedId]._die = true;
//            }
//            //取消颜色的选中
//            _selectChess = -1;
//            //轮换该走的棋子
//            _redTime = !_redTime;
//            update();
//        }
//    }
}

//通过点的横纵坐标计算在那一行那一列
bool Board::getRowCol(QPoint pt, int &row, int &col){
    //行除以40，列除以40
    col = pt.x() / _d;
    row = pt.y() / _d;
    //如果大于一半了的半径了，那就属于下一个焦点的棋子了，需要+1
    if(pt.x() % _d >= _r)
        col++;
    if(pt.y() % _d >= _r)\
        row++;
    row--;
    col--;
    if(col < 0 || col > 8 || row < 0 || row >9){
        //如果大于那就说明没点在棋盘里，pass了
        return false;
    }
    return true;
}

//选中棋子
void Board::click(int clickedId , int row ,int col){

    if(_selectChess == -1){
        //小于32表明，未遍历完就有一个棋子匹配上了，将编号记住
        if(clickedId < 32 && clickedId != -1){
            //判断一下该不该当前颜色的棋子被选定
            if(_redTime == _list[clickedId]._red){
                _selectChess = clickedId;
                update();
            }
        }
    }else {
        //先判断一下能不能走
        if(canMove(_selectChess,row,col,clickedId)){
            //将点击的棋子移动过去
            _list[_selectChess]._row = row;
            _list[_selectChess]._col = col;
            //如果有棋子被覆盖
            if(clickedId != -1){
                //覆盖的棋子死亡
                _list[clickedId]._die = true;
            }
            //取消颜色的选中
            _selectChess = -1;
            //轮换该走的棋子
            _redTime = !_redTime;
            update();
        }
    }
}

//返回被选中的棋子的编号
int Board::selectChecked(int row, int col){
    int i = 0;
    for (; i < 32; ++i) {
        //遍历每一个棋子,并确定行与列对应且没死亡
        if(_list[i]._row == row && _list[i]._col == col && _list[i]._die == false){
            break;
        }
    }
    //小于32表明，未遍历完就有一个棋子匹配上了，将编号返回
    if(i < 32){
        return i;
    }
    return -1;
}

//移动棋子
bool Board::canMove(int moveId,int row,int col,int killId){
    //如果移动的棋子和被吃的棋子颜色相同不可以move
    if(_list[moveId]._red == _list[killId]._red && killId != -1){
        //换选择
       _selectChess = killId;
       update();
       return false;
    }

    //复杂的情况,对每一个棋子进行判断
    switch (_list[moveId]._type) {
        case Chess::CHE:
            return canMoveChe(moveId,row,col,killId);
        case Chess::MA:
            return canMoveMa(moveId,row,col,killId);
        case Chess::XIANG:
            return canMoveXiang(moveId,row,col,killId);
        case Chess::SHI:
            return canMoveShi(moveId,row,col,killId);
        case Chess::JIANG:
            return canMoveJiang(moveId,row,col,killId);
        case Chess::PAO:
            return canMovePao(moveId,row,col,killId);
        case Chess::BING:
            return canMoveBing(moveId,row,col,killId);
    }

    return true;
}
//移动车
bool Board::canMoveChe(int moveId,int row,int col,int killId){
    //看看是不是之间没有棋子
    if(getChessNumWithTwoChess(moveId,row,col) == 0)
        return true;
    return false;
}
//移动马
bool Board::canMoveMa(int moveId,int row,int col,int killId){
    //获取当前棋子的横纵坐标
    int row1,col1;
    getRowCol(moveId,row1,col1);
    //判断特征值，马的特征值是12或21
    int r = relationNum(row ,row1 ,col ,col1);
    if(r != 12 && r != 21)
        return false;
    //看看有没有棋子蹩马腿
    //12代表列相差2
    if(r == 12){
        if(selectChecked(row1, (col+col1)/2) != -1)
            return false;
    }else {
        //代表行相差2
        if(selectChecked((row+row1)/2, col1 ) != -1)
            return false;
    }
    return true;
}
//移动象
bool Board::canMoveXiang(int moveId,int row,int col,int killId){
    int row1,col1;
    //获取象的原先地址
    getRowCol(moveId,row1,col1);
    //计算移动前后的值
    int d = relationNum(row,row1,col,col1);
    //象的特征值是22
    if(d != 22) return false;
    //计算象眼的位置
    int rEye = (row+row1)/2;
    int cEye = (col+col1)/2;
    //看看象眼上有没有棋子
    if(selectChecked(rEye,cEye) != -1){
        return false;
    }

    //现在再去限制象不可以过河
    if(_list[moveId]._red){
        if(row < 5) return false;
    }else {
        if(row > 5) return false;
    }
    return true;
}
//移动士
bool Board::canMoveShi(int moveId,int row,int col,int killId){
    //与将一样
    if(!_list[moveId]._red){
        if(row > 2) return false;
    }else {
        if(row < 7) return false;
    }
    //列相等，看是否在一个范围内
    if(col < 3) return false;
    if(col > 5) return false;

    //判断移动步长
    //行间距
    int dr = _list[moveId]._row - row;
    //列间距
    int dc = _list[moveId]._col - col;
    //移动以后横纵都进行了1个单位的位移，所以相加为11
    int d = abs(dr)*10 + abs(dc);
    if(d == 11)
        return true;
    //满足移动条件
    return false;
}
//移动将
bool Board::canMoveJiang(int moveId,int row,int col,int killId){
    /*
        1.目标在九宫内
        2.移动的步长是一个格子
    */
    //对不同颜色的行进行判断
    if(!_list[moveId]._red){
        if(row > 2) return false;
    }else {
        if(row < 7) return false;
    }
    //列相等，看是否在一个范围内
    if(col < 3) return false;
    if(col > 5) return false;

    //判断移动步长
    //行间距
    int dr = _list[moveId]._row - row;
    //列间距
    int dc = _list[moveId]._col - col;
    //牛逼的方法，把一个×10相加，如果满足的话，就有一个1就好了
    int d = abs(dr)*10 + abs(dc);
    if(d == 10 || d == 1)
        return true;
    //如果老将碰老将可以直接跳过去

    //满足移动条件
    return false;
}
//移动炮
bool Board::canMovePao(int moveId,int row,int col,int killId){
    //计算炮与要移动的地方距离几个棋子
    int num = getChessNumWithTwoChess(moveId,row,col);
    //如果要吃子且中间有一个棋子那么符合规则
    if(killId != -1){
        if(num == 1) return true;
    }else {
        //如果不再吃棋子的状态而且中间没有任何棋子
        if(num == 0) return true;
    }
    return false;
}
//移动兵
bool Board::canMoveBing(int moveId,int row,int col,int killId){
    //计算位置
    int row1,col1;
    getRowCol(moveId,row1,col1);
    //计算特征值，兵的特征值为1与10
    int r = relationNum(row,row1,col,col1);
    if(r != 1 && r != 10) return false;

    //如果是红棋
    if(_list[moveId]._red){
        //目标行需要小于原来的行
        if(row > row1) return false;
        //如果没有过河还想平着走，拒绝
        if(row1 >= 5 && row == row1) return false;
    }else {
        //黑棋，同理
        if(row < row1) return false;
        if(row1 <= 4 && row == row1) return false;
    }
    return true;
}

//通过棋子的id获取该棋子的横纵坐标
void Board::getRowCol(int chessId, int& row , int& col){
    row = _list[chessId]._row;
    col = _list[chessId]._col;
}

//计算棋子的特征值
int Board::relationNum(int row , int row1 , int col ,int col1){
    return abs(row-row1)*10 + abs(col-col1);
}

//计算2个棋子之间的棋子数量
int Board::getChessNumWithTwoChess(int moveId,int row ,int col){
    int row1,col1;
    getRowCol(moveId,row1,col1);
    return getChessNumWithTwoChess(row,row1,col,col1);
}

int Board::getChessNumWithTwoChess(int row,int row1,int col,int col1){
    int ret = 0;
    //不在同一条直线上
    if(row1 != row && col != col1) return -1;
    //同一个棋子
    if(row == row1 && col == col1) return -1;
    //如果在一条横线上
    if(row == row1){
        //找最左边的棋子，开始遍历
        int min = col1 < col ? col1 : col;
        int max = col1 > col ? col1 : col;
        //开始遍历
        for (int c = min + 1; c < max; ++c) {
            //如果有棋子，那么ret++
            if(selectChecked(row,c) != -1) ++ret;
        }
    }else {
        //如果是在一条纵线上
        int min = row < row1 ? row : row1;
        int max = row > row1 ? row : row1;
        //遍历
        for (int r = min + 1; r < max ; ++r) {
            if(selectChecked(r,col) != -1) ++ret;
        }
    }
    return ret;
}
//杀死棋子
void Board::killChess(int killId){
    if(killId == -1) return;
    _list[killId]._die = true;
}
//复活棋子
void Board::reliveChess(int reliveId){
    if(reliveId == -1) return;
    _list[reliveId]._die = false;
}

//移动棋子
void Board::moveChess(int moveId, int row, int col)
{
    _list[moveId]._row = row;
    _list[moveId]._col = col;

    _redTime = !_redTime;
}
void Board::moveChess(int moveid, int killid, int row, int col){
    killChess(killid);
    moveChess(moveid, row, col);

}
Board::~Board(){

}
