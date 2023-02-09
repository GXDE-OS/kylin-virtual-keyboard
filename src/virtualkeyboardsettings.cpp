#include "virtualkeyboardsettings.h"

// static
VirtualKeyboardSettings &VirtualKeyboardSettings::getInstance() {
    static VirtualKeyboardSettings instance;

    return instance;
}

VirtualKeyboardSettings::VirtualKeyboardSettings()
    : settings("kylinsoft", "kylin virtual keyboard") {}

VirtualKeyboardSettings::~VirtualKeyboardSettings() = default;

QVariant VirtualKeyboardSettings::getValue(
    const QString &group, const QString &key,
    const QVariant &defaultValue /*= QVariant()*/) {
    settings.beginGroup(group);

    auto value = settings.value(key, defaultValue);

    settings.endGroup();

    return value;
}

void VirtualKeyboardSettings::setValue(const QString &group, const QString &key,
                                       const QVariant &value) {
    settings.beginGroup(group);

    settings.setValue(key, value);

    settings.endGroup();
}
