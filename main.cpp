/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

/* Qt includes */
#include <QtGui/QApplication>
#include <QtDebug>

/* oFono DBus types */
#include "ofonodbustypes.h"

/* PinApplication */
#include "pinapplication.h"

int main(int argc, char *argv[])
{
    int mainErr = -1;
    PinApplication app(argc, argv);

    // Register meta types defined in ofonodbustypes.h"
    registerOfonoDbusTypes();

    // Initiate connection with oFono
    bool ofonoConnectionReady = app.initOfonoConnection();
    if (!ofonoConnectionReady) {
        qDebug() << "Error: oFono connection not ready";
        return mainErr;
    }

    // Run SimToolkit application
    mainErr = app.exec();
    qDebug() << "Exit : " << mainErr;

    return mainErr;
}

