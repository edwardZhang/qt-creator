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

#include "winrtrunconfigurationwidget.h"
#include "winrtrunconfiguration.h"
#include "ui_winrtrunconfigurationwidget.h"

namespace WinRt {
namespace Internal {

WinRtRunConfigurationWidget::WinRtRunConfigurationWidget(WinRtRunConfiguration *rc, QWidget *parent)
    : Utils::DetailsWidget(parent)
    , m_runConfiguration(rc)
    , m_ui(new Ui::WinRtRunConfigurationWidget)
{
    setState(Expanded);
    setSummaryText(tr("Launch Appx"));
    setWidget(new QWidget(this));
    m_ui->setupUi(widget());
    widget()->setContentsMargins(0, 0, 0, 0);
    m_ui->activationID->setText(rc->activationID());
    m_ui->arguments->setText(rc->arguments());
    connect(m_ui->browseButton, SIGNAL(clicked()), SLOT(browse()));
    connect(rc, SIGNAL(activationIDChanged(QString)),
            m_ui->activationID, SLOT(setText(QString)));
    connect(m_ui->arguments, SIGNAL(textChanged(QString)),
            rc, SLOT(setArguments(QString)));
}

WinRtRunConfigurationWidget::~WinRtRunConfigurationWidget()
{
    delete m_ui;
}

void WinRtRunConfigurationWidget::browse()
{
    m_runConfiguration->chooseApplication();
}

void WinRtRunConfigurationWidget::setArguments(const QString &args)
{
    m_ui->arguments->setText(args);
}

} // namespace Internal
} // namespace WinRt
