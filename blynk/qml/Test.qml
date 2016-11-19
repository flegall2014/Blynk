import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    id: mainWindow
    width: 512
    height: 256
    visible: true

    Column {
        anchors.fill: parent
        spacing: 8
        Slider {
            id: redSlider
            minimumValue: 0
            maximumValue: 255
            value: 255
            onValueChanged: _controller.setRed(value)
        }
        Slider {
            id: greenSlider
            minimumValue: 0
            maximumValue: 255
            value: 255
            onValueChanged: _controller.setGreen(value)
        }
        Slider {
            id: blueSlider
            minimumValue: 0
            maximumValue: 255
            value: 0
            onValueChanged: _controller.setBlue(value)
        }
        Slider {
            id: alphaSlider
            minimumValue: 0
            maximumValue: 255
            value: 128
            onValueChanged: _controller.setAlpha(value)
        }
        Slider {
            id: temperatureSlider
            minimumValue: 1000
            maximumValue: 40000
            value: 1000
            onValueChanged: _controller.setTemperature(value)
        }
    }
}
