#ifndef TETRISBLOCK_H
#define TETRISBLOCK_H

class TetrisBlock {
public:
    static const int shapes[7][4][4]; // 定義 7 種方塊形狀
    static void rotateBlock(int block[4][4]); // 方塊旋轉
};

#endif // TETRISBLOCK_H
