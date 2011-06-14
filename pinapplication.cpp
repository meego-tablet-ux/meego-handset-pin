/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include "pinapplication.h"

#include "simdialog.h"
#include "simdefines.h"
#include "ofonoutils.h"


#include "sim_32x32.xpm"


PinApplication::PinApplication(int &argc, char **argv, int version) :
    QApplication(argc, argv, version)
{
    mMgrIf = NULL;
    mModemIf = NULL;
    mSimIf = NULL;
    mSimProperties = NULL;
    mPinTypeRequired = "none";
    mPinRetries = 0;
    setQuitOnLastWindowClosed(false);
}

PinApplication::~PinApplication()
{
    // disconnect, delete all interfaces
    deleteInterfaces();
}

void PinApplication::resetInterfaces()
{
    // Disconnect all signals sent from oFono interfaces
    if (mMgrIf != NULL) {
        disconnect(mMgrIf, SIGNAL(ModemAdded(QDBusObjectPath, QVariantMap)), this, SLOT(mgrModemAdded(QDBusObjectPath, QVariantMap)));
        disconnect(mMgrIf, SIGNAL(ModemRemoved(QDBusObjectPath)), this, SLOT(mgrModemRemoved(QDBusObjectPath)));
    }
    if (mModemIf != NULL)
        disconnect(mModemIf, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(modemPropertyChanged(QString, QDBusVariant)));
    if (mSimIf != NULL)
        disconnect(mSimIf, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(simPropertyChanged(QString, QDBusVariant)));
    // delete current SIM interface properties
    if (mSimProperties != NULL)
        delete mSimProperties;
    mSimProperties = NULL;
    mModemIf = NULL; // delete with mModemIfs
    mSimIf = NULL; // delete with mSimIfs
}

void PinApplication::deleteInterfaces()
{
    // disconnect from all oFono signals
    resetInterfaces();
    // delete all org.ofono.SimManager interfaces
    while (!mSimIfs.isEmpty()) {
        delete mSimIfs.first();
        mSimIfs.removeFirst();
    }
    // delete all org.ofono.Modem interfaces
    while (!mModemIfs.isEmpty()) {
        delete mModemIfs.first();
        mModemIfs.removeFirst();
    }
    // disconnected by resetInterfaces
    if (mMgrIf != NULL) {
        delete mMgrIf;
        mMgrIf = NULL;
    }
}

bool PinApplication::initOfonoConnection()
{
    // disconnect, delete all interfaces
    deleteInterfaces();
    // DBus Connection systemBus
    QDBusConnection connection = QDBusConnection::systemBus();
    if( !connection.isConnected() ) {
        QDBusError dbusError = connection.lastError();
        qDebug() << "Error:" << dbusError.name() << ":" << dbusError.message();
        return false;
    }
    // Instanciate proxy for org.ofono.Manager interface
    mMgrIf = new MgrIf("org.ofono","/",connection,NULL);
    // find all modems
    OfonoModemList modems = OfonoUtils::findModems(mMgrIf);
    if (modems.isEmpty()) {
        qDebug() << "No modem found, registering for modem add / removed";
        registerModemMgrChanges();
        return true;
    }
    // find org.ofono.Modem interfaces
    mModemIfs = OfonoUtils::findModemInterfaces(connection, mMgrIf);
    if (mModemIfs.isEmpty()) {
        qDebug() << "No modem interface found, registering for modem add / removed";
        registerModemMgrChanges();
        return true;
    }
    // Use the first Modem available
    mModemIf = mModemIfs.first();
    // find org.ofono.SimManager interfaces for all modems
    mSimIfs = OfonoUtils::findSimInterfaces(connection, mMgrIf);
    if (mSimIfs.isEmpty()) {
        qDebug() << "No SIM interface found, registering for Modem property changes";
        registerModemPropertyChanged();
        return true;
    }
    // Use the first SimManager available
    mSimIf = mSimIfs.first();
    // Register for simPropertyChanged
    registerSimPropertyChanged();
    return true;
}

