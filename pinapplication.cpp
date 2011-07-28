/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QIcon>
#include <QPixmap>

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
    mDialogOpen = false;
    setQuitOnLastWindowClosed(false);
    setWindowIcon(QIcon(QPixmap(sim_32x32_xpm)));
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

    // Read initial value for SIM interface properties
    mSimProperties = new SimOfonoProperties(mSimIf);
    // Check if a pin code is already required
    QString pinRequired = mSimProperties->getPropertyValue("PinRequired").toString();
    if (!pinRequired.isEmpty() && pinRequired != "none")
        openDialog();

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

    // First check if we are interested in the property changed
    if (property != "PinRequired" && property != "Retries") {
        qDebug() << "simPropertyChanged: Exit!" << " property (" << property << ") is not 'PinRequired' nor 'Retries'";
        return;
    }

    // Update current SIM interface properties
    if (mSimProperties != NULL)
        delete mSimProperties;
    mSimProperties = new SimOfonoProperties(mSimIf);

    // Get the property values we are interested in
    QString pinRequired = mSimProperties->getPropertyValue("PinRequired").toString();

    // Check error / bad cases
    if (pinRequired.isEmpty() || pinRequired == "none") {
        qDebug() << "simPropertyChanged: Exit!" << " pinRequired:" << pinRequired;
        return;
    }
    if (mDialogOpen) {
        qDebug() << "simPropertyChanged: Exit! (Dialog already open)";
        return;
    }

    // Open dialog
    openDialog();
}

void PinApplication::openDialog()
{
    // Get the property values we are interested in
    QString pinRequired = mSimProperties->getPropertyValue("PinRequired").toString();
    QMap<QString, uchar> retries = mSimProperties->getRetries();
    int pinRetries = retries[pinRequired];

    mDialogOpen = true;

    // Displaying 'pin/puk...' code dialog and send response to modem
    QString text = tr("Enter code ") + pinRequired;
    if (pinRetries)
        text += " (" + QString::number((int)pinRetries) + tr(" left") + ")";
    SimDialog dlg(text, "qrc:/SimPassword.qml");

    QDBusError dbusError;
    QDBusPendingReply<> enterPinCall;
    AgentResponse ret;

    dlg.setHideTyping(true);
    dlg.initView();

    do {
        dlg.exec();
        ret = dlg.getAgentResponse();
        switch (ret) {
        case Ok:
            qDebug() << "EnterPin: " << pinRequired << " : " << dlg.getResponseData().toString();
            enterPinCall = mSimIf->EnterPin(pinRequired, dlg.getResponseData().toString());
            enterPinCall.waitForFinished();
            if (enterPinCall.isError()) {
                dbusError = enterPinCall.error();
                qDebug() << "simPropertyChanged: EnterPin error " << dbusError.type() << " : " << dbusError.name() << ":" << dbusError.message();
            }
            break;
        case Cancel:
            break;
        default:
            Q_ASSERT(false);
        }
    } while(ret==Ok && dbusError.name() == "org.ofono.Error.InvalidFormat");

    mDialogOpen = false;
}
