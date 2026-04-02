import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PageTemplate {
    id: root

    function openEditPage(bookId) {
        ApplicationWindow.window.editBookId = bookId
        contextModel.currentPage = ContextModel.BOOK_EDIT_PAGE
    }

    bodyItem: Component {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Geometry.page.margins
            spacing: Geometry.page.spacing

            // ---------- HEADER ----------
            RowLayout {
                Layout.fillWidth: true

                Label {
                    text: qsTr("My Library")
                    font.pixelSize: Geometry.fontSize.titleLarge
                    font.bold: true
                }

                Item { Layout.fillWidth: true }

                Button {
                    text: qsTr("+ Add Book")
                    onClicked: root.openEditPage(-1)
                }
            }

            // ---------- SEARCH ----------
            SearchBar {
                Layout.fillWidth: true
                onSearchTextChanged: (text) => bookProxyModel.searchQuery = text
            }

            // ---------- ERROR BANNER ----------
            Label {
                id: errorLabel
                Layout.fillWidth: true
                visible: bookListModel.errorMessage !== ""
                text: bookListModel.errorMessage
                padding: Geometry.errorBanner.padding
                color: Styles.errorBanner.text
                background: Rectangle {
                    color: Styles.errorBanner.background
                    radius: Styles.errorBanner.radius
                }
            }

            // ---------- BOOK LIST ----------
            BookListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: bookProxyModel
                bookIdRole: "bookId"
                titleRole: "title"
                authorRole: "author"
                yearRole: "year"
                isbnRole: "isbn"

                onEditRequested: (id, title, author, year, isbn) => root.openEditPage(id)

                onDeleteRequested: (id, title) => {
                    deleteDialog.bookId = id
                    deleteDialog.bookTitle = title
                    deleteDialog.open()
                }
            }
        }
    }

    // ---------- DELETE CONFIRMATION ----------
    Dialog {
        id: deleteDialog
        property int bookId: -1
        property string bookTitle: ""

        title: qsTr("Delete Book")
        modal: true
        parent: Overlay.overlay
        anchors.centerIn: parent

        Label {
            text: qsTr("Are you sure you want to delete \"%1\"?").arg(deleteDialog.bookTitle)
            wrapMode: Text.Wrap
        }

        footer: RowLayout {
            Item { Layout.fillWidth: true }
            Button {
                text: qsTr("Cancel")
                flat: true
                onClicked: deleteDialog.reject()
            }
            Button {
                Layout.rightMargin: Geometry.dialog.buttonRightMargin
                text: qsTr("Delete")
                highlighted: true
                onClicked: {
                    bookListModel.deleteBook(deleteDialog.bookId)
                    deleteDialog.close()
                }
            }
        }
    }
}
