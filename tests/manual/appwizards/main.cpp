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

#include "qtquickapp.h"
#include "html5app.h"

#include <QApplication>
#include <QDebug>

using namespace Qt4ProjectManager::Internal;

int main(int argc, char *argv[])
{
    QString errorMessage;

    const QString projectPath = QLatin1String("testprojects");

    {
        QtQuickApp sAppNew;
        sAppNew.setProjectPath(projectPath);
        sAppNew.setProjectName(QLatin1String("new_qml_app"));
        if (!sAppNew.generateFiles(&errorMessage))
           return 1;
    }

    {
        QtQuickApp sAppNew;
        sAppNew.setProjectPath(projectPath);
        sAppNew.setComponentSet(QtQuickApp::ComponentSetQtQuick2_0);
        sAppNew.setProjectName(QLatin1String("new_qtquick2_app"));
        if (!sAppNew.generateFiles(&errorMessage))
           return 1;
    }

    {
        QtQuickApp sAppImport01;
        sAppImport01.setProjectPath(projectPath);
        sAppImport01.setProjectName(QLatin1String("qml_imported_scenario_01"));
        sAppImport01.setMainQml(QtQuickApp::ModeImport, QLatin1String("../appwizards/qmlimportscenario_01/myqmlapp.qml"));
        if (!sAppImport01.generateFiles(&errorMessage))
            return 1;
    }

    {
        Html5App sAppNew;
        sAppNew.setProjectPath(projectPath);
        sAppNew.setProjectName(QLatin1String("new_html5_app"));
        qDebug() << sAppNew.path(Html5App::MainHtml);
        if (!sAppNew.generateFiles(&errorMessage))
           return 1;
    }

    {
        Html5App sAppNew;
        sAppNew.setProjectPath(projectPath);
        sAppNew.setProjectName(QLatin1String("html5_imported_scenario_01"));
        sAppNew.setMainHtml(Html5App::ModeImport, QLatin1String("../appwizards/htmlimportscenario_01/themainhtml.html"));
        sAppNew.setTouchOptimizedNavigationEnabled(true);
        qDebug() << sAppNew.path(Html5App::MainHtml);
        if (!sAppNew.generateFiles(&errorMessage))
           return 1;
    }

    {
        Html5App sAppNew;
        sAppNew.setProjectPath(projectPath);
        sAppNew.setProjectName(QLatin1String("html5_url"));
        sAppNew.setMainHtml(Html5App::ModeUrl, QLatin1String("http://www.jqtouch.com/preview/demos/main/"));
        qDebug() << sAppNew.path(Html5App::MainHtml);
        if (!sAppNew.generateFiles(&errorMessage))
           return 1;
    }

    return 0;
}
