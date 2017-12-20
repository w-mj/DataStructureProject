#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QCoreApplication>
#include <QDebug>

//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '&lt;  `.___\_&lt;|&gt;_/___.' &gt;' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         永无BUG
//
//
//

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QCoreApplication::applicationDirPath();
    qDebug() << __FILE__;
    MainWindow w;
    w.show();

    return a.exec();
}
