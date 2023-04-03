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

    void showVirtualKeyboard();
    Q_INVOKABLE void hideVirtualKeyboard();
    Q_INVOKABLE void flipPlacementMode();
    Q_INVOKABLE void moveBy(int offsetX, int offsetY);
    Q_INVOKABLE void endDrag();
    void visibiltyChanged();
    bool isVirtualKeyboardVisible() const;
    void updatePreeditCaret(int index);
    void updatePreeditArea(const QString &preeditText);
    void updateCandidateArea(const QStringList &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void notifyIMActivated(const QString &uniqueName);
    void notifyIMDeactivated(const QString &uniqueName);
    void notifyIMListChanged();

signals:
    void virtualKeyboardVisibiltyChanged(bool isShow);

public slots:
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
