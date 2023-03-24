#ifndef VIEWLOCALSETTINGS_H
#define VIEWLOCALSETTINGS_H

#include <QSettings>

#include "localsettings.h"

class ViewLocalSettings : public LocalSettings {
public:
    ViewLocalSettings(const QString &organization, const QString &application);
    ~ViewLocalSettings() override;

    QVariant getValue(const QString &group, const QString &key,
                      const QVariant &defaultValue) override;
    void setValue(const QString &group, const QString &key,
                  const QVariant &value) override;

private:
    QSettings settings;
};

#endif // VIEWLOCALSETTINGS_H
