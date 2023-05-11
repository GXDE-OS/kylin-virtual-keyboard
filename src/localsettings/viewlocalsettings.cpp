#include "viewlocalsettings.h"

#include <QSettings>
#include <QtConcurrent>

namespace {
void updateSettings(QSettings &settings,
                    const ViewLocalSettings::SettingMap &settingMap) {
    for (auto settingIter = settingMap.constBegin();
         settingIter != settingMap.constEnd(); settingIter++) {
        settings.setValue(settingIter.key(), settingIter.value());
    }
}

void saveSettings(const QString &organization, const QString &application,
                  const ViewLocalSettings::GroupSettingMap &groupSettingMap) {
    QSettings settings(organization, application);

    for (auto groupIter = groupSettingMap.constBegin();
         groupIter != groupSettingMap.constEnd(); groupIter++) {
        settings.beginGroup(groupIter.key());

        updateSettings(settings, groupIter.value());

        settings.endGroup();
    }

    settings.sync();
}
} // namespace

ViewLocalSettings::ViewLocalSettings(const QString &organization,
                                     const QString &application)
    : organization_(organization), application_(application) {
    QObject::connect(&futureWatcher_, &QFutureWatcher<void>::finished,
                     [this]() {
                         if (groupSettingMap_.isEmpty()) {
                             return;
                         }

                         saveSettingsAsync();
                     });
}

ViewLocalSettings::~ViewLocalSettings() = default;

QVariant
ViewLocalSettings::getValue(const QString &group, const QString &key,
                            const QVariant &defaultValue /*= QVariant()*/) {
    QSettings settings(organization_, application_);

    settings.beginGroup(group);

    auto value = settings.value(key, defaultValue);

    settings.endGroup();

    return value;
}

void ViewLocalSettings::setValue(const QString &group, const QString &key,
                                 const QVariant &value) {
    auto &settingMap = groupSettingMap_[group];
    settingMap.insert(key, value);

    if (futureWatcher_.isFinished()) {
        saveSettingsAsync();
    }
}

void ViewLocalSettings::saveSettingsAsync() {
    auto oneshotGroupSettingMap(std::move(groupSettingMap_));
    futureWatcher_.setFuture(
        QtConcurrent::run([this, oneshotGroupSettingMap]() {
            saveSettings(organization_, application_, oneshotGroupSettingMap);
        }));
}
