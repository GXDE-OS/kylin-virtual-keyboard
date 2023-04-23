#include "fcitxvirtualkeyboardserviceproxy.h"

// static
const QString FcitxVirtualKeyboardServiceProxy::serviceName_ =
    "org.fcitx.Fcitx5";
// static
const QString FcitxVirtualKeyboardServiceProxy::servicePath_ =
    "/virtualkeyboard";
// static
const QString FcitxVirtualKeyboardServiceProxy::serviceInterface_ =
    "org.fcitx.Fcitx.VirtualKeyboard1";

FcitxVirtualKeyboardServiceProxy::FcitxVirtualKeyboardServiceProxy() {
    virtualKeyboardService.reset(
        new QDBusInterface(serviceName_, servicePath_, serviceInterface_,
                           QDBusConnection::sessionBus()));
}

void FcitxVirtualKeyboardServiceProxy::showVirtualKeyboard() const {
    virtualKeyboardService->call("ShowVirtualKeyboard");
}

void FcitxVirtualKeyboardServiceProxy::hideVirtualKeyboard() const {
    virtualKeyboardService->call("HideVirtualKeyboard");
}
