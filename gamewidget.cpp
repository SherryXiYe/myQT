#include "gamewidget.h"



// 每个方向位置的增量
QPointF dPos[5];

GameWidget::GameWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    timer(this)
{
    memset(board, 0, sizeof(board));  // 初始化board数组
    score = 0;   // 分数初始化为0
    digitCount = 2; // 数码个数初始化为2
    init2Block(); // 初始化两个方格
}

void GameWidget::init2Block()
{
    board[rand() % 4][rand() % 4] = 2;
    int i = rand() % 4, j = rand() % 4;
    while (board[i][j] != 0)
        i = rand() % 4, j = rand() % 4;
    board[i][j] = 2;
    update();
}

void GameWidget::restart()    // 初始化相关变量 同构造函数
{
    score = 0;
    digitCount = 2;
    memset(board, 0, sizeof(board));
    init2Block();
    //emit ScoreInc(score);
    update();
}

bool GameWidget::checkGameOver() // 循环检测是否含有相邻的相同数码,若有则没有结束。
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (j != 3 && board[i][j] == board[i][j + 1])
                return false;
            if (i != 3 && board[i][j] == board[i + 1][j])
                return false;
        }
    return true;
}

bool GameWidget::checkWin()  // 循环检测是否某个方格的数字为2048，若有则胜利，返回true
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (board[i][j] == 2048)
                return true;
    return false;
}

void GameWidget::resizeEvent(QResizeEvent *)
{
    ratioW = width() / 400.0, ratioH = height() / 400.0;
    // 计算每个小格子的宽度和高度
    w = width() - 4 * ratioW, h = height() - 4 * ratioH;
    w = (w - 25 * ratioW) / 4, h = (h - 25 * ratioH) / 4;
    rX = 15 * ratioW, rY = 15 * ratioH;
    dPos[0] = QPointF(-25 * ratioW, 0);
    dPos[1] = QPointF(25 * ratioW, 0);
    dPos[2] = QPointF(0, -25 * ratioH);
    dPos[3] = QPointF(0, 25 * ratioH);
    dPos[4] = QPointF(-4 * ratioW, -4 * ratioH);
}
