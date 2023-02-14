#include "virtualkeyboardview.h"

#include <QQuickItem>

#include <Fcitx5Qt5/Fcitx5Qt5DBusAddons/fcitxqtcontrollerproxy.h>

// static
const QString VirtualKeyboardView::fcitx5Service = "org.fcitx.Fcitx5";
// static
const QString VirtualKeyboardView::fcitx5ServiceControllerPath = "/controller";
// static
const QString VirtualKeyboardView::fcitx5ServiceControllerInterface =
    "org.fcitx.Fcitx.Controller1";

VirtualKeyboardView::VirtualKeyboardView(QObject *parent)
    : QObject(parent), view_(new QQuickView()) {
    init();

    connectSignals();

    showView();
}

VirtualKeyboardView::~VirtualKeyboardView() {
    if (view_ == nullptr) {
        return;
    }

    if (view_->isVisible()) {
        view_->hide();
    }

    view_.release()->deleteLater();
}

void VirtualKeyboardView::showView() {
    fcitx::FcitxQtControllerProxy fcitxQtControllerProxy(
        fcitx5Service, fcitx5ServiceControllerPath,
        QDBusConnection::sessionBus(), this);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
        fcitxQtControllerProxy.CurrentInputMethod(), this);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this,
                     [this](QDBusPendingCallWatcher *watcher) {
                         QDBusPendingReply<QString> reply = *watcher;
                         if (!reply.isError()) {
                             const QString &imName = reply.value();
                             emit changeIM(imName);
                         }
                         view_->show();
                     });
}

QObject *VirtualKeyboardView::rootObject() const { return view_->rootObject(); }

QRect VirtualKeyboardView::geometry() const { return view_->geometry(); }

void VirtualKeyboardView::moveVirtualKeyboard(int x, int y) {
    view_->setX(x);
    view_->setY(y);
}

void VirtualKeyboardView::resizeVirtualKeyboard(int width, int height) {
    view_->setWidth(width);
    view_->setHeight(height);
}

void VirtualKeyboardView::init() {
    view_->setSource(QUrl("qrc:/qml/VirtualKeyboard.qml"));
    view_->setFlags(Qt::Window | Qt::WindowDoesNotAcceptFocus |
                    Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                    Qt::BypassWindowManagerHint);
}

void VirtualKeyboardView::connectSignals() {
    connect(this, SIGNAL(updatePreeditArea(const QString &)), rootObject(),
            SIGNAL(qmlUpdatePreedit(QString)));
    connect(this,
            SIGNAL(updateCandidateArea(const QVariant &, bool, bool, int)),
            rootObject(), SIGNAL(qmlUpdateCandidateList(QVariant)));
    connect(this, SIGNAL(changeIM(const QString &)), rootObject(),
            SIGNAL(qmlChangeIM(QString)));
    connect(this, SIGNAL(reset()), rootObject(), SIGNAL(qmlReset()));
    connect(this, SIGNAL(updateCurrentIMList(const QVariant &)), rootObject(),
            SIGNAL(qmlUpdateCurrentIMList(QVariant)));

    connect(this, SIGNAL(expansionModeEntered()), rootObject(),
            SIGNAL(qmlEnterExpansionPlacementMode()));
    connect(this, SIGNAL(floatModeEntered()), rootObject(),
            SIGNAL(qmlEnterFloatPlacementMode()));
}
