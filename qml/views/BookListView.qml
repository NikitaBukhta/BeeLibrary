import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

ListView {
    id: root

    required property string bookIdRole
    required property string titleRole
    required property string authorRole
    required property string yearRole
    required property string isbnRole

    signal editRequested(int bookId, string title, string author, int year, string isbn)
    signal deleteRequested(int bookId, string title)

    clip: true
    spacing: Geometry.listView.spacing

    delegate: BookCard {
        required property var model
        required property int index

        bookId: model[root.bookIdRole]
        title: model[root.titleRole]
        author: model[root.authorRole]
        year: model[root.yearRole]
        isbn: model[root.isbnRole]

        onEditClicked: root.editRequested(bookId, title, author, year, isbn)
        onDeleteClicked: root.deleteRequested(bookId, title)
    }

    Label {
        anchors.centerIn: parent
        visible: root.count === 0
        text: qsTr("No books found. Click \"+ Add Book\" to get started.")
        font.pixelSize: Geometry.listView.emptyFontSize
        opacity: Geometry.listView.emptyOpacity
    }
}
