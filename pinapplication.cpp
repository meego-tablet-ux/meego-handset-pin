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
	mPinTypeRequired = "none";
	mPinRetries = 0;
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
    qDebug() << "simPropertyChanged: " << property << " variant string : " << value.variant().toString();
    if (mSimProperties != NULL)
        delete mSimProperties;
    mSimProperties = new SimOfonoProperties(mSimIf);
	qDebug() << "property : " << property;

	// Getting which 'pin/puk...' code is requested
	if (property == "PinRequired")
	{
		mPinTypeRequired = value.variant().toString();
	}

	// Getting remaining retries
	if (property == "Retries")
	{
		QMap<QString, uchar> simRetryProperty;

		simRetryProperty = mSimProperties->getRetryProperties();
		mPinRetries = simRetryProperty[mPinTypeRequired];
		qDebug() << "Retries=" << QString::number((int)mPinRetries);
	}

	// Check error / bad cases
	if (mPinTypeRequired.isNull() || mPinRetries == 0)
	{
		qDebug() << "Exit!" << " mPinTypeRequired:" << mPinTypeRequired << " mPinRetries:" << mPinRetries;
        return;
	}
	else
		if (property == "PinRequired" && value.variant().toString() == "none")
		{
			qDebug() << "Exit!" << " property == 'PinRequired':" << value.variant().toString();
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
		if (enterPinCall.isError())
		{

			QDBusError dbusError = enterPinCall.error();
			qDebug() << "Bad Pin Code!";
		}
		break;
	case Cancel:
		break;
	default:
		Q_ASSERT(false);
	}
}
