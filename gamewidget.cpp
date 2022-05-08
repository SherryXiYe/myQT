#include "gamewidget.h"

// 颜色数组 存储每个数字对应的背景色
QColor digitBkg[11] = {QColor::fromRgb(0xFF, 0xFF, 0xCC), QColor::fromRgb(0xFF, 0xFF, 0x99),
                            QColor::fromRgb(0xFF, 0xCC, 0xCC), QColor::fromRgb(0xFF, 0xCC, 0x99),
                            QColor::fromRgb(0xFF, 0x99, 0x99), QColor::fromRgb(0xFF, 0x99, 0x66),
                            QColor::fromRgb(0xFF, 0x66, 0x66), QColor::fromRgb(0xCC, 0x99, 0x66),
                            QColor::fromRgb(0xCC, 0x33, 0x33), QColor::fromRgb(0xCC, 0x00, 0x33),
                            QColor::fromRgb(0xFF, 0x00, 0x00)};

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

int GameWidget::getBitCount(int n)  // 获取数字对应的数组下标
{
    int c = 0;
    while (n >>= 1)
        c++;
    return c - 1;
}


void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);   // 构造一个QPainter对象 使用它来进行绘图
    painter.setRenderHint(QPainter::Antialiasing); // 设置反锯齿绘图

    QBrush brush(QColor::fromRgb(141, 121, 81)); // 构造一个画刷 颜色为R G B分量分别为141 121 81的颜色
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen); // 设置画笔为空笔 目的是使绘制的图形没有描边
    painter.drawRoundedRect(QRectF(2 * ratioW, 2 * ratioH, width() - 4 * ratioW, height() - 4 * ratioH), rX, rY);

    QFont font;       //构造字体：Consolas 粗体 大小为40像素
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40 * ratioH);

    painter.setFont(font);
    for (int i = 0; i < 4; i++)  //绘制游戏面板
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j])       // 如果方格中有数字，则设置画刷颜色为数码对应的颜色并绘制小方格和数字
            {
                brush.setColor(digitBkg[getBitCount(board[i][j])]);
                painter.setBrush(brush);
                painter.drawRoundedRect(QRectF(7 * ratioW + (w + 5 * ratioW) * j, 7 * ratioH + (h + 5 * ratioH) * i, w, h), rX, rY);
                painter.setPen(QColor::fromRgb(0, 0, 0)); // 设置画笔为黑色画笔，绘制数码
                painter.drawText(QRectF(7 * ratioW + (w + 5 * ratioW) * j, 7 * ratioH + (h + 5 * ratioH) * i, w, h), Qt::AlignCenter,
                                 QString::number(board[i][j]));
                painter.setPen(Qt::NoPen); // 设置画笔回空笔
            }
            else  // 如果方格中没有数字，则绘制小方格
            {
                brush.setColor(QColor::fromRgb(171, 165, 141));   // 设置画刷颜色为 RGB分量为171 165 141的颜色
                painter.setBrush(brush);
                painter.drawRoundedRect(QRectF(7 * ratioW + (w + 5 * ratioW) * j, 7 * ratioH + (h + 5 * ratioH) * i, w, h), rX, rY);
            }
        }
}
