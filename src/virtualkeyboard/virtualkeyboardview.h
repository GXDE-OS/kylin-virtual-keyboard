#ifndef VIRTUALKEYBOARDVIEW_H
#define VIRTUALKEYBOARDVIEW_H

#include <QObject>
#include <QQuickView>

#include <memory>

class VirtualKeyboardView : public QObject {
    Q_OBJECT
public:
    explicit VirtualKeyboardView(QObject *parent = nullptr);
    ~VirtualKeyboardView() override;

    QObject *rootObject() const;

    QRect geometry() const;

signals:
    void updatePreeditArea(const QString &preeditText);
    void updateCandidateArea(const QVariant &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void changeIM(const QString &uniqueName);
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
    void showView();

private:
    std::unique_ptr<QQuickView> view_ = nullptr;

    static const QString fcitx5Service;
    static const QString fcitx5ServiceControllerPath;
    static const QString fcitx5ServiceControllerInterface;
};

#endif // VIRTUALKEYBOARDVIEW_H
