import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property int menuState: 0


    MouseArea{
        anchors.fill: parent
        onClicked: {
            if(menuState == 0)
            {
                menuState = 1;
                myRec.opacity = 0;
            }
            else
            {
                menuState = 0;
                myRec.opacity = 1;
            }
        }
    }
    Rectangle{
        id: myRec
        width: parent.width
        height: parent.height /4
        Text{
         anchors.left: parent.left
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                 if(menuState == 1)
                 {
                     menuState = 0;
                     myRec.opacity = 1;
                 }
            }
        }

        Row{
            width: parent.width
            height: parent.height
            Button{

            }
            Button{

            }
            Button{

            }
        }
    }

}
