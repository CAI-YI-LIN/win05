#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Tetris Game");
   // window.resize(400, 400);
    window.show();

    return app.exec();
}