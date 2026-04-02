// TextIconRow.qml
import QtQuick

Item {
    id: root

    width: implicitWidth
    height: implicitHeight
    implicitWidth: contentRow.implicitWidth
    implicitHeight: contentRow.implicitHeight

    property alias text: labelText.text
    property alias textField: labelText
    property alias icon: iconImage

    signal clicked(var mouse)

    Row {
        id: contentRow
        spacing: 4

        Text {
            id: labelText
            anchors.verticalCenter: parent.verticalCenter
            opacity: mouseArea.containsMouse ? Geometry.panel.hoveredItemOpacity : 1
        }

        Image {
            id: iconImage
            anchors.verticalCenter: parent.verticalCenter
            opacity: mouseArea.containsMouse ? Geometry.panel.hoveredItemOpacity : 1
        }
    }

    MouseArea {
        id: mouseArea
        enabled: root.enabled
        hoverEnabled: enabled
        anchors.fill: parent

        onClicked: function (mouse) {
            root.clicked(mouse)
        }
    }
}
