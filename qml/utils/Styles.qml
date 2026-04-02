pragma Singleton

import QtQuick

QtObject {
    id: root

    readonly property color background: "#fbfaf9"
    readonly property color transparent: "transparent"
    readonly property color basePanelColor: "#ffffff"
    readonly property color baseTextColor: "#141826"
    readonly property color secondaryTextColor: "#737b8c"
    readonly property color activeTextColor: "#40988d"
    readonly property color positiveColor: "#29a35c"
    readonly property color negativeColor: "#d74242"
    readonly property color errorColor: "#d32f2f"

    readonly property QtObject panel: QtObject {
        readonly property color titleColor: root.baseTextColor
        readonly property color secondaryColor: root.secondaryTextColor
    }

    readonly property QtObject errorBanner: QtObject {
        readonly property color background: root.errorColor
        readonly property color text: "#ffffff"
        readonly property int radius: 4
    }

    readonly property QtObject bookCard: QtObject {
        readonly property color titleColor: root.baseTextColor
        readonly property color authorColor: root.secondaryTextColor
        readonly property color metaColor: root.secondaryTextColor
    }
}
