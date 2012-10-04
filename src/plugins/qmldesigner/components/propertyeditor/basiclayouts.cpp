/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/

#include "basiclayouts.h"


QT_BEGIN_NAMESPACE

QBoxLayoutObject::QBoxLayoutObject(QObject *parent)
: QLayoutObject(parent), _layout(0)
{
}

QBoxLayoutObject::QBoxLayoutObject(QBoxLayout *layout, QObject *parent)
: QLayoutObject(parent), _layout(layout)
{
}

QLayout *QBoxLayoutObject::layout() const
{
    return _layout;
}

void QBoxLayoutObject::addWidget(QWidget *wid)
{
    _layout->addWidget(wid);
}

void QBoxLayoutObject::clearWidget()
{
}

QHBoxLayoutObject::QHBoxLayoutObject(QObject *parent)
: QBoxLayoutObject(new QHBoxLayout, parent)
{
}


QVBoxLayoutObject::QVBoxLayoutObject(QObject *parent)
: QBoxLayoutObject(new QVBoxLayout, parent)
{
}

QT_END_NAMESPACE

void BasicLayouts::registerDeclarativeTypes()
{
    qmlRegisterType<QBoxLayoutObject>("Bauhaus",1,0,"QBoxLayout");
    qmlRegisterType<QHBoxLayoutObject>("Bauhaus",1,0,"QHBoxLayout");
    qmlRegisterType<QVBoxLayoutObject>("Bauhaus",1,0,"QVBoxLayout");
}

