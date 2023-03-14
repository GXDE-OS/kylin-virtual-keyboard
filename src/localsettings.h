#ifndef LOCALSETTINGS_H
#define LOCALSETTINGS_H

#include <QVariant>

class LocalSettings {
public:
    virtual ~LocalSettings() = default;

    virtual QVariant getValue(const QString &group, const QString &key,
                              const QVariant &defaultValue = QVariant()) = 0;
    virtual void setValue(const QString &group, const QString &key,
                          const QVariant &value) = 0;

protected:
    LocalSettings() = default;
};

#endif // LOCALSETTINGS_H
