#ifndef APPINPUTAREAMANAGER_H
#define APPINPUTAREAMANAGER_H

#include <QObject>
#include <QRect>
#include <QTimer>
#include <QWidget>

class AppInputAreaManager : public QObject {
    Q_OBJECT

public:
    explicit AppInputAreaManager(QObject *parent = nullptr);
    ~AppInputAreaManager() = default;
    // TODO(linyuxuan): 使用kdk重新实现该函数
    void raiseInputArea(const QRect &virtualKeyboardRect);
    void fallInputArea();

private:
    QWidget dummyWidget_;
    QTimer oneshotTimer_;
    // 经过指定时间之后顶起应用程序，避免桌面在虚拟键盘显示之前可见
    // 根据使用经验，选定200毫秒作为经验值
    static const int SHOW_DELAY_TIME = 200;
};

#endif // APPINPUTAREAMANAGER_H
