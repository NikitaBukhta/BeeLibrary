import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    property alias titleText: titleField.text
    property alias authorText: authorField.text
    property alias yearText: yearField.text
    property alias isbnText: isbnField.text

    spacing: Geometry.spacingLarge

    function focusTitle() {
        titleField.forceActiveFocus()
    }

    function loadBook(book) {
        titleField.text = book.title ?? ""
        authorField.text = book.author ?? ""
        yearField.text = (book.year ?? 0) > 0 ? book.year.toString() : ""
        isbnField.text = book.rawIsbn ?? ""
    }

    FormField {
        id: titleField
        label: qsTr("Title *")
        placeholderText: qsTr("Book title")
    }

    FormField {
        id: authorField
        label: qsTr("Author *")
        placeholderText: qsTr("Author name")
    }

    FormField {
        id: yearField
        label: qsTr("Year")
        placeholderText: qsTr("Publication year")
        validator: IntValidator {
            bottom: bookFormController.yearMin
            top: bookFormController.yearMax
        }
    }

    FormField {
        id: isbnField
        label: qsTr("ISBN")
        placeholderText: qsTr("1234")
        prefix: "ISBN-"
        validator: RegularExpressionValidator {
            regularExpression: /\d{0,13}/
        }
    }
}
