// Icon.qml
import QtQuick
import QtQuick.Controls

Item {
    id: root
    enabled: false

    property alias source: img.source
    property alias text: textIcon.text
    property alias font: textIcon.font
    property alias textColor: textIcon.color
    property alias color: background.color
    property alias backgroundOpacity: background.opacity
    property alias radius: background.radius
    property alias hovered: mouseArea.containsMouse

    signal clicked

    Rectangle {
        id: background
        radius: width / 2
        anchors.fill: parent
    }

    Image {
        id: img
        visible: source !== ""
        width: root.width / 2
        height: root.height / 2
        anchors.centerIn: parent
    }

    Text {
        id: textIcon
        visible: text !== ""
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea
        enabled: root.enabled
        hoverEnabled: enabled
        anchors.fill: parent

        onClicked: root.clicked()
    }
}
