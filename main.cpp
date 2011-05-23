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
#include <QDBusConnection>
#include <QtDebug>

/* oFono DBus interfaces */
#include "mgrif.h"
#include "simif.h"

/* oFono utilities*/
#include "ofonoutils.h"

/* oFono DBus types */
#include "ofonodbustypes.h"

/* PinApplication */
#include "pinapplication.h"

int main(int argc, char *argv[])
{
    int mainErr = -1;
    PinApplication a(argc, argv);

    // Register meta types defined in ofonodbustypes.h"
    registerOfonoDbusTypes();

    // DBus Connection systemBus
    QDBusConnection systemBus = QDBusConnection::systemBus();
    if( !systemBus.isConnected() ) {
        QDBusError dbusError = systemBus.lastError();
        qDebug() << "Error:" << dbusError.name() << ":" << dbusError.message();
        return mainErr;
    }

    // Instanciate proxy for org.ofono.Manager interface
    MgrIf mgrIf("org.ofono","/",systemBus,NULL);

    // find org.ofono.SimManager interfaces for all modems
    QList<SimIf*> simIfs = OfonoUtils::findSimInterfaces(systemBus,&mgrIf);
    if (simIfs.isEmpty()) {
        qDebug() << "No  org.ofono.SimManager interface found, exiting";
        return mainErr;
    }

    // use just the first element of SimManager interfaces list
    SimIf *firstSimIf = simIfs.first();

    // register app to track changes on "PinRequired" property
    a.registerPinPropertyChanged(firstSimIf);

    // Run SimToolkit application
    mainErr = a.exec();

    // delete all org.ofono.SimManager interfaces
    while (!simIfs.isEmpty()) {
        delete simIfs.first();
        simIfs.removeFirst();
    }

    return mainErr;
}

