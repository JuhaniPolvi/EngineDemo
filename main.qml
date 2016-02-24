import QtQuick 2.3
import QtQuick.Window 2.2
import Dashboard 1.0
import Carcontrols 1.0

Window {
    id: windowBase
    visible: true
    width: 1000
    height: 500

    Connections
    {
        target: engine
        onNewRews:
        {
            rewsMeterContainer.currentRews = value
        }
        onNewSpeed:
        {
            speedMeterContainer.currentSpeed = speedvalue
        }
        onUnsafeGearSwitch:
        {

        }
        onGearSwitched:
        {
            gearBoxContainer.selectedGear = gearindex
        }
    }

    // Rews meter. Uses bitmap image for meter base and separate inlaid image rotated at suitable angle for needle
    RewsMeter
    {
        objectName: "rewsMeter"
        id: rewsMeterContainer
        anchors.left: parent.left
        anchors.top: parent.top
        width: 300
        height: 300
        property real currentRews: 0
        property real maximumRews: 9000

        // Image for rews meter base
        Image
        {
            objectName: "rewsMeterImage"
            id: rewsMeterImage
            source: "images/rewsmeter.png"
            anchors.fill: parent

            // Inset image for rews meter needle
            Image {
                width: parent.width
                height: parent.height
                anchors.centerIn: parent.Center
                property real startangle: 152
                property real fullangle: 246
                id: rewsNeedle
                source: "images/meterneedle.png"
                rotation: this.startangle + ((rewsMeterContainer.currentRews * this.fullangle)/rewsMeterContainer.maximumRews)

                //Behavior on rotation {SmoothedAnimation {velocity: 800 } }
            }
        }
    }

    Accelerator
    {
        objectName: "accelerator"
        id: acceleratorContainer
        anchors.right: gearBoxContainer.left
        anchors.top: parent.top
        width: 80
        height: 200
        property real acceleration: 0

        // Gas pedal. Uses bitmap image
        Image
        {
            id: gasPedalImage
            source: "images/pedal.png"
            anchors.fill: parent
            property real tmpvalue: 0

            MouseArea
            {
                anchors.fill: parent
                id: gasArea
                onPressed:
                {
                    parent.tmpvalue = ((mouse.y - gasPedalImage.y)*110)/gasPedalImage.height;
                    if (parent.tmpvalue < 0)
                        parent.tmpvalue = 0;
                    else if (parent.tmpvalue > 100)
                        parent.tmpvalue = 100;
                    acceleratorContainer.acceleration = parent.tmpvalue
                    engine.accelerate(acceleratorContainer.acceleration)
                }
            }
        }

        // Label for gas pedal. Shows text marking it as gas pedal and current gas level in percents
        Text
        {
            anchors.top: gasPedalImage.bottom
            anchors.horizontalCenter: gasPedalImage.horizontalCenter
            id: gasLabel
            text: "Gas level\n" + acceleratorContainer.acceleration + " %"

            // Added mouse area to allow releasing gas easily
            MouseArea
            {
                anchors.fill: parent
                id: resetGas
                onClicked:
                {
                    acceleratorContainer.acceleration = 0
                    engine.accelerate(acceleratorContainer.acceleration)
                }
            }
        }
    }

    Brake
    {
        objectName: "brake"
        id: brakeContainer
        anchors.left: gearBoxContainer.right
        anchors.top: parent.top
        width: 80
        height: 200
        property real brakinglevel: 0

        // Brake pedal. Uses bitmap image
        Image
        {
            objectName: "brakePedalImage"
            id: brakePedalImage
            source: "images/pedal.png"
            anchors.fill: parent
            property real tmpvalue: 0

            MouseArea
            {
                anchors.fill: parent
                id: brakeArea
                onPressed:
                {
                    parent.tmpvalue = ((mouse.y - brakePedalImage.y)*110)/brakePedalImage.height;
                    if (parent.tmpvalue < 0)
                        parent.tmpvalue = 0;
                    else if (parent.tmpvalue > 100)
                        parent.tmpvalue = 100;
                    brakeContainer.brakinglevel = parent.tmpvalue
                    engine.brake(brakeContainer.brakinglevel)
                }
            }
        }

        // Label for brake pedal. Shows text marking it as brake pedal and current braking power in percents
        Text
        {
            anchors.top: brakePedalImage.bottom
            anchors.horizontalCenter: brakePedalImage.horizontalCenter
            id: brakeLabel
            text: "Brake level\n" + brakeContainer.brakinglevel + " %"

            // Added mouse area to allow releasing brake easily
            MouseArea
            {
                anchors.fill: parent
                id: resetBrake
                onClicked:
                {
                    brakeContainer.brakinglevel = 0
                    engine.brake(brakeContainer.brakinglevel)
                }
            }
        }
    }

    Gearbox
    {
        objectName: "gearbox"
        id: gearBoxContainer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: 150
        height: 400
        property int selectedGear: 1

        signal gearClicked(int selection)

        // Gearbox (modelled after automatic for simplicity). Uses bitmap image for gearbox base and inset image for knob
        Image
        {
            objectName: "gearBoxImage"
            anchors.fill: parent
            id: gearBoxBaseImage
            source: "images/gearboxbase.png"
            property int selectedgear: 1

            // Mouse area added to allow switching gears by clicking
            MouseArea
            {
                anchors.fill: parent
                anchors.topMargin: 40
                anchors.bottomMargin: 40
                onClicked:
                {
                    engine.switchToGear(mouse.y/40)
                    //gearBoxBaseImage.selectedgear = (mouse.y/40);
                    //gearBoxContainer.gearClicked(gearBoxBaseImage.selectedgear)
                }
            }

            Image
            {
                anchors.horizontalCenter: parent.horizontalCenter
                id: gearboxKnob
                source: "images/gearboxknob.png"
                width: 60
                height: 60
                y: ((gearBoxContainer.selectedGear+1)*40)
            }
        }
    }

    // Speed meter. Uses bitmap image for meter base and inlaid image for needle
    SpeedMeter
    {
        objectName: "speedMeter"
        id: speedMeterContainer
        property real maximumSpeed: 270
        property real currentSpeed: 0
        anchors.right: parent.right
        anchors.top: parent.top
        width: 300
        height: 300

        // Image used for speed meter base
        Image
        {
            objectName: "speedMeterImage"
            id: speedMeterImage
            source: "images/speedmeter.png"
            anchors.fill: parent

            // Image used for speed meter needle
            Image {
                width: parent.width
                height: parent.height
                anchors.centerIn: parent.Center
                id: speedNeedle
                property real startangle: 152
                property real fullangle: 246
                source: "images/meterneedle.png"
                rotation: this.startangle + ((speedMeterContainer.currentSpeed * this.fullangle)/speedMeterContainer.maximumSpeed)
            }
        }
    }

//    MouseArea {
//        anchors.fill: parent
//        onClicked: {
//            Qt.quit();
//        }
//    }
}

