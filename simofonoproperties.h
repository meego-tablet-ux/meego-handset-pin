/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef SIMOFONOPROPERTIES_H
#define SIMOFONOPROPERTIES_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include "simif.h"

class SimOfonoProperties : public QObject
{
    Q_OBJECT
public:
    explicit SimOfonoProperties(SimIf *simIf, QObject *parent = 0);
    QVariant getPropertyValue(const QString &propertyName);
    QMap<QString, uchar> getRetries();
    QString getPinRequired();
signals:

private:
    QVariantMap mProperties;
    SimIf *mSimIf;
};

#endif // SIMOFONOPROPERTIES_H
