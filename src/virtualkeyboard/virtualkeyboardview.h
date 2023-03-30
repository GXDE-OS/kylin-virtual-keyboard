#ifndef VIRTUALKEYBOARDVIEW_H
#define VIRTUALKEYBOARDVIEW_H

#include <functional>
#include <memory>

#include <QObject>
#include <QQuickView>

class VirtualKeyboardView : public QObject {
    Q_OBJECT

public:
    using SyncInputMethodNameCallback = std::function<void()>;

public:
    explicit VirtualKeyboardView(
        SyncInputMethodNameCallback syncInputMethodNameCallback);
    ~VirtualKeyboardView() override;

    QObject *rootObject() const;

    QRect geometry() const;

signals:
    void updatePreeditArea(const QString &preeditText);
    void updateCandidateArea(const QVariant &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void inputMethodNameArrived(const QString &uniqueName);
    void reset();
    void updateCurrentIMList(const QVariant &currentIMList);

    void expansionModeEntered();
    void floatModeEntered();

public slots:
    void move(int x, int y);
    void resize(int width, int height);

private:
    void init();
    void connectSignals();
    void syncInputMethodName();

private:
    std::unique_ptr<QQuickView> view_ = nullptr;

    SyncInputMethodNameCallback syncInputMethodNameCallback_;
};

#endif // VIRTUALKEYBOARDVIEW_H
