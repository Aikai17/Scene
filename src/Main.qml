import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick.Dialogs
import SceneManager 1.0

ApplicationWindow {
    width: 1200
    height: 800
    visible: true

    SceneManager {
        id: sceneManager
    }


    SplitView {
        anchors.fill: parent
        Inspector{
            id: inspector
            sceneManager: sceneManager
            saveDialog: saveDialog
            loadDialog: loadDialog

            width: 300
            SplitView.minimumWidth: 250
        }

        Scene{
            id: scene
            sceneManager: sceneManager

            SplitView.fillWidth: true
        }
    }

    FileDialog {
        id: saveDialog
        title: "Save Scene"
        fileMode: FileDialog.SaveFile
        defaultSuffix: "json"

        onAccepted: {
            sceneManager.saveScene(selectedFile)
        }
    }

    FileDialog {
        id: loadDialog
        title: "Load Scene"
        fileMode: FileDialog.OpenFile

        onAccepted: {
            sceneManager.loadScene(selectedFile)
        }
    }
}
