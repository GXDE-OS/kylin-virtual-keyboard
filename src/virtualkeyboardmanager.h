#ifndef VIRTUALKEYBOARDMANAGER_H
#define VIRTUALKEYBOARDMANAGER_H

#include <memory>
#include <Fcitx5Qt5/Fcitx5Qt5DBusAddons/fcitxqtcontrollerproxy.h>
#include <KWindowSystem>
#include <QApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QDBusReply>
#include <QDBusServiceWatcher>
#include <QGuiApplication>
#include <QObject>
#include <QQuickView>
#include <QScreen>
#include <QVariant>

#include "appinputareamanager.h"
#include "eventhandler.h"
#include "expansiongeometrymanager.h"
#include "floatgeometrymanager.h"
#include "placementmodemanager.h"
#include "virtualkeyboardview.h"

class VirtualKeyboardManager : public QObject {
    Q_OBJECT

public:
    explicit VirtualKeyboardManager(QObject *parent = nullptr);
    ~VirtualKeyboardManager();
    QDBusInterface *getVirtualKeyboardBackendInterface() {
        return virtualKeyboardBackendInterface_.get();
    }
    QDBusInterface *getFcitx5ControllerInterface() {
        return fcitx5ControllerInterface_.get();
    }
    void ShowVirtualKeyboard();
    void HideVirtualKeyboard();
    void VisibiltyChanged();
    bool IsVirtualKeyboardVisible();
    void UpdatePreeditCaret(int index);
    void UpdatePreeditArea(const QString &preeditText);
    void UpdateCandidateArea(const QStringList &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void NotifyIMActivated(const QString &uniqueName);
    void NotifyIMDeactivated(const QString &uniqueName);
    void NotifyIMListChanged();

public slots:
    void hideVirtualKeyboard();
    void requestCurrentIMList();

private slots:
    void backendServiceRegistered(const QString &serviceName);
    void backendServiceUnregistered(const QString &serviceName);
    void imListChanged(QDBusPendingCallWatcher *imChangedCall);
    void processResolutionChangedEvent();
    void raiseInputArea();
    void fallInputArea();

signals:
    void updatePreeditCaret(int index);
    void updatePreeditArea(const QString &preeditText);
    // TODO(linyuxuan): 使用更准确的类型替换QVariant
    void updateCandidateArea(const QVariant &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void changeIM(const QString &uniqueName);
    void reset();
    // TODO(linyuxuan): 使用更准确的类型替换QVariant
    void updateCurrentIMList(const QVariant &currentIMList);

private:
    void initView();
    void destoryView();

    void initDBusServiceWatcher();
    void initVirtualKeyboardBackendInterface();
    void initFcitx5ControllerInterface();
    void initEventHandler();
    void initAppInputAreaManager();
    void initPlacementModeManager();
    void initGeometryManager();
    void initScreenSignalConnections();
    void initPlacementModeManagerSignalConnections();

    void connectVirtualKeyboardManagerSignals();
    void connectGeometryManagerSignals();
    void connectRootObjectSignals();
    void connectPlacementModeManagerSignals();
    void connectSignals();

    std::unique_ptr<AppInputAreaManager> appInputAreaManager_ = nullptr;
    std::unique_ptr<VirtualKeyboardView> view_ = nullptr;
    std::unique_ptr<EventHandler> eventHandler_ = nullptr;
    std::unique_ptr<PlacementModeManager> placementModeManager_ = nullptr;
    std::unique_ptr<FloatGeometryManager> floatGeometryManager_ = nullptr;
    std::unique_ptr<ExpansionGeometryManager> expansionGeometryManager_ =
        nullptr;
    std::unique_ptr<QDBusServiceWatcher> serviceWatcher_ = nullptr;
    std::unique_ptr<QDBusInterface> virtualKeyboardBackendInterface_ = nullptr;
    std::unique_ptr<QDBusInterface> fcitx5ControllerInterface_ = nullptr;
    bool virtualkeyboardVisible_ = false;
    QString virtualKeyboardBackendService =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend";
    QString virtualKeyboardBackendServicePath = "/virtualkeyboard";
    QString virtualKeyboardBackendServiceInterface =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend1";
    QString fcitx5Service = "org.fcitx.Fcitx5";
    QString fcitx5ServiceControllerPath = "/controller";
    QString fcitx5ServiceControllerInterface = "org.fcitx.Fcitx.Controller1";
};

#endif // VIRTUALKEYBOARDMANAGER_H
