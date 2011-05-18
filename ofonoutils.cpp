/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QtDBus>
#include "ofonodbustypes.h"
#include "ofonoutils.h"


OfonoModemList OfonoUtils::findModems(MgrIf *mgrIf)
{
    // org.ofono.Manager interface GetModems method
    QDBusPendingReply<OfonoModemList> modemsCall = mgrIf->GetModems();
    modemsCall.waitForFinished();
    if (modemsCall.isError()) {
        QDBusError dbusError = modemsCall.error();
        qDebug() << "findModems DBus Error:" << dbusError.name() << ":" << dbusError.message();
        return OfonoModemList();
    }
    return modemsCall.value();
}

QList<SimIf*> OfonoUtils::findSimInterfaces(const QDBusConnection &connection, MgrIf *mgrIf)
{
    QList<SimIf*> simInterfaces;
    OfonoModemList modems = OfonoUtils::findModems(mgrIf);
    // loop foreach modem, find all org.ofono.SimManager interfaces
    foreach(const OfonoModem &pms, modems) {
        // loop interfaces to find "org.ofono.SimManager"
        foreach(const QString &interface, pms.varmap.value("Interfaces").toStringList()) {
            if (interface == "org.ofono.SimManager") {
                // Instanciate proxy for org.ofono.SimManager interface
                simInterfaces.append(new SimIf("org.ofono",pms.objpath.path(),connection,NULL));
                break;
            }
        }
    }
    return simInterfaces;
}

