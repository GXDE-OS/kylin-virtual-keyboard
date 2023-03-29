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
#include "geometrymanager/expansiongeometrymanager.h"
#include "geometrymanager/floatgeometrymanager.h"
#include "localsettings/viewlocalsettings.h"
#include "placementmodemanager.h"
#include "virtualkeyboardview.h"

class VirtualKeyboardManager : public QObject {
    Q_OBJECT

public:
    explicit VirtualKeyboardManager(QObject *parent = nullptr);
    ~VirtualKeyboardManager();

    void ShowVirtualKeyboard();
    void HideVirtualKeyboard();
    void VisibiltyChanged();
    bool IsVirtualKeyboardVisible() const;
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
    void processResolutionChangedEvent();
    void raiseInputArea();
    void fallInputArea();

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
    void changeIM(const QString &uniqueName);
    void reset();
    // TODO(linyuxuan): 使用更准确的类型替换QVariant
    void updateCurrentIMList(const QVariant &currentIMList);
    void virtualKeyboardVisibiltyChanged(bool isShow);

private:
    void initView();
    void destoryView();

    void initDBusServiceWatcher();
    void initVirtualKeyboardBackendInterface();
    void initFcitx5Controller();
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

    void syncInputMethodName();

    std::unique_ptr<AppInputAreaManager> appInputAreaManager_ = nullptr;
    std::unique_ptr<VirtualKeyboardView> view_ = nullptr;
    std::unique_ptr<PlacementModeManager> placementModeManager_ = nullptr;
    std::unique_ptr<FloatGeometryManager> floatGeometryManager_ = nullptr;
    std::unique_ptr<ExpansionGeometryManager> expansionGeometryManager_ =
        nullptr;
    std::unique_ptr<QDBusServiceWatcher> serviceWatcher_ = nullptr;
    std::unique_ptr<QDBusInterface> virtualKeyboardBackendInterface_ = nullptr;
    std::unique_ptr<fcitx::FcitxQtControllerProxy> fcitx5Controller_ = nullptr;
    bool virtualkeyboardVisible_ = false;

    ViewLocalSettings viewSettings_{"kylinsoft", "kylin virtual keyboard"};

    QString virtualKeyboardBackendService =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend";
    QString virtualKeyboardBackendServicePath = "/virtualkeyboard";
    QString virtualKeyboardBackendServiceInterface =
        "org.fcitx.Fcitx5.VirtualKeyboardBackend1";
};

#endif // VIRTUALKEYBOARDMANAGER_H
