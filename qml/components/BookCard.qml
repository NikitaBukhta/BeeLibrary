import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    id: root

    property int bookId: -1
    property string title
    property string author
    property int year: 0
    property string isbn

    signal editClicked()
    signal deleteClicked()

    width: ListView.view ? ListView.view.width : implicitWidth
    padding: Geometry.bookCard.padding

    contentItem: RowLayout {
        spacing: Geometry.bookCard.spacing

        ColumnLayout {
            Layout.fillWidth: true
            spacing: Geometry.bookCard.innerSpacing

            Label {
                text: root.title
                font.bold: true
                font.pixelSize: Geometry.bookCard.titleFontSize
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: root.author
                font.pixelSize: Geometry.bookCard.authorFontSize
                opacity: Geometry.bookCard.authorOpacity
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            RowLayout {
                spacing: Geometry.bookCard.metaSpacing

                Label {
                    text: root.year > 0 ? root.year.toString() : ""
                    visible: root.year > 0
                    font.pixelSize: Geometry.bookCard.metaFontSize
                    opacity: Geometry.bookCard.metaOpacity
                }

                Label {
                    text: root.isbn
                    visible: root.isbn.length > 0
                    font.pixelSize: Geometry.bookCard.metaFontSize
                    opacity: Geometry.bookCard.metaOpacity
                }
            }
        }

        Button {
            text: qsTr("Edit")
            flat: true
            onClicked: root.editClicked()
        }

        Button {
            text: qsTr("Delete")
            flat: true
            onClicked: root.deleteClicked()
        }
    }
}
