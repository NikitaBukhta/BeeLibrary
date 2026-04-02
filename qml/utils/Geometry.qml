pragma Singleton

import QtQuick

QtObject {
    id: root

    readonly property int spacingThin: 1
    readonly property int spacingSmall: 4
    readonly property int spacingMedium: 8
    readonly property int spacingLarge: 12
    readonly property int spacingXL: 16

    readonly property int radiusSmall: 4
    readonly property int radiusMedium: 12

    readonly property int formMaxWidth: 500
    readonly property int debounceMs: 300

    readonly property int baselineFontSize: 14

    readonly property QtObject fontSize: QtObject {
        readonly property real fontScale: Qt.application.font.pixelSize / root.baselineFontSize
        readonly property int displayLarge: 57 * fontScale
        readonly property int headlineLarge: 32 * fontScale
        readonly property int titleLarge: 24 * fontScale
        readonly property int bodyLarge: 16 * fontScale
        readonly property int bodyMedium: 14 * fontScale
        readonly property int labelLarge: 14 * fontScale
        readonly property int labelSmall: 12 * fontScale
    }

    readonly property QtObject window: QtObject {
        readonly property int defaultWidth: 900
        readonly property int defaultHeight: 640
        readonly property int minimumWidth: 500
        readonly property int minimumHeight: 400
    }
}
