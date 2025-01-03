#include "gameboard.h"
#include "mainwindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <cstdlib> // for std::rand
#include <QMessageBox> // for game over message

GameBoard::GameBoard(QWidget *parent) : QWidget(parent), mainWindow(qobject_cast<MainWindow*>(parent)) {
    setFocusPolicy(Qt::StrongFocus);
    clearBoard();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameBoard::updateGame);
}

void GameBoard::clearBoard() {
    for (int i = 0; i < BoardWidth; ++i) {
        for (int j = 0; j < BoardHeight; ++j) {
            board[i][j] = false;
        }
    }
}

void GameBoard::startGame() {
    score = 0;
    level = 1;
    linesCleared = 0;
    isPaused = false;
    isGameOver = false;
    clearBoard();
    generateNewPiece();
    timer->start(1500); // 設置初始速度
    update();
    mainWindow->updateScore(); // 更新分數顯示

}

void GameBoard::pauseGame() {
    isPaused = true;
}

void GameBoard::resumeGame() {
    isPaused = false;
}

void GameBoard::endGame() {
    isPaused = false;
    isGameOver = true;
    timer->stop();
    update();
    mainWindow->updateScore(); // 更新分數顯示
}

void GameBoard::moveLeft() {
    if (isGameOver) return;
    // 檢查是否可以左移
    for (int i = 0; i < 4; ++i) {
        int newX = currentX + currentPiece[i].x() - 1;
        int newY = currentY + currentPiece[i].y();
        if (newX < 0 || board[newX][newY]) {
            return; // 無法左移
        }
    }

    // 左移方塊
    currentX--;
    update();
}

void GameBoard::moveRight() {
    if (isGameOver) return;
    // 檢查是否可以右移
    for (int i = 0; i < 4; ++i) {
        int newX = currentX + currentPiece[i].x() + 1;
        int newY = currentY + currentPiece[i].y();
        if (newX >= BoardWidth || board[newX][newY]) {
            return; // 無法右移
        }
    }

    // 右移方塊
    currentX++;
    update();
}

void GameBoard::rotate() {
    if (isGameOver) return;
    // 方塊翻轉
    QPoint newPiece[4];
    for (int i = 0; i < 4; ++i) {
        int newX = -currentPiece[i].y();
        int newY = currentPiece[i].x();
        newPiece[i] = QPoint(newX, newY);
    }

    // 檢查新位置是否有效
    for (int i = 0; i < 4; ++i) {
        int newX = currentX + newPiece[i].x();
        int newY = currentY + newPiece[i].y();
        if (newX < 0 || newX >= BoardWidth || newY >= BoardHeight || board[newX][newY]) {
            return; // 無法翻轉
        }
    }

    // 更新方塊位置
    for (int i = 0; i < 4; ++i) {
        currentPiece[i] = newPiece[i];
    }
    update();
}

void GameBoard::moveToBottom() {
    if (isGameOver) return;
    // 方塊快速下降到底部
    while (true) {
        for (int i = 0; i < 4; ++i) {
            int newX = currentX + currentPiece[i].x();
            int newY = currentY + currentPiece[i].y() + 1;
            if (newY >= BoardHeight || board[newX][newY]) {
                // 將當前方塊固定在遊戲板上
                for (int j = 0; j < 4; ++j) {
                    board[currentX + currentPiece[j].x()][currentY + currentPiece[j].y()] = true;
                }
                generateNewPiece();
                return;
            }
        }
        // 檢查並消除已填滿的行
        checkLines();
        currentY++;
        update();
    }
}


void GameBoard::dropStep() {

        if (isGameOver) return;

        int steps = 3; // 每次按鍵下降的行數
        for (int s = 0; s < steps; ++s) {
            bool canMove = true;

            // 檢查每個方塊是否可以向下移動
            for (int i = 0; i < 4; ++i) {
                int newX = currentX + currentPiece[i].x();
                int newY = currentY + currentPiece[i].y() + 1;

                if (newY >= BoardHeight || board[newX][newY]) {
                    canMove = false;
                    break;
                }
            }

            // 如果不能再下降，固定方塊，生成新方塊並返回
            if (!canMove) {
                for (int i = 0; i < 4; ++i) {
                    board[currentX + currentPiece[i].x()][currentY + currentPiece[i].y()] = true;
                }
                generateNewPiece(); // 生成新方塊
                return;
            }

            // 如果可以，則向下移動一格
            currentY++;
        }
        // 檢查並消除已填滿的行
        checkLines();
        update(); // 更新畫面

    }

int GameBoard::getCurrentLevel() const {
    return level; // 假設 currentLevel 是 GameBoard 的成員
}


void GameBoard::updateGame() {
    if (isPaused || isGameOver) return;

    // 更新方塊位置，檢查是否到底或碰到其他方塊
    for (int i = 0; i < 4; ++i) {
        int newX = currentX + currentPiece[i].x();
        int newY = currentY + currentPiece[i].y() + 1;
        if (newY >= BoardHeight || board[newX][newY]) {
            // 方塊到底或碰到其他方塊，將當前方塊固定在遊戲板上
            for (int j = 0; j < 4; ++j) {
                board[currentX + currentPiece[j].x()][currentY + currentPiece[j].y()] = true;
            }
            // 檢查並消除已填滿的行
            checkLines();
            // 生成新方塊
            generateNewPiece();
            return;
        }
    }
    // 檢查並消除已填滿的行
    checkLines();
    // 方塊下移
    currentY++;
    update();
}

