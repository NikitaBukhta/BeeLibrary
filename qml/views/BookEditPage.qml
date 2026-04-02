import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PageTemplate {
    id: root

    readonly property int bookId: ApplicationWindow.window.editBookId
    readonly property bool editMode: bookId >= 0

    property string currentTitle: ""
    property string currentAuthor: ""
    property string currentYear: ""
    property string currentIsbn: ""
    property string validationMessage: ""

    readonly property bool formValid: currentTitle.trim().length > 0
                                   && currentAuthor.trim().length > 0

    readonly property string isbnPrefix: "ISBN-"

    function isbnValue(): string {
        return currentIsbn.length > 0 ? isbnPrefix + currentIsbn : ""
    }

    function tryAccept() {
        validationMessage = ""

        let y = currentYear.length > 0 ? parseInt(currentYear) : 0
        let isbn = isbnValue()

        let ok = root.editMode
            ? bookListModel.updateBook(root.bookId, currentTitle.trim(),
                                       currentAuthor.trim(), y, isbn)
            : bookListModel.addBook(currentTitle.trim(), currentAuthor.trim(),
                                    y, isbn)

        if (ok)
            contextModel.openPreviousPage()
        else
            validationMessage = bookListModel.errorMessage
    }

    bodyItem: Component {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Geometry.page.margins
            spacing: Geometry.page.spacing

            Component.onCompleted: {
                if (root.editMode) {
                    let book = bookListModel.getBook(root.bookId)
                    titleField.text = book.title
                    authorField.text = book.author
                    yearField.text = book.year > 0 ? book.year.toString() : ""

                    let isbn = book.isbn ?? ""
                    if (isbn.startsWith(root.isbnPrefix))
                        isbnField.text = isbn.substring(root.isbnPrefix.length)
                    else if (isbn.length > 0)
                        isbnField.text = isbn
                }
            }

            // ---------- HEADER ----------
            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: qsTr("< Back")
                    flat: true
                    onClicked: contextModel.openPreviousPage()
                }

                Label {
                    text: root.editMode ? qsTr("Edit Book") : qsTr("Add Book")
                    font.pixelSize: Geometry.fontSize.titleLarge
                    font.bold: true
                }

                Item { Layout.fillWidth: true }
            }

            // ---------- FORM ----------
            GridLayout {
                columns: 2
                columnSpacing: Geometry.form.columnSpacing
                rowSpacing: Geometry.form.rowSpacing
                Layout.fillWidth: true
                Layout.maximumWidth: Geometry.form.maxWidth

                Label { text: qsTr("Title *") }
                TextField {
                    id: titleField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Book title")
                    onTextChanged: root.currentTitle = text
                }

                Label { text: qsTr("Author *") }
                TextField {
                    id: authorField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Author name")
                    onTextChanged: root.currentAuthor = text
                }

                Label { text: qsTr("Year") }
                TextField {
                    id: yearField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Publication year")
                    validator: IntValidator { bottom: Geometry.form.yearMin; top: Geometry.form.yearMax }
                    onTextChanged: root.currentYear = text
                }

                Label { text: qsTr("ISBN") }
                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: root.isbnPrefix
                        font: isbnField.font
                    }
                    TextField {
                        id: isbnField
                        Layout.fillWidth: true
                        placeholderText: qsTr("1234")
                        validator: RegularExpressionValidator { regularExpression: /\d*/ }
                        onTextChanged: root.currentIsbn = text
                    }
                }
            }

            // ---------- VALIDATION ----------
            Label {
                color: Styles.errorColor
                text: root.validationMessage
                visible: root.validationMessage.length > 0
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }

            // ---------- ACTIONS ----------
            RowLayout {
                spacing: Geometry.commonSpacing

                Button {
                    text: root.editMode ? qsTr("Save") : qsTr("Add")
                    highlighted: true
                    enabled: root.formValid
                    onClicked: root.tryAccept()
                }

                Button {
                    text: qsTr("Cancel")
                    flat: true
                    onClicked: contextModel.openPreviousPage()
                }
            }

            Item { Layout.fillHeight: true }
        }
    }
}
