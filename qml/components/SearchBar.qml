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
    }

    Timer {
        id: debounceTimer
        interval: Geometry.searchBar.debounceMs
        onTriggered: root.searchTextChanged(searchField.text)
    }
}
