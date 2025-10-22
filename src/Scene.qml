import QtQuick
import QtQuick3D
import QtQuick.Controls
// 3D Вьювер
View3D {
    id: view3D
    required property var sceneManager

    environment: SceneEnvironment {
        clearColor: "lightgray"
        backgroundMode: SceneEnvironment.Color
    }

    Text{
        text: ""+sceneNode.position
        color:"black"
        font.pixelSize: 14
    }

    camera: cameraNode
    Node{
        id: originNode
        PerspectiveCamera {
            id: cameraNode
            position: Qt.vector3d(0, 0, 600)
        }
        DirectionalLight{ }
    }

    Node {
        id: sceneNode
        Repeater3D {
            id: primitiveRepeater
            model: sceneManager

            delegate: Model {
                position: model.position
                scale: Qt.vector3d(model.size.x / 100, model.size.y / 100, model.size.z / 100)

                source: {
                    switch(model.type) {
                    case "Box": return "#Cube"
                    case "Sphere": return "#Sphere"
                    case "Pyramid": return "#Cone"
                    default: return "#Cube"
                    }
                }

                materials: [ DefaultMaterial {
                    diffuseColor: model.selected ? "yellow" : model.color
                } ]
            }
        }
    }
    OrbitController {
        anchors.fill: parent
        id: orbitController
        origin: originNode
        camera: cameraNode
        invertX: true
        invertY: true
    }
}
