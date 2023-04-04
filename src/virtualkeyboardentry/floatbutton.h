#ifndef FLOATBUTTON_H
#define FLOATBUTTON_H

#include <functional>
#include <memory>

#include <QMouseEvent>
#include <QPushButton>
#include <QSize>
#include <QTimer>

class FloatButton : public QPushButton {
    Q_OBJECT

public:
    using MouseClickedCallback = std::function<void()>;

public:
    explicit FloatButton(MouseClickedCallback mouseClickedCallback);
    ~FloatButton() override = default;

signals:
    void mouseMoved(int x, int y);
    void mouseReleased();

public slots:
    void move(int x, int y);
    void resize(int width, int height);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    bool shouldPerformMouseClick() const;
    void processMouseReleaseEvent();
    void processMouseClickEvent();
    void processMouseMoveEvent(QMouseEvent *event);

    void updateManhattonLength(QMouseEvent *event);
    bool isFloatButtonMoved() const;

    void initStyle();
    void updateBorderRadius();

    void startClickTimer();
    void stopClickTimer();

private:
    int startX_ = -1;
    int startY_ = -1;

    int manhattonLength = 0;
    constexpr static int manhattonLengthThreshold = 10;

    MouseClickedCallback mouseClickedCallback_;
    std::unique_ptr<QTimer> clickTimer_ = nullptr;
    const int clickTimeThreshold_ = 600;
};

#endif // FLOATBUTTON_H
