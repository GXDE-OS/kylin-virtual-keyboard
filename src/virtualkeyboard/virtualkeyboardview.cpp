#include "virtualkeyboardview.h"

#include <QQuickItem>

VirtualKeyboardView::VirtualKeyboardView(
    SyncInputMethodNameCallback syncInputMethodNameCallback)
    : view_(new QQuickView()),
      syncInputMethodNameCallback_(std::move(syncInputMethodNameCallback)) {
    init();

    connectSignals();

    syncInputMethodName();

    view_->show();
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

void VirtualKeyboardView::syncInputMethodName() {
    if (!syncInputMethodNameCallback_) {
        return;
    }

    syncInputMethodNameCallback_();
}

QObject *VirtualKeyboardView::rootObject() const { return view_->rootObject(); }

QRect VirtualKeyboardView::geometry() const { return view_->geometry(); }

void VirtualKeyboardView::move(int x, int y) {
    view_->setX(x);
    view_->setY(y);
}

void VirtualKeyboardView::resize(int width, int height) {
    view_->setWidth(width);
    view_->setHeight(height);
}

void VirtualKeyboardView::init() {
    view_->setColor(QColor(Qt::transparent));
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
