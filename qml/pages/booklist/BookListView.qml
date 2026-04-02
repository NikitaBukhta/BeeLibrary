pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls

ListView {
    id: root

    signal editRequested(int bookId)
    signal deleteRequested(int bookId, string title)

    clip: true
    spacing: Geometry.spacingThin

    delegate: BookCard {
        required property var model
        required property int index

        bookId: model.bookId
        title: model.title
        author: model.author
        year: model.year
        isbn: model.isbn

        onEditClicked: root.editRequested(bookId)
        onDeleteClicked: root.deleteRequested(bookId, title)
    }

    Label {
        anchors.centerIn: parent
        visible: root.count === 0
        text: qsTr("No books found. Click \"+ Add Book\" to get started.")
        font.pixelSize: Geometry.fontSize.bodyLarge
        opacity: Styles.mutedOpacity
    }
}
