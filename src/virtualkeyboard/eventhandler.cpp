#include "eventhandler.h"
#include "virtualkeyboardmanager.h"

EventHandler::EventHandler(VirtualKeyboardManager *virtualKeyboardManager,
                           QObject *parent)
    : QObject(parent), virtualKeyboardManager_(virtualKeyboardManager) {}

void EventHandler::connectSignals(const QObject *rootObject) {
    connect(rootObject, SIGNAL(qmlKeyEvent(QString, int, int, bool, int)), this,
            SLOT(ProcessKeyEvent(QString, int, int, bool, int)));
    connect(rootObject, SIGNAL(qmlCandidateClicked(int)), this,
            SLOT(SelectCandidate(int)));
    connect(rootObject, SIGNAL(qmlSetCurrentIM(QString)), this,
            SLOT(SetCurrentInputMethod(const QString &)));
}

void EventHandler::SelectCandidate(int index) {
    virtualKeyboardManager_->getVirtualKeyboardBackendInterface()->asyncCall(
        "SelectCandidate", index);
}

void EventHandler::SetCurrentInputMethod(const QString &imName) {
    virtualKeyboardManager_->getFcitx5ControllerInterface()->asyncCall(
        "SetCurrentIM", imName);
}

void EventHandler::ProcessKeyEvent(const QString & /*keyval*/, int keycode,
                                   int state, bool isRelease, int time) {
    virtualKeyboardManager_->getVirtualKeyboardBackendInterface()->asyncCall(
        "ProcessKeyEvent", (uint)keycode, (uint)keycode, (uint)state, isRelease,
        (uint)time);
}
