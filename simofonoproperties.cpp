/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


 #include <QDBusArgument>
#include <QtDebug>

#include "ofonodbustypes.h"
#include "simofonoproperties.h"


SimOfonoProperties::SimOfonoProperties(SimIf *simIf, QObject *parent) :
    QObject(parent)
{
    mSimIf = simIf;
    // org.ofono.SimToolkit interface GetProperties method
    QDBusPendingReply<QVariantMap> simPropsCall = mSimIf->GetProperties();
    simPropsCall.waitForFinished();
    if (simPropsCall.isError()) {
        QDBusError error = simPropsCall.error();
        qDebug() << "Error:" << error.name() << ":" << error.message();
    }
    else
    {   // get properties map
        mProperties = simPropsCall.value();
    }
}

QVariant SimOfonoProperties::getPropertyValue(QString &propertyName)
{
      return mProperties.value(propertyName);
}

QMap<QString, uchar> SimOfonoProperties::getRetryProperties()
{
    QMap<QString, uchar> orp;

    QVariant variantRetry;
    QDBusArgument::ElementType type;
    QString signature;

    variantRetry = mProperties.value("Retries");
    QDBusArgument arg = variantRetry.value<QDBusArgument>();
    signature = arg.currentSignature();
    type = arg.currentType();
    qDebug() << "signature:" << signature << " type " << type;

    arg >> orp;
 //   orp << arg;
    qDebug() << "orp:" << orp;

    return orp;
}

