#include "viewlocalsettings.h"

ViewLocalSettings::ViewLocalSettings(const QString &organization,
                                     const QString &application)
    : settings(organization, application) {}

ViewLocalSettings::~ViewLocalSettings() = default;

QVariant
ViewLocalSettings::getValue(const QString &group, const QString &key,
                            const QVariant &defaultValue /*= QVariant()*/) {
    settings.beginGroup(group);

    auto value = settings.value(key, defaultValue);

    settings.endGroup();

    return value;
}

void ViewLocalSettings::setValue(const QString &group, const QString &key,
                                 const QVariant &value) {
    settings.beginGroup(group);

    settings.setValue(key, value);

    settings.endGroup();
}
