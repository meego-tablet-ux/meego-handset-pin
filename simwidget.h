/*
 * meego-handset-pin
 * Copyright © 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef SIMWIDGET_H
#define SIMWIDGET_H

#include <QWidget>

class SimWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimWidget(QWidget *parent = 0);

signals:
    void rejected();
    void textEntered(QString text);
};

#endif // SIMWIDGET_H
