#include "mainwindow.h"
#include "gameboard.h"
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSoundEffect> // 用於播放音樂
#include <QCheckBox> // 用於音樂播放控制



const int gameBoardWidth = 200; // 遊戲區的寬度
const int gameBoardHeight = 400; // 遊戲區的高度

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), elapsedTime(0), musicPlayer(new QSoundEffect(this)) ,
    swingOffset(0),swingDirection(1), floatOffset(0), floatDirection(1) {

    gameBoard = new GameBoard(this);
    gameBoard->setFixedSize(gameBoardWidth, gameBoardHeight); // 設置遊戲區的固定大小

    // 設置背景顏色為白色
    gameBoard->setStyleSheet("background-color: white;");



    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, gameBoard, &GameBoard::updateGame);

    scoreLabel = new QLabel("分數: 0", this);
    levelLabel = new QLabel("等級: 1", this);
    timeLabel = new QLabel("時間: 00:00", this); // 初始化時間顯示的 QLabel
    QPushButton *startButton = new QPushButton("開始", this);
    QPushButton *pauseButton = new QPushButton("暫停", this);
    musicCheckBox = new QCheckBox("播放音樂", this); // 添加播放音樂的 QCheckBox


    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(musicCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleMusic);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(scoreLabel);
    rightLayout->addWidget(levelLabel);
    rightLayout->addWidget(timeLabel); // 添加時間顯示的 QLabel 到佈局中
    rightLayout->addWidget(startButton);
    rightLayout->addWidget(pauseButton);
    rightLayout->addWidget(musicCheckBox); // 添加播放音樂的 QCheckBox 到佈局中

    rightLayout->setContentsMargins(10, 10, 10, 10); // 設置右側佈局的邊距

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(gameBoard);

    mainLayout->addLayout(rightLayout);
    mainLayout->setContentsMargins(10, 10, 10, 10); // 設置主佈局的邊距

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setFixedSize(sizeHint()); // 設置視窗的固定大小

    timeTimer = new QTimer(this); // 初始化計時器
    connect(timeTimer, &QTimer::timeout, this, &MainWindow::updateTime);

    musicPlayer = new QSoundEffect(this); // 初始化 QSoundEffect
    musicPlayer->setSource(QUrl("qrc:/resources/bgm.wav"));
    musicPlayer->setLoopCount(QSoundEffect::Infinite); // 設置無限循環播放
    musicPlayer->setVolume(0.5f); // 設置音量

    nextBlockLabel = new QLabel(this);
    rightLayout->addWidget(nextBlockLabel); // 添加 nextBlockLabel 到佈局

    // 初始化 nextBlockLabel
    nextBlockLabel->setFixedSize(80, 80); // 設置固定大小
    nextBlockLabel->setStyleSheet("background-color: white;"); // 設置背景顏色
    rightLayout->insertWidget(0, nextBlockLabel); // 插入到最上方

    swingTimer = new QTimer(this);
    floatDirection = 0; // 初始狀態未浮動
    connect(swingTimer, &QTimer::timeout, this, &MainWindow::swingWindow);
    connect(gameBoard, &GameBoard::levelIncreased, this, [this](int level) {
        if (level >= 2 && !swingTimer->isActive()) {
            swingTimer->start(50); // 等級 2 開始搖擺
        }
        if (level >= 3) {
            floatDirection = 1; // 等級 3 開始上下浮動
        }
    });

    // 搖擺計時器
    swingTimer = new QTimer(this);
    connect(swingTimer, &QTimer::timeout, this, &MainWindow::swingWindow);

    // 浮動計時器
    floatTimer = new QTimer(this);
    connect(floatTimer, &QTimer::timeout, this, &MainWindow::swingWindow);

    //swingTimer = new QTimer(this);
    //connect(swingTimer, &QTimer::timeout, this, &MainWindow::swingWindow);

}

void MainWindow::onStartClicked() {
    gameBoard->startGame();
    gameTimer->start(500 / gameBoard->getLevel());
    elapsedTime = 0; // 重置經過的時間
    timeTimer->start(1000); // 每秒更新一次時間
    musicCheckBox->setChecked(true);
    musicPlayer->play(); // 播放音樂

}

void MainWindow::onPauseClicked() {
    if (gameBoard->getIsPaused()) {
        // 恢復遊戲
        gameBoard->resumeGame();
        timeTimer->start(1000); // 恢復時間計時器
        int currentLevel = gameBoard->getCurrentLevel();

        // 根據級數恢復搖擺和浮動
        if (currentLevel >= 2) {
            swingTimer->start(50); // 恢復搖擺的計時器
        }
        if (currentLevel >= 3) {
            floatTimer->start(50); // 恢復浮動的計時器
        }

    } else {
        // 暫停遊戲
        gameBoard->pauseGame();
        timeTimer->stop(); // 暫停時間計時器
        swingTimer->stop(); // 停止搖擺
        floatTimer->stop(); // 停止浮動

        // 可選：是否重置偏移量
        swingOffset = 0;
        floatOffset = 0;
    }
}


void MainWindow::updateScore() {

    scoreLabel->setText("分數: " + QString::number(gameBoard->getScore()));
    levelLabel->setText("等級: " + QString::number(gameBoard->getLevel()));
}

void MainWindow::onEndClicked() {
    timeTimer->stop(); // 停止時間計時器
    swingTimer->stop(); // 停止搖擺
    swingOffset = 0;
    floatOffset = 0;

}

void MainWindow::updateTime() {
    elapsedTime++;
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;
    QString timeString = QString("%1:%2")
                             .arg(minutes, 2, 10, QChar('0'))
                             .arg(seconds, 2, 10, QChar('0'));
    timeLabel->setText("時間: " + timeString);
}

void MainWindow::updateNextBlockPreview(const QPixmap &pixmap) {
    nextBlockLabel->setPixmap(pixmap);
}

void MainWindow::toggleMusic(bool play) {
    if (play) {
        musicPlayer->play(); // 播放音樂
    } else {
        musicPlayer->stop(); // 停止音樂
    }
}

void MainWindow::swingWindow() {
    int swingRange = 5;  // 左右搖擺的範圍
    int floatRange = 10;   // 上下浮動的範圍

    // 更新左右搖擺的偏移
    swingOffset += swingDirection;
    if (swingOffset > swingRange || swingOffset < -swingRange) {
        swingDirection = -swingDirection;
    }

    // 更新上下浮動的偏移
    floatOffset += floatDirection;
    if (floatOffset > floatRange || floatOffset < -floatRange) {
        floatDirection = -floatDirection;
    }

    // 設置新位置
    this->move(this->pos().x() + swingDirection, this->pos().y() + floatDirection);
}


