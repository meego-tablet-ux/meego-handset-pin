/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include "pinapplication.h"

PinApplication::PinApplication(int &argc, char **argv, int version) :
    QApplication(argc, argv, version)
{
    mSimIf = NULL;
    mSimProperties = NULL;
}

PinApplication::~PinApplication()
{
    if (mSimProperties != NULL)
        delete mSimProperties;
    // mSimIf is deleted from main.cpp
}

bool PinApplication::registerPinPropertyChanged(SimIf *simIf)
{
    mSimIf = simIf;
    // Connect simIf signals
    return connect(mSimIf, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(simPropertyChanged(QString, QDBusVariant)));
}

void PinApplication::simPropertyChanged(const QString &property, const QDBusVariant &value)
{
    qDebug() << "simPropertyChanged: " << property;
}

