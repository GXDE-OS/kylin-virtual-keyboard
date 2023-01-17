#include "virtualkeyboardsettings.h"

// static
VirtualKeyboardSettings &VirtualKeyboardSettings::getInstance() {
    static VirtualKeyboardSettings instance;

    return instance;
}

VirtualKeyboardSettings::VirtualKeyboardSettings()
    : settings("kylinos", "kylin-virtual-keyboard") {}

VirtualKeyboardSettings::~VirtualKeyboardSettings() = default;

QVariant VirtualKeyboardSettings::getValue(
    const QString &key, const QVariant &defaultValue /*= QVariant()*/) {
    return settings.value(key, defaultValue);
}

void VirtualKeyboardSettings::setValue(const QString &key,
                                       const QVariant &value) {
    settings.setValue(key, value);
}
