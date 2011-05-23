/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef SIMDIALOG_H
#define SIMDIALOG_H

#include <QtDebug>

#include <QtGui/QDialog>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include "simwidget.h"
#include "simdefines.h"

class SimDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimDialog(SimWidget * simWidget, QWidget *parent = 0);

    inline AgentResponse getAgentResponse()
    {
        return agentResponse;
    }

    inline QVariant getResponseData()
    {
        return responseData;
    }


private:
    AgentResponse agentResponse;
    QVariant responseData;
    SimWidget * mSimWidget;

public slots:
    inline void responseOkWithText(QString text)
    {
        agentResponse = Ok;
        responseData = text;
        accept();
    }

    inline void responseCancel()
    {
        agentResponse = Cancel;
        reject();
    }

};

#endif // SIMDIALOG_H
