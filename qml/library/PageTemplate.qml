// PageTemplate.qml
import QtQuick
import QtQuick.Controls

Page {
    id: root

    property Component bodyItem

    background: Rectangle {
        anchors.fill: parent
        color: Styles.background
    }

    Flickable {
        id: bodyFlicker
        clip: true
        anchors.fill: parent

        contentWidth: width
        contentHeight: Math.max(bodyLoader.implicitHeight, height)

        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds

        Loader {
            id: bodyLoader
            width: bodyFlicker.width
            height: Math.max(implicitHeight, bodyFlicker.height)
            sourceComponent: root.bodyItem
        }
    }
}
