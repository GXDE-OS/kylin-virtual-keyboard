#include "virtualkeyboardview.h"

#include <QQmlContext>
#include <QQuickItem>

VirtualKeyboardView::VirtualKeyboardView(QObject *model)
    : view_(new QQuickView()) {
    init(model);

    connectSignals();

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

void VirtualKeyboardView::init(QObject *model) {
    view_->rootContext()->setContextProperty("model", model);

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
    connect(this, SIGNAL(reset()), rootObject(), SIGNAL(qmlReset()));

    connect(this, SIGNAL(expansionModeEntered()), rootObject(),
            SIGNAL(qmlEnterExpansionPlacementMode()));
    connect(this, SIGNAL(floatModeEntered()), rootObject(),
            SIGNAL(qmlEnterFloatPlacementMode()));
}
