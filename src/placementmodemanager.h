#ifndef PLACEMENTMODEMANAGER_H
#define PLACEMENTMODEMANAGER_H

#include <QObject>
#include <QSettings>

class VirtualKeyboardManager;

enum class PlacementMode
{
    Expansion,
    Float
};

class PlacementModeManager : public QObject
{
    Q_OBJECT
public:
    explicit PlacementModeManager(VirtualKeyboardManager* virtualKeyboardManager, QObject *parent = nullptr);
    ~PlacementModeManager() = default;
    void updatePlacementMode();
signals:
    void expansionModeEntered();
    void floatModeEntered();
public slots:
    void flipPlacementMode();
    
private:
    void enterExpansionMode();
    void enterFloatMode();
    void savePlacementMode();
    PlacementMode loadPlacementMode();
    
    VirtualKeyboardManager* virtualKeyboardManager_ = nullptr;
    PlacementMode placementMode_ = PlacementMode::Expansion;
    QSettings settings_;
};

#endif // PLACEMENTMODEMANAGER_H
