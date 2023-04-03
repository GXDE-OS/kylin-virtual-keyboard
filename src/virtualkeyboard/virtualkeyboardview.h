#ifndef VIRTUALKEYBOARDVIEW_H
#define VIRTUALKEYBOARDVIEW_H

#include <memory>

#include <QObject>
#include <QQuickView>
#include <QString>

class VirtualKeyboardView : public QObject {
    Q_OBJECT

public:
    VirtualKeyboardView(QObject *manager, QObject *model);
    ~VirtualKeyboardView() override;

    QRect geometry() const;

signals:
    void updatePreeditArea(const QString &preeditText);
    void updateCandidateArea(const QVariant &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void imDeactivated();

    void expansionModeEntered();
    void floatModeEntered();

public slots:
    void move(int x, int y);
    void resize(int width, int height);

private:
    void init(QObject *manager, QObject *model);
    void connectSignals();

private:
    std::unique_ptr<QQuickView> view_ = nullptr;
};

#endif // VIRTUALKEYBOARDVIEW_H
