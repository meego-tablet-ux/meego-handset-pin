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
        - isNumeric: false
        - hideTyping: true
        - minChars: -1
        - maxChars: -1
    - \b okRect : "Ok" button.
      - Signals
        - accepted()
    - \b cancelRect : "Cancel" button.
      - Signals
        - rejected()
*/

import Qt 4.7
import MeeGo.Components 0.1
import "qrc:/SimUiCheck.js" as SimUiCheck

Rectangle {
    id: view
    objectName: "view"
    width: 480
    height: 320

    Theme { id: theme }
    SimTheming { id: simTheme }
    color: simTheme.viewBackgroundColor

    signal accepted()
    onAccepted: {
        console.log("Accepted");
        console.log("Text len: ", editText.text.length, ", bounds: [", editText.minChars, "..", editText.maxChars, "], numeric: ", editText.isNumeric);
        if (editText.minChars != -1 && editText.text.length < editText.minChars) {
            formatMsgBox.text = qsTr("You entered ") + editText.text.length + qsTr(" characters.") + "\n" +
            qsTr("Please enter at least ") + editText.minChars + qsTr(" characters.");
            formatMsgBox.show();
            return;
        }
        if (editText.maxChars != -1 && editText.text.length > editText.maxChars) {
            formatMsgBox.text = qsTr("You entered ") + editText.text.length + qsTr(" characters.") + "\n" +
            qsTr("Please enter no more than ") + editText.maxChars + qsTr(" characters.");
            formatMsgBox.show();
            return;
        }
        if (editText.isNumeric && !SimUiCheck.checkNumericString(editText.text)) {
            formatMsgBox.text = qsTr("Please enter numbers only");
            formatMsgBox.show();
            return;
        }
        view.textEntered(editText.text)
    }
    signal rejected()
    onRejected: console.log("Rejected")
    signal textEntered(string text)
    onTextEntered: console.log("Text entered: " + text)

    Label {
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

    TextEntry {
        id: editText
        objectName: "editText"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: icon.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        textInput.horizontalAlignment: TextInput.AlignHCenter
        textInput.focus: true
        textInput.echoMode: TextInput.Password
        property bool isNumeric: false
        property bool hideTyping: true
        property int minChars: -1
        property int maxChars: -1
    }

    SimButton {
        id: okRect
        objectName: "okRect"
        anchors.horizontalCenterOffset: -50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: qsTr( "Ok" )
        onClicked: {
            view.accepted()
        }
    }

    SimButton {
        id: cancelRect
        objectName: "cancelRect"
        anchors.horizontalCenterOffset: 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: qsTr( "Cancel" )
        onClicked: {
            view.rejected()
        }
    }

    ModalMessageBox {
        id: formatMsgBox
        title: qsTr("Attention")
        text: ""
        showAcceptButton: true
        showCancelButton: false
        fogClickable: false
        acceptButtonText: qsTr( "Ok")
    }

}





