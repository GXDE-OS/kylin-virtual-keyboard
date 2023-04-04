#ifndef FLOATBUTTONMANAGER_H
#define FLOATBUTTONMANAGER_H

#include <memory>

#include <QDateTime>
#include <QObject>
#include <QPushButton>

#include "localsettings/viewlocalsettings.h"
#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/fcitxvirtualkeyboardservice.h"
#include "virtualkeyboardentry/floatbutton.h"

class FloatButtonManager : public QObject {
    Q_OBJECT

public:
    FloatButtonManager(
        const VirtualKeyboardManager &virtualKeyboardManager,
        const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService);
    ~FloatButtonManager() override = default;

    void loadFloatButtonAvailability();

    void enableFloatButton();
    void disableFloatButton();

signals:
    void floatButtonEnabled();
    void floatButtonDisabled();

private slots:
    void initFloatButton();
    void destroyFloatButton();

    void onScreenResolutionChanged();

private:
    void initGeometryManager();

    void initInternalSignalConnections();
    void initScreenSignalConnections();

    void createFloatButton();
    void connectFloatButtonSignals();

    void showFloatButton();
    void hideFloatButton();

    void updateFloatButtonEnabled(bool enabled);
    void setFloatButtonEnabled(bool enabled);

    void saveFloatButtonAvailablity();

private:
    bool floatButtonEnabled_ = true;

    ViewLocalSettings floatButtonSettings_{"kylinsoft", "kylin float button"};

    const VirtualKeyboardManager &virtualKeyboardManager_;
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService_;

    std::unique_ptr<FloatButton> floatButton_ = nullptr;

    std::unique_ptr<FloatGeometryManager> geometryManager_ = nullptr;
};

#endif // FLOATBUTTONMANAGER_H
