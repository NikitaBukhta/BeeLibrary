pragma Singleton

import QtQuick

QtObject {
    id: root

    readonly property int sidePadding: 16
    readonly property int topPadding: 16
    readonly property int elementSpacing: 12
    readonly property int commonSpacing: 8

    readonly property QtObject fontSize: QtObject {
        readonly property real fontScale: Qt.application.font.pixelSize / 14
        readonly property int displayLarge: 57 * fontScale
        readonly property int headlineLarge: 32 * fontScale
        readonly property int titleLarge: 24 * fontScale
        readonly property int bodyLarge: 16 * fontScale
        readonly property int bodyMedium: 14 * fontScale
        readonly property int labelLarge: 14 * fontScale
        readonly property int labelSmall: 12 * fontScale
    }

    readonly property QtObject icon: QtObject {
        readonly property QtObject small: QtObject {
            readonly property int width: 16
            readonly property int height: 16
        }

        readonly property QtObject common: QtObject {
            readonly property int width: 20
            readonly property int height: 20
        }

        readonly property QtObject medium: QtObject {
            readonly property int width: 36
            readonly property int height: 36
        }

        readonly property QtObject large: QtObject {
            readonly property int width: 40
            readonly property int height: 40
        }
    }

    readonly property QtObject panel: QtObject {
        readonly property int paddings: 16
        readonly property int radius: 12
        readonly property int spacing: root.commonSpacing
        readonly property int fontSize: root.fontSize.bodyLarge
        readonly property int secondaryFontSize: root.fontSize.labelSmall
        readonly property real hoveredItemOpacity: 0.8
    }

    readonly property QtObject window: QtObject {
        readonly property int defaultWidth: 900
        readonly property int defaultHeight: 640
        readonly property int minimumWidth: 500
        readonly property int minimumHeight: 400
    }

    readonly property QtObject page: QtObject {
        readonly property int margins: root.sidePadding
        readonly property int spacing: root.elementSpacing
    }

    readonly property QtObject listView: QtObject {
        readonly property int spacing: 1
        readonly property int emptyFontSize: root.fontSize.bodyLarge
        readonly property real emptyOpacity: 0.5
    }

    readonly property QtObject bookCard: QtObject {
        readonly property int padding: 12
        readonly property int spacing: 12
        readonly property int innerSpacing: 4
        readonly property int metaSpacing: 16
        readonly property int titleFontSize: root.fontSize.bodyLarge
        readonly property int authorFontSize: root.fontSize.bodyMedium
        readonly property int metaFontSize: root.fontSize.labelSmall
        readonly property real authorOpacity: 0.7
        readonly property real metaOpacity: 0.5
    }

    readonly property QtObject form: QtObject {
        readonly property int columnSpacing: 12
        readonly property int rowSpacing: 12
        readonly property int maxWidth: 500
        readonly property int fieldImplicitWidth: 200
        readonly property int fieldImplicitHeight: 40
        readonly property int fieldRadius: 2
        readonly property int fieldBorderWidth: 1
        readonly property int fieldErrorBorderWidth: 2
        readonly property int yearMin: 1
        readonly property int yearMax: new Date().getFullYear()
    }

    readonly property QtObject dialog: QtObject {
        readonly property int buttonRightMargin: 8
    }

    readonly property QtObject errorBanner: QtObject {
        readonly property int padding: 8
    }

    readonly property QtObject searchBar: QtObject {
        readonly property int debounceMs: 300
    }
}
