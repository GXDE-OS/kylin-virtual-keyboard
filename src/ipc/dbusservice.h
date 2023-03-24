#ifndef DBUSSERVICE_H
#define DBUSSERVICE_H

#include <QDBusConnection>
#include <QObject>

#include "requestmerger.h"
#include "virtualkeyboard/virtualkeyboardmanager.h"

class DBusService : public QObject {
    Q_OBJECT

public:
    explicit DBusService(VirtualKeyboardManager *virtualKeyboardManager);
    ~DBusService() override;

private:
    void initRequestMerger();

    bool startService();
    bool stopService();

public slots:
    void ShowVirtualKeyboard();
    void HideVirtualKeyboard();
    bool IsVirtualKeyboardVisible();
    void UpdatePreeditCaret(int preeditCursor);
    void UpdatePreeditArea(const QString &preeditText);
    void UpdateCandidateArea(const QStringList &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void NotifyIMActivated(const QString &uniqueName);
    void NotifyIMDeactivated(const QString &uniqueName);
    void NotifyIMListChanged();

private:
    VirtualKeyboardManager *virtualKeyboardManager_ = nullptr;
    QString serviceName_ = "org.fcitx.Fcitx5.VirtualKeyboard";
    QString servicePath_ = "/org/fcitx/virtualkeyboard/impanel";
    QString serviceInterface_ = "org.fcitx.Fcitx5.VirtualKeyboard1";

    // 在10ms之内的显示和隐藏虚拟键盘的请求将会被合并处理，
    // 避免虚拟键盘不必要的显示和隐藏及其相应的闪烁效果
    const int VIRTUAL_KEYBOARD_VISIBILITY_PEROID = 10;
    RequestMerger virtualKeyboardVisibilityRequestMerger_;
};

#endif // DBUSSERVICE_H
