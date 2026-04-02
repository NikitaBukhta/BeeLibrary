import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    property alias text: field.text
    property alias placeholderText: field.placeholderText
    property var validator: null
    property string label
    property string prefix: ""

    spacing: 0
    Layout.fillWidth: true

    function forceActiveFocus() {
        field.forceActiveFocus()
    }

    Label {
        text: root.label
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: Geometry.spacingSmall

        Label {
            text: root.prefix
            font: field.font
            visible: root.prefix.length > 0
        }

        TextField {
            id: field
            Layout.fillWidth: true
            validator: root.validator
        }
    }
}
