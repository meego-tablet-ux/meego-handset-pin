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


typedef uchar OfonoByte;
Q_DECLARE_METATYPE(OfonoByte)       /* y */

typedef QMap<QString, OfonoByte> OfonoRetryMap;
Q_DECLARE_METATYPE(OfonoRetryMap)       /* a{sy} */

inline const QDBusArgument &operator>>(const QDBusArgument &arg, OfonoRetryMap &map)
{
    arg.beginMap();
    map.clear();
    while (!arg.atEnd()) {
        QString key;
        OfonoByte value;
        arg.beginMapEntry();
        arg >> key >> value;
        map.insert(key, value);
        arg.endMapEntry();
    }
    arg.endMap();
    return arg;
}

inline QDBusArgument &operator<<(QDBusArgument &arg, const OfonoRetryMap &map)
{
    arg.beginMap(QVariant::String, qMetaTypeId<OfonoByte>());
    QList<QString> keys = map.keys();
    foreach (const QString &key, keys) {
        arg.beginMapEntry();
        arg << key << map.value(key);
        arg.endMapEntry();
    }
    arg.endMap();
    return arg;
}


/* Register all meta types declared above */

inline void registerOfonoDbusTypes()
{
    qDBusRegisterMetaType<OfonoModem>();
    qDBusRegisterMetaType<OfonoModemList>();
    qDBusRegisterMetaType<OfonoRetryMap>();
    qDBusRegisterMetaType<OfonoByte>();
}



#endif // OFONODBUSTYPES_H
