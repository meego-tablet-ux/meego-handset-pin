/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <Qt>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGridLayout>

#include "siminputtext.h"


SimInputText::SimInputText(const QPixmap & pixmap, const QString & text, QWidget *parent) :
    SimWidget(parent)
{
    // initialize private variables
    mMinChars = 4;
    mMaxChars = 4;
    mHideTyping = true;
    mNumeric = true;

    // create widgets
    // main icon
    mIcon = new QLabel(this);
    mIcon->setPixmap(pixmap);
    // main text message
    mText = new QLabel(text, this);
    // input text edit
    mEditText = new QLineEdit(this);
    mEditText->setEchoMode(mHideTyping ? QLineEdit::PasswordEchoOnEdit : QLineEdit::Normal);
    if (mMaxChars != -1)
        mEditText->setMaxLength(mMaxChars);
    // ok button box
    mOkBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);

    // layout widgets
    // grid layout 4x2
    QGridLayout *grid = new QGridLayout(this);
    // main icon topleft
    grid->addWidget(mIcon,0,0,Qt::AlignLeft);
    // main text message middle left
    grid->addWidget(mText,1,0,1,2,Qt::AlignHCenter);
    // input text edit middle left
    grid->addWidget(mEditText,2,0,1,2,Qt::AlignHCenter);
    // ok button box bottom center
    grid->addWidget(mOkBox,3,0,Qt::AlignHCenter);

    // connect signals
    connect(mOkBox, SIGNAL(accepted()), this, SLOT(textEntered()));
    connect(mOkBox, SIGNAL(rejected()), this, SIGNAL(rejected()));
}


SimInputText::~SimInputText()
{
    close();
    delete mText;
    delete mIcon;
    delete mEditText;
    delete mOkBox;
}


void SimInputText::setDefaultText(const QString & defaultText)
{
    mDefaultText = defaultText;
    mText->setText(mDefaultText);
}


void SimInputText::setCharBounds(int minChars, int maxChars)
{
    mMinChars = minChars;
    mMaxChars = maxChars;
    if (mMaxChars != -1)
        mEditText->setMaxLength(maxChars);
    // #### TODO #### Handle minChars input
}


void SimInputText::setHideTyping(bool hideTyping)
{
    mHideTyping = hideTyping;
    mEditText->setEchoMode(mHideTyping ? QLineEdit::PasswordEchoOnEdit : QLineEdit::Normal);
}


void SimInputText::setNumeric(bool numeric)
{
    mNumeric = numeric;
    // #### TODO #### Handle numeric input
}


void SimInputText::textEntered() {
    emit SimWidget::textEntered(mEditText->text());
}
