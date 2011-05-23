/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef SIMINPUTTEXT_H
#define SIMINPUTTEXT_H

#include <QWidget>
#include "simwidget.h"

class QPixmap;
class QLabel;
class QLineEdit;
class QDialogButtonBox;

class SimInputText : public SimWidget
{
    Q_OBJECT
public:
    explicit SimInputText(const QPixmap & pixmap, const QString & text, QWidget *parent = 0);
    virtual ~SimInputText();

    void setDefaultText(const QString & defaultText);
    void setCharBounds(int minChars, int maxChars);
    void setHideTyping(bool hideTyping=true);
    void setNumeric(bool numeric=true);

signals:

private:
    QLabel * mText;
    QLabel * mIcon;
    QLineEdit * mEditText;
    QDialogButtonBox * mOkBox;

    QString mDefaultText;
    int mMinChars;
    int mMaxChars;
    bool mHideTyping;
    bool mNumeric;

private slots:
    void textEntered();
};

#endif // SIMINPUTTEXT_H
