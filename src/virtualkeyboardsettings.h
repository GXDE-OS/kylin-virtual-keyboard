#ifndef VIRTUALKEYBOARDSETTINGS_H
#define VIRTUALKEYBOARDSETTINGS_H

#include <memory>

#include <QSettings>
#include <QVariant>

class VirtualKeyboardSettings {
public:
    static VirtualKeyboardSettings &getInstance();

    ~VirtualKeyboardSettings();

    QVariant getValue(const QString &group, const QString &key,
                      const QVariant &defaultValue = QVariant());
    void setValue(const QString &group, const QString &key,
                  const QVariant &value);

private:
    VirtualKeyboardSettings();

private:
    QSettings settings;
};

#endif // VIRTUALKEYBOARDSETTINGS_H