void GameBoard::checkLines() {
    int linesRemoved = 0;
    for (int y = 0; y < BoardHeight; ++y) {
        bool isFull = true;
        for (int x = 0; x < BoardWidth; ++x) {
            if (!board[x][y]) {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            // 消除該行並將上方的方塊下移
            for (int j = y; j > 0; --j) {
                for (int x = 0; x < BoardWidth; ++x) {
                    board[x][j] = board[x][j - 1];
                }
            }
            for (int x = 0; x < BoardWidth; ++x) {
                board[x][0] = false;
            }
            linesRemoved++;
        }
    }

    // 更新分數和等級
    if (linesRemoved > 0) {
        score += linesRemoved * 100;
        linesCleared += linesRemoved;
        int newLevel = linesCleared / 2 + 1; // 每消除 10 行升一級
        if (newLevel > level) {
            level = newLevel;
            int newInterval = 1500 - (level * 100); // 每提升一個等級減少 100 毫秒
            timer->setInterval(qMax(newInterval, 100)); // 確保間隔時間不低於 100 毫秒

            // 發送等級改變信號
            emit levelIncreased(level);
        }
        mainWindow->updateScore(); // 更新分數顯示
    }
}

void GameBoard::generateNewPiece() {
    static const QPoint pieces[7][4] = {
        { QPoint(0, 0), QPoint(1, 0), QPoint(0, 1), QPoint(1, 1) }, // 方塊 O
        { QPoint(0, 0), QPoint(1, 0), QPoint(2, 0), QPoint(3, 0) }, // 方塊 I
        { QPoint(0, 0), QPoint(1, 0), QPoint(2, 0), QPoint(2, 1) }, // 方塊 L
        { QPoint(0, 0), QPoint(1, 0), QPoint(2, 0), QPoint(0, 1) }, // 方塊 J
        { QPoint(0, 0), QPoint(1, 0), QPoint(1, 1), QPoint(2, 1) }, // 方塊 Z
        { QPoint(1, 0), QPoint(2, 0), QPoint(0, 1), QPoint(1, 1) }, // 方塊 S
        { QPoint(1, 0), QPoint(0, 1), QPoint(1, 1), QPoint(2, 1) }  // 方塊 T
    };

    // 當前方塊變為下一個方塊
    for (int i = 0; i < 4; ++i) {
        currentPiece[i] = nextPiece[i];
    }

    // 隨機生成新的下一個方塊
    int index = std::rand() % 7;
    for (int i = 0; i < 4; ++i) {
        nextPiece[i] = pieces[index][i];
    }

    currentX = BoardWidth / 2 - 1;
    currentY = 0;

    // 檢查新生成的當前方塊是否立即碰撞
    for (int i = 0; i < 4; ++i) {
        if (board[currentX + currentPiece[i].x()][currentY + currentPiece[i].y()]) {
            gameOver();
            return;
        }
    }

    // 更新下一個方塊的預覽圖像
    QPixmap nextBlockPixmap = getNextBlockPreview();
    mainWindow->updateNextBlockPreview(nextBlockPixmap);

    update();

}

QPixmap GameBoard::getNextBlockPreview() {
    QPixmap pixmap(80, 80);
    pixmap.fill(Qt::white); // 背景為黑色

    QPainter painter(&pixmap);
    painter.setBrush(Qt::red); // 假設下一個方塊為紅色
    for (const QPoint &point : nextPiece) {
        painter.drawRect(point.x() * 20, point.y() * 20, 20, 20); // 假設每個方塊的大小為 20x20
    }

    return pixmap;
}



void GameBoard::gameOver() {
    isGameOver = true;
    timer->stop();
    mainWindow->onEndClicked(); // 停止時間計時器
    int elapsedTime = mainWindow->getElapsedTime(); // 獲取經過的時間
    QString timeString = QString("%1:%2")
                             .arg(elapsedTime / 60, 2, 10, QChar('0'))
                             .arg(elapsedTime % 60, 2, 10, QChar('0'));
    QMessageBox::information(this, "遊戲結束",
                             "遊戲結束！\n您的分數是：" + QString::number(score) +
                                 "\n等級：" + QString::number(level) +
                                 "\n時間：" + timeString);
    mainWindow->updateScore(); // 更新分數顯示
}

void GameBoard::paintEvent(QPaintEvent * /* event */) {
    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());

    int gridSize = qMin(width() / BoardWidth, height() / BoardHeight);



    // 繪製遊戲板上的固定方塊
    painter.setBrush(Qt::gray);
    for (int i = 0; i < BoardWidth; ++i) {
        for (int j = 0; j < BoardHeight; ++j) {
            if (board[i][j]) {
                painter.drawRect(i * gridSize, j * gridSize, gridSize, gridSize);
            }
        }
    }

    // 檢查 `currentPiece` 是否已經生成
    if (currentPiece[0] != QPoint(0, 0) || currentPiece[1] != QPoint(0, 0)) {
        painter.setBrush(Qt::blue);
        for (int i = 0; i < 4; ++i) {
            painter.drawRect((currentX + currentPiece[i].x()) * gridSize, (currentY + currentPiece[i].y()) * gridSize, gridSize, gridSize);
        }
    }
}


void GameBoard::keyPressEvent(QKeyEvent *event) {
    if (isPaused || isGameOver) return;
    switch (event->key()) {
    case Qt::Key_Left: moveLeft(); break;
    case Qt::Key_Right: moveRight(); break;
    case Qt::Key_Up: rotate(); break;
    case Qt::Key_Down: dropStep(); break; // 使用下鍵快速下降
    case Qt::Key_Space: moveToBottom(); break;
    }
    update();
}
