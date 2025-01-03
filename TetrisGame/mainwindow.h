#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSoundEffect>
#include <QCheckBox>
#include <QLabel>


class GameBoard;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void updateScore();
    int getElapsedTime() const { return elapsedTime; }
    void onEndClicked();
    void updateNextBlockPreview(const QPixmap &pixmap);

private:
    GameBoard *gameBoard;
    QTimer *gameTimer;
    QLabel *scoreLabel;
    QLabel *levelLabel;
    QLabel *timeLabel;
    QLabel *nextBlockLabel; // 新增這行
    QTimer *timeTimer;
    int elapsedTime;
    QSoundEffect *musicPlayer;
    QCheckBox *musicCheckBox;

    QTimer *swingTimer; // 控制搖擺的定時器
    QTimer *floatTimer; // 控制搖擺的定時器
    int swingOffset;    // 當前的水平偏移
    int swingDirection; // 搖擺方向（1 表示右，-1 表示左）
    int floatOffset;    // 上下浮動的偏移量
    int floatDirection; // 上下浮動的方向（1 表示向下，-1 表示向上）

private slots:
    void onStartClicked();
    void onPauseClicked();
    void updateTime();
    void swingWindow();
    void toggleMusic(bool play);
};

#endif // MAINWINDOW_H
