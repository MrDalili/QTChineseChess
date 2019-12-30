#include <QApplication>
#include "SingleGame.h"
#include "NetGame.h"
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMessageBox::StandardButton ret;
    ret = QMessageBox::question(NULL, "server or client",
                          "作为服务器启动");
    bool bServer = false;
    if(ret == QMessageBox::Yes)
    {
        bServer = true;
    }


    NetGame board(bServer);
    board.show();
    return app.exec();
}
