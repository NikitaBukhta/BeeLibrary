import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root

    width: Geometry.window.defaultWidth
    height: Geometry.window.defaultHeight
    minimumWidth: Geometry.window.minimumWidth
    minimumHeight: Geometry.window.minimumHeight
    visible: true
    title: qsTr("BeeLibrary")

    Loader {
        id: pageLoader
        anchors.fill: parent
        source: contextModel.currentPagePath
    }
}
