#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QOpenGLWidget>
//#include <QMouseEvent>
//#include <QEventLoop>
#include <QTimer>
//#include <QPainter>
//#include <QList>

// 手势的方向
enum GestureDirect
{
    LEFT = 0,   // 向左
    RIGHT = 1,  // 向右
    UP = 2,     // 向上
    DOWN = 3    // 向下
};

// 定义动画的类型
enum AnimationType
{
    MOVE = 0,       // 方格移动动画
    APPEARANCE = 1  // 方格出现动画
};

// 动画结构体
struct Animation
{
    AnimationType type;     // 动画类型
    GestureDirect direct;   // 方向
    QPointF startPos;       // 起始点坐标 出现动画仅仅使用这个坐标
    QPointF endPos;         // 终止点坐标 移动动画的终点坐标
    int digit;              // 数码
    int digit2;             // 第二数码 数码可能被合并
};

// 游戏部件类 继承自QWidget
class GameWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    // 构造函数
    explicit GameWidget(QWidget *parent = 0);

private:
    void init2Block();          // 初始化两个方格
    bool checkGameOver();      // 检测游戏是否结束
    bool checkWin();     // 检测游戏是否获胜
    int getBitCount(int);  //获取一个数字对应颜色数组的下标。比如2对应0，8对应2。）

    void resizeEvent(QResizeEvent *);    // 窗口尺寸改变事件

    int board[4][4];     // 游戏面板 存储每个格子的数值
    int digitCount;    // 数码的个数 存储当前面板上的数字的个数
    int score;  // 分数 存储当前得分
    qreal w, h;    // 小格子的宽度和高度
    qreal ratioW, ratioH;      // 宽度和高度的缩放比例
    qreal rX, rY;    // 小圆角的 x y
    QTimer timer;      // 计时器

    // 以下为一些信号
signals:

    void ScoreInc(int);          // 分数增加信号
    void GameOver();    // 游戏结束信号
    void win();   // 游戏获胜信号

    // 以下为一些槽函数
public slots:
    void restart();   // 重新开始的槽函数

};

#endif // GAMEWIDGET_H
