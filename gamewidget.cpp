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
    emit ScoreInc(score);
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
    ratioW = width() / 800.0, ratioH = height() / 800.0;
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

    QBrush brush(QColor::fromRgb(128,138,135)); // 构造一个画刷 颜色为R G B分量分别为141 121 81的颜色
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

void GameWidget::mousePressEvent(QMouseEvent *e) // 获取起点坐标
{
    startPos = e->pos();
}

void GameWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // 根据终点坐标和起点坐标计算XY坐标的增量
    float dX = (float)(e->pos().x() - startPos.x());
    float dY = (float)(e->pos().y() - startPos.y());
    // 确定手势方向
    GestureDirect dir;
    if (abs(dX) > abs(dY))
    {
        if (dX < 0)
            emit dir=LEFT;
        else
            emit dir=RIGHT;
    }
    else
    {
        if (dY < 0)
            emit dir=UP;
        else
            emit dir=DOWN;
    }
}

void GameWidget::onGestureMove(GestureDirect direct)
{
    int i, j, k;
    Animation a;
    bool move = false, combine = false; // 记录是否移动过方格以及是否有方格合并
    bool isCombined[4][4];     // 记录某个格子是否参与过合并
    memset(isCombined, 0, sizeof(isCombined));
    switch (direct)   // 处理不同方向
    {
    case LEFT:      //向左
        for (i = 0; i < 4; i++)  // 循环每一行
        {
            j = 0, k = 0;   //j表示要交换的数字列号；k表示交换到的位置的列号。初始化j k为0。
            while (true)
            {
                while (j < 4 && board[i][j] == 0)    // 循环找到第一个不是0的数字对应的列号
                    j++;
                if (j > 3)  // 如果超过了3则说明搜索完毕，跳出循环
                    break;
                qSwap(board[i][k], board[i][j]);  //交换j和k对应位置的两个数字
                if (j != k)
                    move = true;
                a.type = MOVE;   // 记录动画信息
                a.startPos = QPointF(7 * ratioW + (w + 5 * ratioW) * j, 7 * ratioH + (h + 5 * ratioH) * i);
                a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * k, 7 * ratioH + (h + 5 * ratioH) * i);
                a.digit = a.digit2 = board[i][k];
                a.direct = LEFT;
                if (k > 0 && board[i][k] == board[i][k - 1] && !isCombined[i][k - 1]) //如果交换后的数字与其前一列的数字相同，且还没有被合并，则与之合并
                {
                    board[i][k - 1] <<= 1;  // 前一列的数字*2
                    isCombined[i][k - 1] = true;
                    board[i][k] = 0;
                    combine = true;

                    a.digit2 = board[i][k - 1]; // 记录动画信息
                    a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * (k - 1), 7 * ratioH + (h + 5 * ratioH) * i);

                    score += board[i][k - 1]; // 增加分数
                    emit ScoreInc(score);
                    digitCount--;  // 数码个数-1
                }
                else
                    k++;
                j++;
                animationList.append(a);  // 添加到动画链表
            }
        }
        break;
        // 其余三个方向与左向类似
    case RIGHT:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3;
            while (true)
            {
                while (j > -1 && board[i][j] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[i][k], board[i][j]);
                if (j != k)
                    move = true;
                a.type = MOVE;
                a.startPos = QPointF(7 * ratioW + (w + 5 * ratioW) * j, 7 * ratioH + (h + 5 * ratioH) * i);
                a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * k, 7 * ratioH + (h + 5 * ratioH) * i);
                a.digit = a.digit2 = board[i][k];
                a.direct = RIGHT;
                if (k < 3 && board[i][k] == board[i][k + 1] && !isCombined[i][k + 1])
                {
                    board[i][k + 1] <<= 1;
                    isCombined[i][k + 1] = true;
                    board[i][k] = 0;
                    combine = true;
                    a.digit2 = board[i][k + 1];
                    a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * (k + 1), 7 * ratioH + (h + 5 * ratioH) * i);
                    score += board[i][k + 1];
                    emit ScoreInc(score);
                    digitCount--;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    case UP:
        for (i = 0; i < 4; i++)
        {
            j = 0, k = 0;
            while (true)
            {
                while (j < 4 && board[j][i] == 0)
                    j++;
                if (j > 3)
                    break;
                qSwap(board[k][i], board[j][i]);
                if (j != k)
                    move = true;
                a.type = MOVE;
                a.startPos = QPointF(7 * ratioW + (w + 5 * ratioW) * i, 7 * ratioH + (h + 5 * ratioH) * j);
                a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * i, 7 * ratioH + (h + 5 * ratioH) * k);
                a.digit = a.digit2 = board[k][i];
                a.direct = UP;
                if (k > 0 && board[k][i] == board[k - 1][i] && !isCombined[k - 1][i])
                {
                    board[k - 1][i] <<= 1;
                    isCombined[k - 1][i] = true;
                    board[k][i] = 0;
                    combine = true;
                    a.digit2 = board[k - 1][i];
                    a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * i, 7 * ratioH + (h + 5 * ratioH) * (k - 1));
                    score += board[k - 1][i];
                    emit ScoreInc(score);
                    digitCount--;
                }
                else
                    k++;
                j++;
                animationList.append(a);
            }
        }
        break;
    case DOWN:
        for (i = 0; i < 4; i++)
        {
            j = 3, k = 3;
            while (true)
            {
                while (j > -1 && board[j][i] == 0)
                    j--;
                if (j < 0)
                    break;
                qSwap(board[k][i], board[j][i]);
                if (j != k)
                    move = true;
                a.type = MOVE;
                a.startPos = QPointF(7 * ratioW + (w + 5 * ratioW) * i, 7 * ratioH + (h + 5 * ratioH) * j);
                a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * i, 7 * ratioH + (h + 5 * ratioH) * k);
                a.digit = a.digit2 = board[k][i];
                a.direct = DOWN;
                if (k < 3 && board[k][i] == board[k + 1][i] && !isCombined[k + 1][i])
                {
                    board[k + 1][i] <<= 1;
                    isCombined[k + 1][i] = true;
                    board[k][i] = 0;
                    combine = true;
                    a.digit2 = board[k + 1][i];
                    a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * i, 7 * ratioH + (h + 5 * ratioH) * (k + 1));
                    score += board[k + 1][i];
                    emit ScoreInc(score);
                    digitCount--;
                }
                else
                    k--;
                j--;
                animationList.append(a);
            }
        }
        break;
    }
    if ((move || combine) && digitCount != 16) // 移动和合并完成后，如果数字没有填满，则随机生成一个行号和列号，如果该位置为0，则在该位置填上2，否则继续随机生成一个位置直到该位置为0.
    {
        i = rand() % 4, j = rand() % 4;
        while (board[i][j] != 0)
            i = rand() % 4, j = rand() % 4;
        board[i][j] = (rand() % 2 + 1) * 2;
        a.type = APPEARANCE;     // 记录动画信息，并加入动画链表
        a.startPos = a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * j, 7 * ratioH + (h + 5 * ratioH) * i);
        a.startPos += QPointF(w / 2, h / 2);
        a.digit = board[i][j];
        animationList.append(a);
        digitCount++;  // 数码个数加一
    }

    isAnimating = true; // 开始绘制动画效果
    timer.start(10);   // 启动计时器
}