bool PinApplication::registerModemMgrChanges()
{
    // Connect mgrIf modem added / removed signals
    bool reg1 = connect(mMgrIf, SIGNAL(ModemAdded(QDBusObjectPath, QVariantMap)), this, SLOT(mgrModemAdded(QDBusObjectPath, QVariantMap)));
    bool reg2 = connect(mMgrIf, SIGNAL(ModemRemoved(QDBusObjectPath)), this, SLOT(mgrModemRemoved(QDBusObjectPath)));
    return reg1 && reg2;
}

bool PinApplication::registerModemPropertyChanged()
{
    // Connect simIf signals
    return connect(mModemIf, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(modemPropertyChanged(QString, QDBusVariant)));
}

bool PinApplication::registerSimPropertyChanged()
{
    // Connect simIf signals
    return connect(mSimIf, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(simPropertyChanged(QString, QDBusVariant)));
}

void PinApplication::mgrModemAdded(const QDBusObjectPath &in0, const QVariantMap &in1)
{
    qDebug() << "mgrModemAdded: " << in0.path() << " variant map: " << in1;
    initOfonoConnection();
}

void PinApplication::mgrModemRemoved(const QDBusObjectPath &in0)
{
    qDebug() << "mgrModemRemoved: " << in0.path();
    initOfonoConnection();
}

void PinApplication::modemPropertyChanged(const QString &property, const QDBusVariant &value)
{
    qDebug() << "modemPropertyChanged: " << property << " variant string : " << value.variant().toString();
    initOfonoConnection();
}

void PinApplication::simPropertyChanged(const QString &property, const QDBusVariant &value)
{
    qDebug() << "simPropertyChanged: " << property << " variant string : " << value.variant().toString();
    // Update current SIM interface properties
    if (mSimProperties != NULL)
        delete mSimProperties;
    mSimProperties = new SimOfonoProperties(mSimIf);

    // Getting which 'pin/puk...' code is requested
    if (property == "PinRequired") {
        mPinTypeRequired = value.variant().toString();
    }

    // Getting remaining retries
    if (property == "Retries") {
        QMap<QString, uchar> simRetryProperty;

        simRetryProperty = mSimProperties->getRetryProperties();
        mPinRetries = simRetryProperty[mPinTypeRequired];
		qDebug() << "simPropertyChanged: Retries=" << QString::number((int)mPinRetries);
    }

    // Check error / bad cases
    if (mPinTypeRequired.isNull() || mPinRetries == 0) {
		qDebug() << "simPropertyChanged: Exit!" << " mPinTypeRequired:" << mPinTypeRequired << " mPinRetries:" << mPinRetries;
        return;
    }
	else if (property == "PinRequired" && value.variant().toString() == "none") {
		qDebug() << "simPropertyChanged: Exit!" << " property == 'PinRequired':" << value.variant().toString();
		return;
	}
	if (property != "PinRequired" && property != "Retries") {
		qDebug() << "simPropertyChanged: Exit!" << " property (" << property << ") is not 'PinRequired' nor Retries";
		return;
	}

    // Displaying 'pin/puk...' code dialog and send response to modem
    QDBusPendingReply<> enterPinCall;
    QString text;
    if (mPinRetries > 1)
        text = " remaining tries left";
    else
        text = " remaining try left";
    SimDialog dlg(mPinTypeRequired + " code required!\n" + QString::number((int)mPinRetries) + text, "qrc:/SimPassword.qml");

    dlg.setHideTyping(true);
    dlg.initView();
    dlg.exec();

    AgentResponse ret = dlg.getAgentResponse();
    switch (ret) {
    case Ok:
        qDebug() << "EnterPin: " << mPinTypeRequired << " : " << dlg.getResponseData().toString();
        enterPinCall = mSimIf->EnterPin(mPinTypeRequired, dlg.getResponseData().toString());
        enterPinCall.waitForFinished();
        if (enterPinCall.isError()) {
            QDBusError dbusError = enterPinCall.error();
			qDebug() << "simPropertyChanged: Bad Pin Code!";
        }
        break;
    case Cancel:
        break;
    default:
        Q_ASSERT(false);
    }
}
