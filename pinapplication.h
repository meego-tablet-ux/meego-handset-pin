/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef PINAPPLICATION_H
#define PINAPPLICATION_H

#include <QApplication>
#include "simif.h"
#include "simofonoproperties.h"

class PinApplication : public QApplication
{
    Q_OBJECT
public:
    explicit PinApplication(int &argc, char **argv, int version = QT_VERSION);
    ~PinApplication();
    bool registerPinPropertyChanged(SimIf *simIf);

signals:

private:
    SimIf *mSimIf;
    SimOfonoProperties *mSimProperties;
	QString	mPinTypeRequired;
	int		mPinRetries;

private slots:
    void simPropertyChanged(const QString &property, const QDBusVariant &value);

};

#endif // PINAPPLICATION_H
