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

#include "statusbarmanager.h"

#include "coreconstants.h"
#include "mainwindow.h"
#include "statusbarwidget.h"

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <aggregation/aggregate.h>
#include <extensionsystem/pluginmanager.h>

#include <QSettings>
#include <QHBoxLayout>
#include <QLabel>
#include <QStatusBar>

using namespace Core;
using namespace Core::Internal;

StatusBarManager::StatusBarManager(MainWindow *mainWnd)
  : QObject(mainWnd),
    m_mainWnd(mainWnd)
{
    for (int i = 0; i <= StatusBarWidget::Last; ++i) {
        QWidget *w = new QWidget();
        m_mainWnd->statusBar()->insertPermanentWidget(i, w);
        w->setLayout(new QHBoxLayout);
        w->setVisible(true);
        w->layout()->setMargin(0);
        m_statusBarWidgets.append(w);
    }
    m_mainWnd->statusBar()->insertPermanentWidget(StatusBarWidget::Last+1,
                                                  new QLabel(), 1);
}

StatusBarManager::~StatusBarManager()
{
}

void StatusBarManager::init()
{
    connect(ExtensionSystem::PluginManager::instance(), SIGNAL(objectAdded(QObject*)),
            this, SLOT(objectAdded(QObject*)));
    connect(ExtensionSystem::PluginManager::instance(), SIGNAL(aboutToRemoveObject(QObject*)),
            this, SLOT(aboutToRemoveObject(QObject*)));
}

void StatusBarManager::objectAdded(QObject *obj)
{
    StatusBarWidget *view = Aggregation::query<StatusBarWidget>(obj);
    if (!view)
        return;

    QWidget *viewWidget = 0;
    viewWidget = view->widget();
    m_statusBarWidgets.at(view->position())->layout()->addWidget(viewWidget);

    m_mainWnd->addContextObject(view);
}

void StatusBarManager::aboutToRemoveObject(QObject *obj)
{
    StatusBarWidget *view = Aggregation::query<StatusBarWidget>(obj);
    if (!view)
        return;
    m_mainWnd->removeContextObject(view);
}

void StatusBarManager::extensionsInitalized()
{
}
