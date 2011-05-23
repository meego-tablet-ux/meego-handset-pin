/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include <QVBoxLayout>
#include "simdialog.h"

SimDialog::SimDialog(SimWidget * simWidget, QWidget *parent) :
    QDialog(parent)
{
    mSimWidget = simWidget;
    // layout widget
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mSimWidget);
    setLayout(layout);
    // connect signals
    connect(mSimWidget, SIGNAL(rejected()), this, SLOT(responseCancel()));
    connect(mSimWidget, SIGNAL(textEntered(QString)), this, SLOT(responseOkWithText(QString)));
    // default response: Cancel
    agentResponse = Cancel;
}
