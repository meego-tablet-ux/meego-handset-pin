/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


/*!
    \file SimButton.qml
    \section SimButton
    \brief Overrides default Meego Button geometry
*/

import Qt 4.7
import MeeGo.Components 0.1

Button {
    width: simTheme.buttonWidth
    height: simTheme.buttonHeight
    font.pixelSize: simTheme.buttonFontPixelSize
}
