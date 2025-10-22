import QtQuick
import QtQuick3D

Item {
    id: orbitController
    anchors.fill: parent

    required property var origin
    required property var camera

    property bool invertX: false
    property bool invertY: false

    property real minPitch: -85
    property real maxPitch: 85

    property real yaw: 0
    property real pitch: 0

    property real sensitivity: 0.5
    property real zoomSpeed: 10
    property real panSpeed: 0.5

    property real distance: 600    // Расстояние от камеры до центра

    property real lastX: 0
    property real lastY: 0
    property bool dragging: false
    property bool panning: false

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.MiddleButton
        hoverEnabled: true

        onPressed: (mouse) => {
            orbitController.lastX = mouse.x;
            orbitController.lastY = mouse.y;

            if (mouse.button === Qt.LeftButton)
                orbitController.dragging = true;
            else if (mouse.button === Qt.MiddleButton)
                orbitController.panning = true;
        }

        onReleased: (mouse) => {
            if (mouse.button === Qt.LeftButton)
                orbitController.dragging = false;
            else if (mouse.button === Qt.MiddleButton)
                orbitController.panning = false;
        }

        onPositionChanged: (mouse) => {
           const dx = mouse.x - orbitController.lastX;
           const dy = mouse.y - orbitController.lastY;

           if (orbitController.dragging) {
               let effectiveDx = orbitController.invertX ? -dx : dx;
               let effectiveDy = orbitController.invertY ? -dy : dy;

               orbitController.yaw += effectiveDx * orbitController.sensitivity;
               orbitController.pitch += effectiveDy * orbitController.sensitivity;

               if (orbitController.pitch > orbitController.maxPitch)
                   orbitController.pitch = orbitController.maxPitch;
               if (orbitController.pitch < orbitController.minPitch)
                   orbitController.pitch = orbitController.minPitch;
           }


            if (orbitController.panning) {
                // Панорамируем origin (двигаем его в пространстве)
                const panX = -dx * orbitController.panSpeed;
                const panY = dy * orbitController.panSpeed;

                // Вычисляем направление смещения в мировых координатах
                let right = Qt.vector3d(
                    Math.cos(orbitController.yaw * Math.PI / 180),
                    0,
                    -Math.sin(orbitController.yaw * Math.PI / 180)
                );

                let up = Qt.vector3d(0, 1, 0);

               let panOffset = Qt.vector3d(
                   right.x * panX + up.x * panY,
                   right.y * panX + up.y * panY,
                   right.z * panX + up.z * panY
               );

               orbitController.origin.position = Qt.vector3d(
                   origin.position.x + panOffset.x,
                   origin.position.y + panOffset.y,
                   origin.position.z + panOffset.z
               );

            }

            orbitController.lastX = mouse.x;
            orbitController.lastY = mouse.y;

            orbitController.update();
        }

        onWheel: (wheel) => {
            orbitController.distance -= wheel.angleDelta.y * orbitController.zoomSpeed / 120;
            if (orbitController.distance < 50) orbitController.distance = 50;
            if (orbitController.distance > 3000) orbitController.distance = 3000;
            orbitController.update();
        }
    }

    function update() {
        // Вращаем origin
        origin.eulerRotation = Qt.vector3d(pitch, yaw, 0);

        // Камера остаётся на фиксированном расстоянии по Z от центра
        camera.position = Qt.vector3d(0, 0, distance);
    }
}
