/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
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

import Qt 4.7

TabWidget {
    id: tabs
    width: 640; height: 480

    Rectangle {
        property string title: "Red"
        anchors.fill: parent
        color: "#e3e3e3"

        Rectangle {
            anchors { fill: parent; topMargin: 20; leftMargin: 20; rightMargin: 20; bottomMargin: 20 }
            color: "#ff7f7f"
            Text {
                width: parent.width - 20
                anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
                text: "Roses are red"
                font.pixelSize: 20
                wrapMode: Text.WordWrap
            }
        }
    }

    Rectangle {
        property string title: "Green"
        anchors.fill: parent
        color: "#e3e3e3"

        Rectangle {
            anchors { fill: parent; topMargin: 20; leftMargin: 20; rightMargin: 20; bottomMargin: 20 }
            color: "#7fff7f"
            Text {
                width: parent.width - 20
                anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
                text: "Flower stems are green"
                font.pixelSize: 20
                wrapMode: Text.WordWrap
            }
        }
    }

    Rectangle {
        property string title: "Blue"
        anchors.fill: parent; color: "#e3e3e3"

        Rectangle {
            anchors { fill: parent; topMargin: 20; leftMargin: 20; rightMargin: 20; bottomMargin: 20 }
            color: "#7f7fff"
            Text {
                width: parent.width - 20
                anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
                text: "Violets are blue"
                font.pixelSize: 20
                wrapMode: Text.WordWrap
            }
        }
    }
}
