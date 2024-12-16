#include "mainwindow.h"
#include "gameboard.h"
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSoundEffect> // 用於播放音樂
#include <QCheckBox> // 用於音樂播放控制

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), elapsedTime(0) {
    gameBoard = new GameBoard(this);
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
    rightLayout->addStretch(); // 添加彈性空間

    QWidget *rightWidget = new QWidget(this);
    rightWidget->setLayout(rightLayout);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(gameBoard);
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 1); // 設置遊戲區域佔據主要部分
    splitter->setStretchFactor(1, 1); // 設置功能介面佔據次要部分

    setCentralWidget(splitter);

    timeTimer = new QTimer(this); // 初始化計時器
    connect(timeTimer, &QTimer::timeout, this, &MainWindow::updateTime);

    musicPlayer = new QSoundEffect(this); // 初始化 QSoundEffect
    //musicPlayer->setSource(QUrl::fromLocalFile("C:/Users/LCY/Desktop/QtHW/TetrisGame/TetrisGamemusic.wav")); // 設置音樂文件
    musicPlayer->setLoopCount(QSoundEffect::Infinite); // 設置無限循環播放
    musicPlayer->setVolume(0.5f); // 設置音量
}

void MainWindow::onStartClicked() {
    gameBoard->startGame();
    gameTimer->start(500 / gameBoard->getLevel());
    elapsedTime = 0; // 重置經過的時間
    timeTimer->start(1000); // 每秒更新一次時間
}

void MainWindow::onPauseClicked() {
    if (gameBoard->getIsPaused()) {
        gameBoard->resumeGame();
        timeTimer->start(1000); // 恢復時間計時器
    } else {
        gameBoard->pauseGame();
        timeTimer->stop(); // 暫停時間計時器
    }
}

void MainWindow::updateScore() {
    scoreLabel->setText("分數: " + QString::number(gameBoard->getScore()));
    levelLabel->setText("等級: " + QString::number(gameBoard->getLevel()));
}

void MainWindow::onEndClicked() {
    timeTimer->stop(); // 停止時間計時器
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

void MainWindow::toggleMusic(bool play) {
    if (play) {
        musicPlayer->play(); // 播放音樂
    } else {
        musicPlayer->stop(); // 停止音樂
    }
}
