// ImageTextButton.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    implicitWidth: contentColumn.implicitWidth
    implicitHeight: contentColumn.implicitHeight

    property alias text: label.text
    property alias font: label.font
    property alias fontColor: label.color
    property alias source: img.source
    property alias iconWidth: img.width
    property alias iconHeight: img.height

    signal clicked

    Column {
        id: contentColumn
        anchors.centerIn: parent
        spacing: 8

        Image {
            id: img
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: root.clicked()
    }
}
