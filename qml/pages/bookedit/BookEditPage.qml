import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PageTemplate {
    id: root

    readonly property bool editMode: bookFormController.editMode

    property string validationMessage: ""

    function goBack() {
        navigationController.goBack()
    }

    function tryAccept() {
        validationMessage = ""

        let ok = bookFormController.submitForm(
            bookForm.titleText, bookForm.authorText,
            bookForm.yearText, bookForm.isbnText)

        if (ok)
            root.goBack()
        else
            validationMessage = bookFormController.errorMessage
    }

    Flickable {
        anchors.fill: parent
        anchors.margins: Geometry.spacingXL
        contentHeight: formColumn.implicitHeight
        contentWidth: width
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        ColumnLayout {
            id: formColumn
            width: parent.width
            spacing: Geometry.spacingLarge

            Component.onCompleted: {
                if (root.editMode)
                    bookForm.loadBook(bookFormController.currentBookData)

                bookForm.focusTitle()
            }

            RowLayout {
                id: header
                Layout.fillWidth: true

                Button {
                    text: qsTr("< Back")
                    flat: true
                    onClicked: root.goBack()
                }

                Label {
                    text: root.editMode ? qsTr("Edit Book") : qsTr("Add Book")
                    font.pixelSize: Geometry.fontSize.titleLarge
                    font.bold: true
                }

                Item { Layout.fillWidth: true }
            }

            BookForm {
                id: bookForm
                Layout.fillWidth: true
                Layout.maximumWidth: Geometry.formMaxWidth
            }

            Label {
                id: validationLabel
                color: Styles.errorColor
                text: root.validationMessage
                visible: root.validationMessage.length > 0
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }

            RowLayout {
                id: actions
                spacing: Geometry.spacingMedium

                Button {
                    text: root.editMode ? qsTr("Save") : qsTr("Add")
                    highlighted: true
                    enabled: bookFormController.isFormValid(
                        bookForm.titleText, bookForm.authorText)
                    onClicked: root.tryAccept()
                }

                Button {
                    text: qsTr("Cancel")
                    flat: true
                    onClicked: root.goBack()
                }
            }
        }
    }
}
