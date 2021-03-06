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
#include "remotelinuxenvironmentreader.h"

#include <projectexplorer/devicesupport/deviceprocess.h>
#include <projectexplorer/devicesupport/idevice.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/target.h>

using namespace ProjectExplorer;

namespace RemoteLinux {
namespace Internal {

RemoteLinuxEnvironmentReader::RemoteLinuxEnvironmentReader(RunConfiguration *config, QObject *parent)
    : QObject(parent)
    , m_stop(false)
    , m_env(Utils::OsTypeLinux)
    , m_kit(config->target()->kit())
    , m_deviceProcess(0)
{
    connect(config->target(), SIGNAL(kitChanged()),
        this, SLOT(handleCurrentDeviceConfigChanged()));
}

void RemoteLinuxEnvironmentReader::start()
{
    IDevice::ConstPtr device = DeviceKitInformation::device(m_kit);
    if (!device)
        return;
    m_stop = false;
    m_deviceProcess = device->createProcess(this);
    connect(m_deviceProcess, SIGNAL(error(QProcess::ProcessError)), SLOT(handleError()));
    connect(m_deviceProcess, SIGNAL(finished()), SLOT(remoteProcessFinished()));
    m_deviceProcess->start(QLatin1String("env"));
}

void RemoteLinuxEnvironmentReader::stop()
{
    m_stop = true;
    destroyProcess();
}

void RemoteLinuxEnvironmentReader::handleError()
{
    if (m_stop)
        return;

    emit error(tr("Error: %1").arg(m_deviceProcess->errorString()));
    setFinished();
}

void RemoteLinuxEnvironmentReader::handleCurrentDeviceConfigChanged()
{
    m_env.clear();
    setFinished();
}

void RemoteLinuxEnvironmentReader::remoteProcessFinished()
{
    if (m_stop)
        return;

    m_env.clear();
    QString errorMessage;
    if (m_deviceProcess->exitStatus() != QProcess::NormalExit) {
        errorMessage = m_deviceProcess->errorString();
    } else if (m_deviceProcess->exitCode() != 0) {
        errorMessage = tr("Process exited with code %1.")
                .arg(m_deviceProcess->exitCode());
    }
    if (!errorMessage.isEmpty()) {
        errorMessage = tr("Error running 'env': %1").arg(errorMessage);
        const QString remoteStderr
                = QString::fromUtf8(m_deviceProcess->readAllStandardError()).trimmed();
        if (!remoteStderr.isEmpty())
            errorMessage += tr("\nRemote stderr was: '%1'").arg(remoteStderr);
        emit error(errorMessage);
    } else {
        QString remoteOutput = QString::fromUtf8(m_deviceProcess->readAllStandardOutput());
        if (!remoteOutput.isEmpty()) {
            m_env = Utils::Environment(remoteOutput.split(QLatin1Char('\n'),
                QString::SkipEmptyParts), Utils::OsTypeLinux);
        }
    }
    setFinished();
}

void RemoteLinuxEnvironmentReader::setFinished()
{
    stop();
    emit finished();
}

void RemoteLinuxEnvironmentReader::destroyProcess()
{
    if (!m_deviceProcess)
        return;
    m_deviceProcess->disconnect(this);
    if (m_deviceProcess->state() != QProcess::NotRunning)
        m_deviceProcess->terminate();
    m_deviceProcess->deleteLater();
    m_deviceProcess = 0;
}

} // namespace Internal
} // namespace RemoteLinux
