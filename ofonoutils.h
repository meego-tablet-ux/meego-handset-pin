/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef OFONOUTILS_H
#define OFONOUTILS_H

#include <QDBusConnection>
#include "mgrif.h"
#include "modemif.h"
#include "simif.h"

class OfonoUtils
{
public:
    static OfonoModemList findModems(MgrIf *mgrIf);
    static QList<ModemIf*> findModemInterfaces(const QDBusConnection &connection, MgrIf *mgrIf);
    static QList<SimIf*> findSimInterfaces(const QDBusConnection &connection, MgrIf *mgrIf);
};

#endif // OFONOUTILS_H
