#ifndef PLACEMENTMODEMANAGER_H
#define PLACEMENTMODEMANAGER_H

#include <QObject>
#include <QSettings>

class VirtualKeyboardManager;

class PlacementModeManager : public QObject {
    Q_OBJECT

public:
    explicit PlacementModeManager(
        VirtualKeyboardManager *virtualKeyboardManager,
        QObject *parent = nullptr);
    ~PlacementModeManager() override;

    void updatePlacementMode();

    enum PlacementMode { Expansion, Float };
    Q_ENUM(PlacementMode)

signals:
    void expansionModeEntered();
    void floatModeEntered();
public slots:
    void flipPlacementMode();

private:
    void setPlacementMode(PlacementMode placementMode);
    void enterExpansionMode();
    void enterFloatMode();
    void savePlacementMode();
    void loadPlacementMode();

    VirtualKeyboardManager *virtualKeyboardManager_ = nullptr;
    PlacementMode placementMode_ = PlacementMode::Expansion;
    static QString placementModeKey;
};

#endif // PLACEMENTMODEMANAGER_H
