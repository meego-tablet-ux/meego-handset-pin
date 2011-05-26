/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


/*!
    \file SimPassword.qml
    \section SimPassword
    \brief Multiple char password input.

    \subsection Signals
    \li accepted()
    \li rejected()
    \li textEntered(string text)

    \subsection Objects
    - \b title : Title bar
      - Properties
        - text: "Enter code"
    - \b icon : Main icon.
      - Properties
        - source
    - \b editText : Multiple char input.
      - Properties
        - text
        - isNumeric: true
        - hideTyping: true
        - minChars: 4
        - maxChars: 4
    - \b okRect : "Ok" button.
      - Signals
        - accepted()
    - \b cancelRect : "Cancel" button.
      - Signals
        - rejected()
*/

import Qt 4.7
import "qrc:/SimUiCheck.js" as SimUiCheck

Rectangle {
    id: view
    objectName: "view"
    width: 480
    height: 320
    color: "#20400080"

    signal accepted()
    onAccepted: {
        console.log("Accepted");
        console.log("Text len: ", editText.text.length, ", bounds: [", editText.minChars, "..", editText.maxChars, "], numeric: ", editText.isNumeric);
        if (editText.minChars != -1 && editText.text.length < editText.minChars) {
            console.log( qsTr("You entered ") + editText.text.length + qsTr(" characters.") + "\n" +
            qsTr("Please enter at least ") + editText.minChars + qsTr(" characters."));
            return;
        }
        if (editText.maxChars != -1 && editText.text.length > editText.maxChars) {
            console.log( qsTr("You entered ") + editText.text.length + qsTr(" characters.") + "\n" +
            qsTr("Please enter no more than ") + editText.maxChars + qsTr(" characters."));
            return;
        }
        if (editText.isNumeric && !SimUiCheck.checkNumericString(editText.text)) {
            console.log( qsTr("Please enter numbers only"));
            return;
        }
        view.textEntered(editText.text)
    }
    signal rejected()
    onRejected: console.log("Rejected")
    signal textEntered(string text)
    onTextEntered: console.log("Text entered: " + text)

    Text {
        id: title
        objectName: "title"
        text: "Enter code"
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: icon.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        wrapMode: Text.WordWrap
        color: theme.dialogTitleFontColor
        font.pixelSize: theme.dialogTitleFontPixelSize
    }

    Image {
        id: icon
        objectName: "icon"
        source: "qrc:/sim_32x32.xpm"
        width: 64
        height: 64
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
    }

    Rectangle {
        id: editRect
        objectName: "editRect"
        color: "#20408000"
        height: 30
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: icon.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        TextInput {
            id: editText
            objectName: "editText"
            anchors.fill: parent
            horizontalAlignment: TextInput.AlignHCenter
            echoMode: TextInput.Password
            property bool isNumeric: true
            property bool hideTyping: true
            property int minChars: 4
            property int maxChars: 4
        }
    }

    Rectangle {
        id: okRect
        objectName: "okRect"
        width: 80
        height: 30
        color: "#20408000"
        anchors.horizontalCenterOffset: -50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        Text {
            id: okText
            text: qsTr( "Ok" )
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: view.accepted()
        }
    }

    Rectangle {
        id: cancelRect
        objectName: "cancelRect"
        width: 80
        height: 30
        color: "#20408000"
        anchors.horizontalCenterOffset: 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        Text {
            id: cancelText
            text: qsTr( "Cancel" )
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: view.rejected()
        }
    }

}





