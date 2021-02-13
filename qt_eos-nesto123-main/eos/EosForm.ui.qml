import QtQuick 2.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.15

Item {
    id: item1
    width: 500
    height: 400
    property alias calculate: calculate
    property alias reset: reset
    property alias v3result: v3result
    property alias v2result: v2result
    property alias v3: v3
    property alias v2: v2
    property alias fieldb: fieldb
    property alias fielda: fielda
    property alias fieldP: fieldP
    property alias v1result: v1result
    property alias v1: v1
    property alias fieldT: fieldT

    Label {
        id: label
        x: 32
        y: 11
        text: "Redlich Kwongova jednadžba stanja"
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 16
        textFormat: Text.AutoText
        anchors.horizontalCenter: parent.horizontalCenter



    }

    Button {
        id: calculate
        y: 274
        width: 92
        height: 35
        text: qsTr("Calculate")
        anchors.left: reset.right
        anchors.leftMargin: 10
    }

    Label {
        id: label1
        x: 32
        width: 26
        height: 18
        text: qsTr("T=")
        anchors.right: fieldT.left
        anchors.top: parent.top
        anchors.topMargin: 95
        anchors.rightMargin: 6
    }

    TextField {
        id: fieldT
        x: 50
        y: 87
        width: 151
        height: 29
        anchors.horizontalCenterOffset: -110
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: qsTr("")
    }

    Label {
        id: v1
        x: 212
        y: 95
        text: qsTr("v1=")
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Label {
        id: v1result
        width: 183
        height: 13
        text: qsTr("")
        anchors.left: v1.right
        anchors.top: parent.top
        anchors.topMargin: 95
        anchors.leftMargin: 20
    }

    Label {
        id: label3
        x: 32
        width: 26
        height: 18
        text: qsTr("P=")
        anchors.right: fieldP.left
        anchors.top: parent.top
        anchors.topMargin: 132
        anchors.rightMargin: 6
    }

    TextField {
        id: fieldP
        x: 57
        width: 151
        height: 29
        anchors.top: fieldT.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -110
        placeholderText: qsTr("")
    }

    TextField {
        id: fielda
        x: 57
        width: 151
        height: 29
        anchors.top: fieldP.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -110
        anchors.topMargin: 10
        placeholderText: qsTr("")
    }

    Label {
        id: label4
        x: 32
        width: 26
        height: 18
        text: qsTr("a=")
        anchors.right: fielda.left
        anchors.top: parent.top
        anchors.topMargin: 173
        anchors.rightMargin: 6
    }

    TextField {
        id: fieldb
        x: 57
        width: 151
        height: 29
        anchors.top: fielda.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        anchors.horizontalCenterOffset: -110
        placeholderText: qsTr("")
    }

    Label {
        id: label5
        x: 32
        width: 26
        height: 18
        text: qsTr("b=")
        anchors.right: fieldb.left
        anchors.top: parent.top
        anchors.topMargin: 210
        anchors.rightMargin: 6
    }

    Label {
        id: v2
        x: 214
        y: 134
        text: qsTr("v2=")
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Label {
        id: v3
        x: 218
        y: 173
        opacity: 1
        text: qsTr("v3=")
        anchors.horizontalCenterOffset: 1
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Label {
        id: v2result
        width: 183
        height: 13
        text: qsTr("")
        anchors.left: v2.right
        anchors.top: parent.top
        anchors.topMargin: 134
        anchors.leftMargin: 20
    }

    Label {
        id: v3result
        width: 183
        height: 13
        text: qsTr("")
        anchors.left: v3.right
        anchors.top: parent.top
        anchors.topMargin: 173
        anchors.leftMargin: 19
    }

    Button {
        id: reset
        x: 260
        y: 274
        width: 92
        height: 35
        text: qsTr("Reset")
        anchors.horizontalCenterOffset: 80
        anchors.horizontalCenter: parent.horizontalCenter
    }
    states: [
        State {
            name: "tri"

            PropertyChanges {
                target: v2
                opacity: 0
            }

            PropertyChanges {
                target: v3
                opacity: 0
            }

            PropertyChanges {
                target: v2result
                opacity: 0
            }

            PropertyChanges {
                target: v3result
                opacity: 0
            }
        }
    ]






}
