#ifndef VIEWLOCALSETTINGS_H
#define VIEWLOCALSETTINGS_H

#include <QFutureWatcher>
#include <QMap>

#include "localsettings.h"

class ViewLocalSettings : public LocalSettings {
public:
    using SettingMap = QMap<QString, QVariant>;
    using GroupSettingMap = QMap<QString, SettingMap>;

public:
    ViewLocalSettings(const QString &organization, const QString &application);
    ~ViewLocalSettings() override;

    QVariant getValue(const QString &group, const QString &key,
                      const QVariant &defaultValue) override;
    void setValue(const QString &group, const QString &key,
                  const QVariant &value) override;

private:
    void saveSettingsAsync();

private:
    const QString organization_;
    const QString application_;

    GroupSettingMap groupSettingMap_;
    QFutureWatcher<void> futureWatcher_;
};

#endif // VIEWLOCALSETTINGS_H
