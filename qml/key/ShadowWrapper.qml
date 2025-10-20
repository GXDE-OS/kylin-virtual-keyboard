import QtQuick 2.0

Item {
    id: wrapper

    property var source
    property color color
    property real horizontalOffset: 0
    property real verticalOffset: 0
    property real radius: virtualKeyboard.keyRadius
    property int samples: 100
    property int qtVersionMajor: QT_VERSION_MAJOR
    property bool isQt6: qtVersionMajor >= 6

    anchors.fill: parent
    onSourceChanged: {
        if (effectLoader.item)
            effectLoader.item.source = wrapper.source;

    }
    onColorChanged: {
        if (effectLoader.item)
            effectLoader.item.color = wrapper.color;

    }
    onHorizontalOffsetChanged: {
        if (effectLoader.item)
            effectLoader.item.horizontalOffset = wrapper.horizontalOffset;

    }
    onVerticalOffsetChanged: {
        if (effectLoader.item)
            effectLoader.item.verticalOffset = wrapper.verticalOffset;

    }
    onRadiusChanged: {
        if (effectLoader.item)
            effectLoader.item.radius = wrapper.radius;

    }
    onSamplesChanged: {
        if (effectLoader.item)
            effectLoader.item.samples = wrapper.samples;

    }
    onZChanged: {
        if (effectLoader.item)
            effectLoader.item.z = wrapper.z;

    }

    Loader {
        id: effectLoader

        anchors.fill: parent
        source: isQt6 ? "GraphicalEffectsQt6.qml" : "GraphicalEffectsQt5.qml"
        onLoaded: {
            if (item) {
                item.source = wrapper.source;
                item.color = wrapper.color;
                item.horizontalOffset = wrapper.horizontalOffset;
                item.verticalOffset = wrapper.verticalOffset;
                item.radius = wrapper.radius;
                item.samples = wrapper.samples;
                item.z = wrapper.z;
            }
        }
    }

}
