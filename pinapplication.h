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
#include "mgrif.h"
#include "modemif.h"
#include "simif.h"
#include "simofonoproperties.h"

class PinApplication : public QApplication
{
    Q_OBJECT
public:
    explicit PinApplication(int &argc, char **argv, int version = QT_VERSION);
    ~PinApplication();

    bool initOfonoConnection();

signals:

private:
    // DBus interfaces
    MgrIf *mMgrIf;
    ModemIf *mModemIf;
    SimIf *mSimIf;
    // interfaces explicitly freed at destruction
    QList<ModemIf*> mModemIfs;
    QList<SimIf*> mSimIfs;
    // current SIM interface properties
    SimOfonoProperties *mSimProperties;
    // current pin required status
	QString	mPinTypeRequired;
	int		mPinRetries;

    // methods
    void resetInterfaces();
    void deleteInterfaces();

    bool registerModemMgrChanges();
    bool registerModemPropertyChanged();
    bool registerSimPropertyChanged();

private slots:
    void mgrModemAdded(const QDBusObjectPath &in0, const QVariantMap &in1);
    void mgrModemRemoved(const QDBusObjectPath &in0);
    void modemPropertyChanged(const QString &property, const QDBusVariant &value);
    void simPropertyChanged(const QString &property, const QDBusVariant &value);

};

#endif // PINAPPLICATION_H
