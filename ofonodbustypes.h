/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef OFONODBUSTYPES_H
#define OFONODBUSTYPES_H

#include <QString>
#include <QList>
#include <QMap>
#include <QtDBus/QtDBus>


/*
 * Interface: org.ofono.Manager
 * Method: GetModems
 * Argument: Out0
 */

typedef struct
{
    QDBusObjectPath objpath;            /* o */
    QVariantMap varmap;                 /* a{sv} */
} OfonoModem;
Q_DECLARE_METATYPE(OfonoModem)       /* oa{sv} */

inline QDBusArgument &operator<<(QDBusArgument &arg,const OfonoModem &pms)
{
    arg.beginStructure();
    arg << pms.objpath << pms.varmap;
    arg.endStructure();
    return arg;
}

inline const QDBusArgument &operator>>(const QDBusArgument &arg,OfonoModem &pms)
{
    arg.beginStructure();
    arg >> pms.objpath >> pms.varmap;
    arg.endStructure();
    return arg;
}

typedef QList<OfonoModem> OfonoModemList;
Q_DECLARE_METATYPE(OfonoModemList)   /* a(oa{sv}) */



/* Register all meta types declared above */

inline void registerOfonoDbusTypes()
{
    qDBusRegisterMetaType<OfonoModem>();
    qDBusRegisterMetaType<OfonoModemList>();
}



#endif // OFONODBUSTYPES_H
