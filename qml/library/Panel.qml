// Panel.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    implicitHeight: mainLayout.implicitHeight + Geometry.panel.paddings * 2

    property alias title: titleText.text
    property alias description: secondaryText.text
    property alias spacing: mainLayout.spacing
    property alias contentItem: contentLoader.sourceComponent
    property alias titleRightContent: titleRightContentLoader.sourceComponent

    Rectangle {
        anchors.fill: parent
        radius: Geometry.panel.radius
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: Geometry.panel.paddings
        spacing: Geometry.panel.spacing

        // ---------- HEADER ----------
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                Layout.fillWidth: true

                Text {
                    id: titleText
                    Layout.fillWidth: true
                    font.pixelSize: Geometry.panel.fontSize
                    font.bold: true
                    elide: Text.ElideRight
                }

                Text {
                    id: secondaryText
                    Layout.fillWidth: true
                    visible: text.length > 0
                    color: Styles.panel.secondaryColor
                    font.pixelSize: Geometry.panel.secondaryFontSize
                    elide: Text.ElideRight
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Loader {
                id: titleRightContentLoader
                Layout.alignment: Qt.AlignVCenter
            }
        }

        // ---------- CONTENT ----------
        Loader {
            id: contentLoader
            Layout.fillWidth: true
        }
    }
}
