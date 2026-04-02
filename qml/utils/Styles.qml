pragma Singleton

import QtQuick

QtObject {
    id: root

    readonly property color background: "#fbfaf9"
    readonly property color errorColor: "#d32f2f"
    readonly property color errorTextColor: "#ffffff"

    readonly property real secondaryOpacity: 0.7
    readonly property real mutedOpacity: 0.5
}
