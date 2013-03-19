/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "winrtrunfactories.h"
#include "winrtrunconfiguration.h"
#include "winrtruncontrol.h"
#include "winrtdebugsupport.h"

#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitinformation.h>

#include <debugger/debuggerrunner.h>

#include <qtsupport/qtkitinformation.h>

#include <utils/qtcassert.h>

using namespace ProjectExplorer;

namespace WinRt {
namespace Internal {

static const char winrtConfigurationIdC[] = "WinRTConfigurationID";

WinRtRunConfigurationFactory::WinRtRunConfigurationFactory()
{
}

QList<Core::Id> WinRtRunConfigurationFactory::availableCreationIds(Target *parent) const
{
    QList<Core::Id> result;
    QtSupport::BaseQtVersion *version = QtSupport::QtKitInformation::qtVersion(parent->kit());
    // TODO: It would likely be best to generate a QtVersion and base our decision on that,
    // but for now just check the mkspec:
    if (version->mkspec().toString().startsWith(QLatin1String("winrt")))
        result.push_back(Core::Id(winrtConfigurationIdC));
    return result;
}

QString WinRtRunConfigurationFactory::displayNameForId(const Core::Id id) const
{
    Q_UNUSED(id);
    return tr("Run Appx package");
}

bool WinRtRunConfigurationFactory::canCreate(Target *parent, const Core::Id id) const
{
    Q_UNUSED(parent);
    return id == winrtConfigurationIdC;
}

RunConfiguration *WinRtRunConfigurationFactory::create(Target *parent, const Core::Id id)
{
    return new WinRtRunConfiguration(parent, id);
}

bool WinRtRunConfigurationFactory::canRestore(Target *parent, const QVariantMap &map) const
{
    Q_UNUSED(parent);
    return ProjectExplorer::idFromMap(map) == winrtConfigurationIdC;
}

RunConfiguration *WinRtRunConfigurationFactory::restore(Target *parent, const QVariantMap &map)
{
    RunConfiguration *config = new WinRtRunConfiguration(parent, winrtConfigurationIdC);
    config->fromMap(map);
    return config;
}

bool WinRtRunConfigurationFactory::canClone(Target *parent, RunConfiguration *product) const
{
    Q_UNUSED(parent);
    Q_UNUSED(product);
    return false;
}

RunConfiguration *WinRtRunConfigurationFactory::clone(Target *parent, RunConfiguration *product)
{
    Q_UNUSED(parent);
    Q_UNUSED(product);
    return 0;
}

WinRtRunControlFactory::WinRtRunControlFactory()
{
}

bool WinRtRunControlFactory::canRun(ProjectExplorer::RunConfiguration *runConfiguration,
        ProjectExplorer::RunMode mode) const
{
    Q_UNUSED(mode);
    return qobject_cast<WinRtRunConfiguration *>(runConfiguration);
}

ProjectExplorer::RunControl *WinRtRunControlFactory::create(
        RunConfiguration *runConfiguration, RunMode mode, QString *errorMessage)
{
    WinRtRunConfiguration *rc = qobject_cast<WinRtRunConfiguration *>(runConfiguration);
    if (!rc) {
        *errorMessage = tr("Internal error in class WinRTRunControlFactory: Non-WinRT RunConfiguration passed.");
        return 0;
    }
    switch (mode) {
    case NormalRunMode:
        return new WinRtRunControl(rc, mode);
    case DebugRunMode: {
        WinRtDebugSupport *debugger = new WinRtDebugSupport(rc, errorMessage);
        return debugger->runControl();
       }
    default:
        break;
    }
    *errorMessage = tr("Unsupported debug mode.");
    return 0;
}

QString WinRtRunControlFactory::displayName() const
{
    return tr("WinRT Run Control Factory");
}

} // namespace Internal
} // namespace WinRt
