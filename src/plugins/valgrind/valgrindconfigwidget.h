/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
** Author: Milian Wolff, KDAB (milian.wolff@kdab.com)
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

#ifndef ANALYZER_VALGRINDCONFIGWIDGET_H
#define ANALYZER_VALGRINDCONFIGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

namespace Valgrind {
namespace Internal {

namespace Ui {
class ValgrindConfigWidget;
}

class ValgrindBaseSettings;

class ValgrindConfigWidget : public QWidget
{
    Q_OBJECT

public:
    ValgrindConfigWidget(ValgrindBaseSettings *settings, QWidget *parent, bool global);
    virtual ~ValgrindConfigWidget();

    void setSuppressions(const QStringList &files);
    QStringList suppressions() const;

public Q_SLOTS:
    void slotAddSuppression();
    void slotRemoveSuppression();
    void slotSuppressionsRemoved(const QStringList &files);
    void slotSuppressionsAdded(const QStringList &files);
    void slotSuppressionSelectionChanged();

private slots:
    void updateUi();

private:
    ValgrindBaseSettings *m_settings;
    Ui::ValgrindConfigWidget *m_ui;
    QStandardItemModel *m_model;
};

} // namespace Internal
} // namespace Valgrind

#endif // ANALYZER_VALGRINDCONFIGWIDGET_H
