import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root

    property int bookId: -1
    property string bookTitle: ""

    title: qsTr("Delete Book")
    modal: true
    parent: Overlay.overlay
    anchors.centerIn: parent

    Label {
        text: qsTr("Are you sure you want to delete \"%1\"?").arg(root.bookTitle)
        wrapMode: Text.Wrap
    }

    footer: RowLayout {
        Item { Layout.fillWidth: true }

        Button {
            text: qsTr("Cancel")
            flat: true
            onClicked: root.reject()
        }

        Button {
            Layout.rightMargin: Geometry.spacingMedium
            text: qsTr("Delete")
            highlighted: true
            onClicked: root.accept()
        }
    }
}
