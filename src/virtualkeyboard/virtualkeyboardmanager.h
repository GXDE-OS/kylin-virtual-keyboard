#ifndef VIRTUALKEYBOARDMANAGER_H
#define VIRTUALKEYBOARDMANAGER_H

#include <memory>

#include <QObject>

#include "appinputareamanager.h"
#include "geometrymanager/expansiongeometrymanager.h"
#include "geometrymanager/floatgeometrymanager.h"
#include "localsettings/viewlocalsettings.h"
#include "placementmodemanager.h"
#include "virtualkeyboardmodel.h"
#include "virtualkeyboardview.h"

class VirtualKeyboardManager : public QObject {
    Q_OBJECT

public:
    explicit VirtualKeyboardManager(QObject *parent = nullptr);
    ~VirtualKeyboardManager();

    void ShowVirtualKeyboard();
    void VisibiltyChanged();
    bool IsVirtualKeyboardVisible() const;
    void UpdatePreeditCaret(int index);
    void UpdatePreeditArea(const QString &preeditText);
    void UpdateCandidateArea(const QStringList &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void NotifyIMActivated(const QString &uniqueName);
    void NotifyIMDeactivated(const QString &uniqueName);
    void NotifyIMListChanged();

signals:
    void virtualKeyboardVisibiltyChanged(bool isShow);

public slots:
    void HideVirtualKeyboard();

    void processResolutionChangedEvent();

    void raiseInputArea();
    void fallInputArea();

private:
    void initAppInputAreaManager();
    void initPlacementModeManager();
    void initGeometryManager();
    void initVirtualKeyboardModel();

    void initScreenSignalConnections();
    void initPlacementModeManagerSignalConnections();

    void initView();
    void destoryView();

    void connectSignals();
    void connectVirtualKeyboardModelSignals();
    void connectGeometryManagerSignals();
    void connectRootObjectSignals();
    void connectPlacementModeManagerSignals();

    std::unique_ptr<AppInputAreaManager> appInputAreaManager_ = nullptr;
    std::unique_ptr<VirtualKeyboardView> view_ = nullptr;
    std::unique_ptr<VirtualKeyboardModel> model_ = nullptr;
    std::unique_ptr<PlacementModeManager> placementModeManager_ = nullptr;
    std::unique_ptr<FloatGeometryManager> floatGeometryManager_ = nullptr;
    std::unique_ptr<ExpansionGeometryManager> expansionGeometryManager_ =
        nullptr;
    bool virtualkeyboardVisible_ = false;

    ViewLocalSettings viewSettings_{"kylinsoft", "kylin virtual keyboard"};
};

#endif // VIRTUALKEYBOARDMANAGER_H
