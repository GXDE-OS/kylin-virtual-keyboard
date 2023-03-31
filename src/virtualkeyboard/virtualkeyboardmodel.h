#ifndef VIRTUALKEYBOARDMODEL_H
#define VIRTUALKEYBOARDMODEL_H

#include <memory>

#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QObject>

#include <Fcitx5Qt5/Fcitx5Qt5DBusAddons/fcitxqtcontrollerproxy.h>

class VirtualKeyboardModel : public QObject {
    Q_OBJECT
public:
    explicit VirtualKeyboardModel(QObject *parent = nullptr);
    ~VirtualKeyboardModel() override = default;

    void syncInputMethodName();

    void processVisibilityEvent(bool visible);

public slots:
    void requestCurrentIMList();
    void selectCandidate(int index);
    void setCurrentInputMethod(const QString &imName);
    void processKeyEvent(const QString &keyval, int keycode, int state,
                         bool isRelease, int time);

signals:
    void updatePreeditCaret(int index);
    void updatePreeditArea(const QString &preeditText);
    // TODO(linyuxuan): 使用更准确的类型替换QVariant
    void updateCandidateArea(const QVariant &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void inputMethodNameArrived(const QString &uniqueName);
    void reset();
    // TODO(linyuxuan): 使用更准确的类型替换QVariant
    void updateCurrentIMList(const QVariant &currentIMList);

    void backendConnectionDisconnected();

private slots:
    void backendServiceRegistered(const QString &serviceName);
    void backendServiceUnregistered(const QString &serviceName);

private:
    void initFcitx5Controller();
    void initDBusServiceWatcher();
    void initVirtualKeyboardBackendInterface();

private:
    std::unique_ptr<QDBusServiceWatcher> serviceWatcher_ = nullptr;
    std::unique_ptr<QDBusInterface> virtualKeyboardBackendInterface_ = nullptr;
    std::unique_ptr<fcitx::FcitxQtControllerProxy> fcitx5Controller_ = nullptr;

    QString virtualKeyboardBackendService =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend";
    QString virtualKeyboardBackendServicePath = "/virtualkeyboard";
    QString virtualKeyboardBackendServiceInterface =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend1";
};

#endif // VIRTUALKEYBOARDMODEL_H
