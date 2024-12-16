#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTimer>
#include <QPoint>

class MainWindow;

class GameBoard : public QWidget {
    Q_OBJECT

public:
    GameBoard(QWidget *parent = nullptr);
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();
    void updateGame();
    int getLevel() const { return level; }
    void setLevel(int newLevel) { level = newLevel; }
    bool getIsPaused() const { return isPaused; }
    int getScore() const { return score; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    enum { BoardWidth = 10, BoardHeight = 20 };
    void clearBoard();
    void moveLeft();
    void moveRight();
    void rotate();
    void moveToBottom();
    void generateNewPiece();
    void checkLines();
    void gameOver();

    QTimer *timer;
    bool board[BoardWidth][BoardHeight];
    QPoint currentPiece[4];
    int currentX;
    int currentY;
    int score;
    int level;
    int linesCleared;
    bool isPaused;
    bool isGameOver;
    bool isGameStarted; // 添加 isGameStarted 成員變數
    MainWindow *mainWindow; // 添加 MainWindow 指針
};

#endif // GAMEBOARD_H
