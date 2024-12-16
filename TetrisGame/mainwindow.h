#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSoundEffect>
#include <QCheckBox>

class GameBoard;
class QLabel;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void updateScore(); // 將 updateScore 設置為公有
    int getElapsedTime() const { return elapsedTime; } // 添加獲取經過時間的函數
    void onEndClicked(); // 將 onEndClicked 設置為公有

private slots:
    void onStartClicked();
    void onPauseClicked();
    void updateTime();
    void toggleMusic(bool play);

private:
    GameBoard *gameBoard;
    QTimer *gameTimer;
    QLabel *scoreLabel;
    QLabel *levelLabel;
    QLabel *timeLabel; // 添加時間顯示的 QLabel
    QTimer *timeTimer; // 添加計時器來更新時間
    int elapsedTime; // 保存經過的時間（秒）
    QSoundEffect *musicPlayer; // 添加 QSoundEffect 成員變數
    QCheckBox *musicCheckBox; // 添加 QCheckBox 成員變數
};

#endif // MAINWINDOW_H
