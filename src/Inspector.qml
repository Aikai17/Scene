import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D
import QtQuick.Dialogs

Rectangle {
    required property var sceneManager
    required property var saveDialog
    required property var loadDialog
    id: controlPanel
    color: "#f0f0f0"

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        Button {
            id: addButton
            text: "Add Primitives"
            onClicked: addPopup.open()
        }
        Button {
            text: "Clear Scene"
            onClicked: sceneManager.clearScene()
        }
        Button {
            text: "Save Scene"

            onClicked:{
                saveDialog.open()

            }
        }

        Button {
            text: "Load Scene"
            onClicked: {
                loadDialog.open()
            }
        }
        ListView {
            id: primitiveList
            width: parent.width
            height: parent.height - 200
            model: sceneManager
            spacing: 2

            delegate: Rectangle {
                width: primitiveList.width
                height: 60
                color: model.selected ? "lightblue" : "white"
                border.color: "gray"

                Column {
                    anchors.fill: parent
                    anchors.margins: 5

                    Text {
                        text: "Type: " + model.type
                        font.bold: true
                    }
                    Text { text: "Position: " + model.position.x.toFixed(1) +
                        ", " + model.position.y.toFixed(1) +
                        ", " + model.position.z.toFixed(1) }
                    Text { text: "Size: " + model.size.x.toFixed(1) +
                        " x " + model.size.y.toFixed(1) +
                        " x " + model.size.z.toFixed(1) }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: sceneManager.selectPrimitive(index)
                }
            }
        }
    }

    Popup {
        id: addPopup
        modal: true
        focus: true
        x: addButton.mapToItem(null, 0, addButton.height).x + addButton.width
        y: addButton.mapToItem(null, 0, addButton.height).y
        width: 240
        padding: 10
        background: Rectangle {
            color: "white"
            border.color: "#999"
            radius: 6
        }

        Column {
            spacing: 8
            width: parent.width

            ComboBox {
                id: typeCombo
                width: parent.width
                model: ["Box", "Sphere", "Pyramid"]
            }

            SpinBox {
                id: countSpin
                width: parent.width
                from: 1
                to: 100
                value: 5
            }

            Row {
                spacing: 4
                SpinBox { id: widthSpin; from: 10; to: 500; value: 100; width: 70 }
                SpinBox { id: heightSpin; from: 10; to: 500; value: 100; width: 70 }
                SpinBox { id: depthSpin; from: 10; to: 500; value: 100; width: 70 }
            }

            Row {
                spacing: 8
                Button {
                    text: "Add"
                    onClicked: {
                        sceneManager.addPrimitives(
                            typeCombo.currentText,
                            countSpin.value,
                            widthSpin.value,
                            heightSpin.value,
                            depthSpin.value
                        )
                        addPopup.close()
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: addPopup.close()
                }
            }
        }
    }
}
