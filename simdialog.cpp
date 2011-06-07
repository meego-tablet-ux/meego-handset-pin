/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include <QVBoxLayout>
#include <QtDeclarative>
#include "simdialog.h"

SimDialog::SimDialog(const QString &title, const QString &qmlViewUrl, QWidget *parent) :
    QDialog(parent)
{
    mTitle = title;
    mQmlViewUrl = qmlViewUrl;
    mMinChars = mMaxChars = -1;
    mHideTyping = mNumeric = false;
}

void SimDialog::initView()
{
    // Create QML View as central widget
    mView = new QDeclarativeView;
    // Register image provider, deleted with the engine
    mView->setSource(QUrl(mQmlViewUrl));
    mView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mView);
    setLayout(layout);
    // Find qml objects and connect signals
    QObject *object;
    QObject *root = this->mView->rootObject();
    // Main title
    object = root->findChild<QObject*>("title");
    if (object && !mTitle.isEmpty())
        object->setProperty("text", mTitle);
    // Text edit / input
    QObject * editText = root->findChild<QObject*>("editText");
    if (editText) {
        editText->setProperty("text",mDefaultText);
        editText->setProperty("isNumeric",mNumeric);
        editText->setProperty("hideTyping",mHideTyping);
        editText->setProperty("minChars",mMinChars);
        editText->setProperty("maxChars",mMaxChars);
        connect(root, SIGNAL(textEntered(QString)), this, SLOT(responseOkWithText(QString)));
    // Cancel button
    object = root->findChild<QObject*>("cancelRect");
    if (object)
        connect(root, SIGNAL(rejected()), this, SLOT(responseCancel()));
    // default response: Cancel
    agentResponse = Cancel;
    }
}


