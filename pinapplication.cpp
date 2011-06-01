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


#include "sim_32x32.xpm"


PinApplication::PinApplication(int &argc, char **argv, int version) :
    QApplication(argc, argv, version)
{
    mSimIf = NULL;
    mSimProperties = NULL;
    setQuitOnLastWindowClosed ( false );
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
    if (mSimProperties != NULL)
        delete mSimProperties;
    mSimProperties = new SimOfonoProperties(mSimIf);
    // Connect simIf signals
    return connect(mSimIf, SIGNAL(PropertyChanged(QString, QDBusVariant)), this, SLOT(simPropertyChanged(QString, QDBusVariant)));
}

void PinApplication::simPropertyChanged(const QString &property, const QDBusVariant &value)
{
    int Retries = 3;

    qDebug() << "simPropertyChanged: " << property << " variant string : " << value.variant().toString();
    if (mSimProperties != NULL)
        delete mSimProperties;
    mSimProperties = new SimOfonoProperties(mSimIf);
    if (property == "Retries")
    {
        qDebug() << "Retries : " << value.variant().toByteArray();
    }
    if (property != "PinRequired" && property != "Retries")
        return;
    else
        if (value.variant().toString() == "none")
            return;

    QDBusPendingReply<> enterPinCall;
    {
        // Getting "Retries" property
        QMap<QString, uchar> simRetryProperty;
        QString propName = "Retries";

        simRetryProperty = mSimProperties->getRetryProperties();
        SimDialog dlg(value.variant().toString() + "\n" + QString::number((int)Retries) + " retries", "qrc:/SimPassword.qml");

        dlg.setHideTyping(true);
        dlg.initView();
        dlg.exec();

        AgentResponse ret = dlg.getAgentResponse();
        switch (ret) {
        case Ok:
            qDebug() << "EnterPin: " << value.variant().toString() << " : " << dlg.getResponseData().toString();
            enterPinCall = mSimIf->EnterPin(value.variant().toString(), dlg.getResponseData().toString());
            enterPinCall.waitForFinished();
            if (enterPinCall.isError())
            {

                QDBusError dbusError = enterPinCall.error();
                qDebug() << "Bad Pin Code! Retries = " << Retries;
            }
            break;
        case Cancel:
            break;
        default:
            Q_ASSERT(false);
        }
    }
}
