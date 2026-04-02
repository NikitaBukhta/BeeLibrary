import QtQuick
import QtQuick.Controls

Label {
    id: root

    required property string message

    visible: message !== ""
    text: message
    padding: Geometry.spacingMedium
    color: Styles.errorTextColor

    background: Rectangle {
        color: Styles.errorColor
        radius: Geometry.radiusSmall
    }
}
