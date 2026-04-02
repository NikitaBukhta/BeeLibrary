import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    id: root

    required property int bookId
    required property string title
    required property string author
    required property int year
    required property string isbn

    signal editClicked()
    signal deleteClicked()

    width: ListView.view ? ListView.view.width : implicitWidth
    padding: Geometry.spacingLarge

    contentItem: RowLayout {
        spacing: Geometry.spacingLarge

        ColumnLayout {
            Layout.fillWidth: true
            spacing: Geometry.spacingSmall

            Label {
                text: root.title
                font.bold: true
                font.pixelSize: Geometry.fontSize.bodyLarge
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: root.author
                font.pixelSize: Geometry.fontSize.bodyMedium
                opacity: Styles.secondaryOpacity
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            RowLayout {
                spacing: Geometry.spacingXL

                component MetaLabel: Label {
                    font.pixelSize: Geometry.fontSize.labelSmall
                    opacity: Styles.mutedOpacity
                }

                MetaLabel {
                    text: root.year > 0 ? root.year.toString() : ""
                    visible: root.year > 0
                }

                MetaLabel {
                    text: root.isbn
                    visible: root.isbn.length > 0
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
