#include "virtualkeyboardmodel.h"

#include <QDBusConnection>
#include <QDBusMetaType>
#include <QDBusPendingReply>

VirtualKeyboardModel::VirtualKeyboardModel(QObject *parent) : QObject(parent) {
    initFcitx5Controller();
    initDBusServiceWatcher();
}

void VirtualKeyboardModel::updateCandidateArea(
    const QVariant &candidateTextList, bool /*hasPrev*/, bool /*hasNext*/,
    int /*pageIndex*/, int globalCursorIndex) {
    emit updateCandidateArea(candidateTextList, globalCursorIndex);
}

class FcitxQtIMInfo {
public:
    friend QDBusArgument &operator<<(QDBusArgument &argument,
                                     const FcitxQtIMInfo &arg);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument,
                                           FcitxQtIMInfo &arg);
    static void registerDBusMetaType();
    void setUniqueName(const QString &uniqueName) { uniqueName_ = uniqueName; }
    void setLocalName(const QString &localName) { localName_ = localName; }
    void setLabel(const QString &label) { label_ = label; }
    const QString &getUniqueName() const { return uniqueName_; }
    const QString &getLocalName() const { return localName_; }
    const QString &getLabel() const { return label_; }

private:
    QString uniqueName_;
    QString localName_;
    QString label_;
};

QDBusArgument &operator<<(QDBusArgument &argument, const FcitxQtIMInfo &arg) {
    argument.beginStructure();
    argument << arg.uniqueName_;
    argument << arg.localName_;
    argument << arg.label_;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                FcitxQtIMInfo &arg) {
    QString uniqueName;
    QString localName;
    QString label;
    argument.beginStructure();
    argument >> uniqueName >> localName >> label;
    argument.endStructure();
    arg.setUniqueName(uniqueName);
    arg.setLocalName(localName);
    arg.setLabel(label);
    return argument;
}

Q_DECLARE_METATYPE(FcitxQtIMInfo)

void FcitxQtIMInfo::registerDBusMetaType() {
    qDBusRegisterMetaType<FcitxQtIMInfo>();
    qDBusRegisterMetaType<QList<FcitxQtIMInfo>>();
}

void VirtualKeyboardModel::selectCandidate(int index) {
    virtualKeyboardBackendInterface_->asyncCall("SelectCandidate", index);
}

void VirtualKeyboardModel::setCurrentIM(const QString &imName) {
    fcitx5Controller_->SetCurrentIM(imName);
}

void VirtualKeyboardModel::processKeyEvent(const QString & /*keyval*/,
                                           int keycode, int state,
                                           bool isRelease, int time) {
    virtualKeyboardBackendInterface_->asyncCall(
        "ProcessKeyEvent", (uint)keycode, (uint)keycode, (uint)state, isRelease,
        (uint)time);
}

void VirtualKeyboardModel::initFcitx5Controller() {
    fcitx5Controller_.reset(new fcitx::FcitxQtControllerProxy(
        "org.fcitx.Fcitx5", "/controller", QDBusConnection::sessionBus()));
}

void VirtualKeyboardModel::initDBusServiceWatcher() {
    serviceWatcher_.reset(new QDBusServiceWatcher(this));
    serviceWatcher_->setConnection(QDBusConnection::sessionBus());
    serviceWatcher_->addWatchedService(virtualKeyboardBackendService);
    serviceWatcher_->setWatchMode(QDBusServiceWatcher::WatchForRegistration |
                                  QDBusServiceWatcher::WatchForUnregistration);
    connect(serviceWatcher_.get(), SIGNAL(serviceRegistered(const QString &)),
            this, SLOT(backendServiceRegistered(const QString &)));
    connect(serviceWatcher_.get(), SIGNAL(serviceUnregistered(const QString &)),
            this, SLOT(backendServiceUnregistered(const QString &)));
}

void VirtualKeyboardModel::initVirtualKeyboardBackendInterface() {
    virtualKeyboardBackendInterface_.reset(new QDBusInterface(
        virtualKeyboardBackendService, virtualKeyboardBackendServicePath,
        virtualKeyboardBackendServiceInterface, QDBusConnection::sessionBus(),
        this));
}

void VirtualKeyboardModel::backendServiceRegistered(
    const QString &serviceName) {
    if (serviceName != virtualKeyboardBackendService) {
        return;
    }
    initVirtualKeyboardBackendInterface();

    syncUniqueName();

    syncCurrentIMList();
}

void VirtualKeyboardModel::backendServiceUnregistered(
    const QString &serviceName) {
    if (serviceName != virtualKeyboardBackendService) {
        return;
    }

    emit backendConnectionDisconnected();

    virtualKeyboardBackendInterface_.reset();
}

QString VirtualKeyboardModel::getUniqueName() const { return uniqueName_; }

void VirtualKeyboardModel::setUniqueName(const QString &uniqueName) {
    if (uniqueName_ == uniqueName) {
        return;
    }

    uniqueName_ = uniqueName;

    emit uniqueNameChanged();
}

void VirtualKeyboardModel::syncUniqueName() {
    QDBusPendingReply<QString> reply = fcitx5Controller_->CurrentInputMethod();
    reply.waitForFinished();

    setUniqueName(reply.value());
}

QVariant VirtualKeyboardModel::getCurrentIMList() const {
    return currentIMList_;
}

void VirtualKeyboardModel::setCurrentIMList(
    const QVariant &currentInputMethodList) {
    if (currentIMList_ == currentInputMethodList) {
        return;
    }

    currentIMList_ = currentInputMethodList;

    emit currentIMListChanged();
}

void VirtualKeyboardModel::syncCurrentIMList() {
    FcitxQtIMInfo::registerDBusMetaType();
    QDBusPendingReply<QList<FcitxQtIMInfo>> reply =
        virtualKeyboardBackendInterface_->asyncCall("CurrentIMList");
    reply.waitForFinished();

    auto items = reply.value();

    QStringList stringList;
    for (const auto &imInfo : items) {
        stringList.append(imInfo.getUniqueName() + "|" + imInfo.getLocalName() +
                          "|" + imInfo.getLabel());
    }

    setCurrentIMList(QVariant(stringList));
}
