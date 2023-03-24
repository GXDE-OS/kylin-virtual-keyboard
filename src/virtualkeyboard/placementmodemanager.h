#ifndef PLACEMENTMODEMANAGER_H
#define PLACEMENTMODEMANAGER_H

#include <QObject>

#include "localsettings/localsettings.h"

class PlacementModeManager : public QObject {
    Q_OBJECT

public:
    explicit PlacementModeManager(LocalSettings &viewSettings);
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

private:
    PlacementMode placementMode_ = PlacementMode::Expansion;

    LocalSettings &viewSettings_;

    static const QString placementModeGroup;
    static const QString placementModeKey;
};

#endif // PLACEMENTMODEMANAGER_H
