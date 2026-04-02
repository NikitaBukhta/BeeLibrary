import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PageTemplate {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Geometry.spacingXL
        spacing: Geometry.spacingLarge

        RowLayout {
            id: header
            Layout.fillWidth: true

            Label {
                text: qsTr("My Library")
                font.pixelSize: Geometry.fontSize.titleLarge
                font.bold: true
            }

            Item { Layout.fillWidth: true }

            Button {
                text: qsTr("+ Add Book")
                onClicked: {
                    bookFormController.currentBookId = 0
                    navigationController.currentPage = NavigationController.BOOK_EDIT_PAGE
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: Geometry.spacingMedium

            SearchBar {
                Layout.fillWidth: true
                onSearchTextChanged: (text) => bookProxyModel.searchQuery = text
            }

            SortBar {
                onSortFieldChanged: (role) => bookProxyModel.sortField = role
                onSortDescendingChanged: (desc) => bookProxyModel.sortDescending = desc
            }
        }

        ErrorBanner {
            Layout.fillWidth: true
            message: bookListModel.errorMessage
        }

        BookListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: bookProxyModel

            onEditRequested: (bookId) => {
                bookFormController.currentBookId = bookId
                navigationController.currentPage = NavigationController.BOOK_EDIT_PAGE
            }

            onDeleteRequested: (bookId, title) => {
                deleteDialog.bookId = bookId
                deleteDialog.bookTitle = title
                deleteDialog.open()
            }
        }
    }

    DeleteBookDialog {
        id: deleteDialog
        onAccepted: bookListModel.deleteBook(deleteDialog.bookId)
    }
}
