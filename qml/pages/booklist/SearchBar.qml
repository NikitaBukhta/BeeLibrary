import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root

    signal searchTextChanged(string text)

    TextField {
        id: searchField
        Layout.fillWidth: true
        placeholderText: qsTr("Search by title, author, or ISBN...")

        onTextChanged: debounceTimer.restart()

        onEditingFinished: {
            debounceTimer.stop()
            root.searchTextChanged(text)
        }
    }

    Timer {
        id: debounceTimer
        interval: Geometry.debounceMs
        onTriggered: root.searchTextChanged(searchField.text)
    }
}
