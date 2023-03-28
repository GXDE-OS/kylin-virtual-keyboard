#ifndef FCITXVIRTUALKEYBOARDSERVICEPROXY_H
#define FCITXVIRTUALKEYBOARDSERVICEPROXY_H

#include <memory>

#include <QDBusInterface>
#include <QString>

#include "virtualkeyboardentry/fcitxvirtualkeyboardservice.h"

class FcitxVirtualKeyboardServiceProxy : public FcitxVirtualKeyboardService {
public:
    FcitxVirtualKeyboardServiceProxy();
    ~FcitxVirtualKeyboardServiceProxy() override = default;

    void showVirtualKeyboard() const override;
    void hideVirtualKeyboard() const override;

private:
    std::unique_ptr<QDBusInterface> virtualKeyboardService = nullptr;

    static const QString serviceName_;
    static const QString servicePath_;
    static const QString serviceInterface_;
};

#endif // FCITXVIRTUALKEYBOARDSERVICEPROXY_H
