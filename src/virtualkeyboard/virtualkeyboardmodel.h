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

public:
    Q_PROPERTY(QString uniqueName READ getUniqueName NOTIFY uniqueNameChanged);
    Q_PROPERTY(QVariant currentIMList READ getCurrentIMList NOTIFY
                   currentIMListChanged);

signals:
    void uniqueNameChanged();

    void currentIMListChanged();

public:
    void setUniqueName(const QString &uniqueName);

    void syncCurrentIMList();

    void processVisibilityEvent(bool visible);

public:
    Q_INVOKABLE void selectCandidate(int index);
    Q_INVOKABLE void setCurrentIM(const QString &imName);
    Q_INVOKABLE void processKeyEvent(const QString &keyval, int keycode,
                                     int state, bool isRelease, int time);

signals:
    void updatePreeditCaret(int index);
    void updatePreeditArea(const QString &preeditText);
    // TODO(linyuxuan): 使用更准确的类型替换QVariant
    void updateCandidateArea(const QVariant &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void reset();

    void backendConnectionDisconnected();

private slots:
    void backendServiceRegistered(const QString &serviceName);
    void backendServiceUnregistered(const QString &serviceName);

private:
    void initFcitx5Controller();
    void initDBusServiceWatcher();
    void initVirtualKeyboardBackendInterface();

    QString getUniqueName() const;
    void syncUniqueName();

    QVariant getCurrentIMList() const;
    void setCurrentIMList(const QVariant &currentIMList);

private:
    std::unique_ptr<QDBusServiceWatcher> serviceWatcher_ = nullptr;
    std::unique_ptr<QDBusInterface> virtualKeyboardBackendInterface_ = nullptr;
    std::unique_ptr<fcitx::FcitxQtControllerProxy> fcitx5Controller_ = nullptr;

    QString uniqueName_;

    QVariant currentIMList_;

    QString virtualKeyboardBackendService =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend";
    QString virtualKeyboardBackendServicePath = "/virtualkeyboard";
    QString virtualKeyboardBackendServiceInterface =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend1";
};

#endif // VIRTUALKEYBOARDMODEL_H
