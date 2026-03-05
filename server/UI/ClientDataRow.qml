import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: rowRoot
    height: 45
    color: index % 2 === 0 ? "#3b4f63" : "#34495e"
    radius: 4

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15

        Text {
            text: "ID: " + (index + 1)
            color: "white"
            Layout.preferredWidth: 50
        }

        Text {
            text: "User_Session_" + index
            color: "#ecf0f1"
            Layout.fillWidth: true
        }

        Rectangle {
            width: 10
            height: 10
            radius: 5
            color: "#2ecc71" // Индикатор статуса
        }
    }
}
