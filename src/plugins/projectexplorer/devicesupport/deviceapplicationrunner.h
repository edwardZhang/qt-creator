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
#ifndef ONDEVICEAPPLICATIONRUNNER_H
#define ONDEVICEAPPLICATIONRUNNER_H

#include "idevice.h"

#include "../projectexplorer_export.h"

#include <QObject>

namespace ProjectExplorer {

class PROJECTEXPLORER_EXPORT DeviceApplicationHelperAction : public QObject
{
    Q_OBJECT
public:
    ~DeviceApplicationHelperAction();
    virtual void start() = 0;
    virtual void stop() = 0;
signals:
    void reportProgress(const QString &progressOutput);
    void reportError(const QString &errorOutput);
    void finished(bool success);

protected:
    DeviceApplicationHelperAction(QObject *parent = 0);
};

class PROJECTEXPLORER_EXPORT DeviceApplicationRunner : public QObject
{
    Q_OBJECT
public:
    explicit DeviceApplicationRunner(QObject *parent = 0);
    virtual ~DeviceApplicationRunner();

    void start(const IDevice::ConstPtr &device, const QByteArray &commandLine);
    void stop(const QByteArray &stopCommand);

    // Use these if you need to do something before and after the application is run, respectively.
    // Typically, the post-run action reverts the effects of the pre-run action.
    // If you only have a pre-run action, you probably want a deploy step instead.
    void setPreRunAction(DeviceApplicationHelperAction *action);
    void setPostRunAction(DeviceApplicationHelperAction *action);

signals:
    void remoteStdout(const QByteArray &output);
    void remoteStderr(const QByteArray &output);
    void reportProgress(const QString &progressOutput);
    void reportError(const QString &errorOutput);
    void remoteProcessStarted();
    void finished(bool success);

private slots:
    void handleConnected();
    void handleConnectionFailure();
    void handleHelperActionFinished(bool success);
    void handleStopTimeout();
    void handleApplicationFinished(int exitStatus);
    void handleRemoteStdout();
    void handleRemoteStderr();

private:
    void addAction(DeviceApplicationHelperAction *&target, DeviceApplicationHelperAction *source);
    void connectToServer();
    void executePreRunAction();
    void executePostRunAction();
    void runApplication();
    void setFinished();

    class DeviceApplicationRunnerPrivate;
    DeviceApplicationRunnerPrivate * const d;
};

} // namespace ProjectExplorer

#endif // ONDEVICEAPPLICATIONRUNNER_H
