import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    visible: true
    width: 600
    height: 400
    flags: Qt.Window | Qt.FramelessWindowHint

    background: Rectangle {
        color: "#2c3e50"
        radius: 10
        border.color: "#34495e"
        border.width: 1
    }

    // Основной контейнер, который жестко фиксирует элементы по вертикали
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // --- ВЕРХ: Заголовок (Фиксированная высота) ---
        Rectangle {
            id: titleBar
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#34495e"
            // Скругление только сверху (через нижний прямоугольник-заплатку)
            Rectangle {
                width: parent.width; height: 10;
                anchors.bottom: parent.bottom; color: parent.color
            }

            Text {
                text: "Server Control Panel"
                anchors.centerIn: parent
                color: "white"
            }

            Button {
                text: "✕"
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                width: 30
                onClicked: Qt.quit()
            }
        }

        ListView {
            id: tableView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 5
            clip: true
            model: 20 // Пример данных
            delegate: ClientDataRow { // Убедитесь, что файл ServerRowDelegate.qml в той же папке
                width: tableView.width
            }
        }

        Rectangle {
            id: serverControllBar
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#34495e"

            RowLayout {
                anchors.centerIn: parent
                spacing: 20


                Button {
                    id: btnOn
                    text: "Start Server"
                    onClicked: view.startServer()
                }

                Button {
                    id: btnOff
                    text: "Stop Server"
                    onClicked: view.stopServer()
                }

                Text {
                    text: view ? view.serverStatus : "Offline"
                    color: view ? (view.isRunning ? "green" : "red") : "red"
                    font.pixelSize: 14
                }
            }
        }
    }
}
