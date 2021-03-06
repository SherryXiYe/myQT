#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include<QSound>
#include "gamewidget.h"

#define LBLSTYLESHEET "QLabel {color: MidnightBlue;background: #F0FFFF;border: %1px solid SteelBlue;border-radius: %2px;}"
#define BTNSTYLESHEET "QPushButton {color: DimGrey;background: #FFF8DC;border: %1px solid darkgray;border-radius: %2px;} QPushButton:pressed{color: white;background: orange;border: %1px solid darkgray;border-radius: %2px;}"

// 主窗口部件类
class Widget : public QWidget
{
    Q_OBJECT

public:
    // 构造&析构函数
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QPushButton *restartBtn;    // “restart”按钮
    QLabel *scoreLbl;       // “score”标签
    QLabel *highScoreLbl;     // “HightScore”标签
    QSound* backgroundSound;    //背景音乐
    GameWidget *gameWidget; // 游戏部件
    qreal b_w,b_h;   //大方框的宽和高
    qreal ratioW, ratioH;   // 宽度和高度的缩放比例 用来使窗口部件随主窗口的尺寸改变而改变位置与尺寸
    int highScore; // 记录最高分
protected:
    void resizeEvent(QResizeEvent *);       // 窗口尺寸改变触发的事件

public slots:
    void onScoreInc(int);      // 处理分数增加信号的槽函数
    void onGameOver();        // 处理游戏结束信号的槽函数
    void onWin();        // 处理游戏获胜信号的槽函数
};

#endif // WIDGET_H
