import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root

    readonly property var sortOptions: [
        { text: qsTr("Title"),  role: BookListModel.TitleRole },
        { text: qsTr("Author"), role: BookListModel.AuthorRole },
        { text: qsTr("Year"),   role: BookListModel.YearRole }
    ]

    signal sortFieldChanged(int role)
    signal sortDescendingChanged(bool descending)

    Label {
        text: qsTr("Sort by:")
    }

    ComboBox {
        id: sortCombo
        model: root.sortOptions.map(opt => opt.text)
        onCurrentIndexChanged: root.sortFieldChanged(
            root.sortOptions[currentIndex].role)
    }

    Button {
        id: orderButton
        flat: true
        text: bookProxyModel.sortDescending ? "▼" : "▲"
        onClicked: root.sortDescendingChanged(!bookProxyModel.sortDescending)
    }
}
