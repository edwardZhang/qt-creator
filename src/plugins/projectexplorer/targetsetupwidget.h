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

#ifndef TARGETSETUPWIDGET_H
#define TARGETSETUPWIDGET_H

#include "projectexplorer_export.h"

#include "buildinfo.h"
#include "task.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;
class QSpacerItem;
QT_END_NAMESPACE

namespace Utils {
class DetailsWidget;
class PathChooser;
} // namespace Utils

namespace ProjectExplorer {
class BuildInfo;
class Kit;

namespace Internal {

class TargetSetupWidget : public QWidget
{
    Q_OBJECT
public:
    TargetSetupWidget(Kit *k,
                      const QString &projectPath,
                      const QList<BuildInfo *> &infoList);
    ~TargetSetupWidget();

    Kit *kit();
    void clearKit();

    bool isKitSelected() const;
    void setKitSelected(bool b);

    void addBuildInfo(BuildInfo *info, bool isImport);

    QList<const BuildInfo *> selectedBuildInfoList() const;
    void setProjectPath(const QString &projectPath);

signals:
    void selectedToggled() const;

private slots:
    void handleKitUpdate(ProjectExplorer::Kit *k);

    void checkBoxToggled(bool b);
    void pathChanged();
    void targetCheckBoxToggled(bool b);
    void manageKit();

private:
    void reportIssues(int index);
    QPair<Task::TaskType, QString> findIssues(const BuildInfo *info);
    void clear();

    Kit *m_kit;
    QString m_projectPath;
    bool m_haveImported;
    Utils::DetailsWidget *m_detailsWidget;
    QPushButton *m_manageButton;
    QGridLayout *m_newBuildsLayout;
    QList<QCheckBox *> m_checkboxes;
    QList<Utils::PathChooser *> m_pathChoosers;
    QList<BuildInfo *> m_infoList;
    QList<bool> m_enabled;
    QList<QLabel *> m_reportIssuesLabels;
    QList<bool> m_issues;
    bool m_ignoreChange;
    int m_selected; // Number of selected buildconfiguartions
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // TARGETSETUPWIDGET_H
