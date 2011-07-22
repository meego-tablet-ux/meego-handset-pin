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
#include <QtDeclarative/QDeclarativeView>

#include "simdefines.h"

class SimDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimDialog(const QString &title, const QString &qmlViewUrl, QWidget *parent = 0);

    inline AgentResponse getAgentResponse()
    {
        return agentResponse;
    }

    inline QVariant getResponseData()
    {
        return responseData;
    }

    inline void setDefaultText(const QString &defaultText)
    {
        mDefaultText = defaultText;
    }

    inline void setNumeric(const bool numeric)
    {
        mNumeric = numeric;
    }

    inline void setHideTyping(const bool hideTyping)
    {
        mHideTyping = hideTyping;
    }

    inline void setCharBounds(const int minChars, const int maxChars)
    {
        mMinChars = minChars;
        mMaxChars = maxChars;
    }

private:
    AgentResponse agentResponse;
    QVariant responseData;
    // QML view specifics, inline setters
    // for "editText" items
    QString mDefaultText;
    bool mNumeric;
    bool mHideTyping;
    int mMinChars;
    int mMaxChars;
    // QML view support
    QDeclarativeView *mView;
    // constructor parameters
    QString mTitle;
    QString mQmlViewUrl;

public:
    void initView();

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
