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

import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    FlickableSection {
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Grid View")

        SectionLayout {

            Label {
                text: qsTr("Cache")
                toolTip: qsTr("Cache buffer")
            }

            SectionLayout {
                SpinBox {
                    backendValue: backendValues.cacheBuffer
                    minimumValue: 0;
                    maximumValue: 1000;
                    decimals: 0
                }

                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Cell Size")
            }

            SecondColumnLayout {

                Label {
                    text: "W"
                    width: 12
                }

                SpinBox {
                    backendValue: backendValues.cellWidth
                    minimumValue: 0;
                    maximumValue: 1000;
                    decimals: 0
                }

                Label {
                    text: "H"
                    width: 12
                }

                SpinBox {
                    backendValue: backendValues.cellHeight
                    minimumValue: 0;
                    maximumValue: 1000;
                    decimals: 0
                }

                ExpandingSpacer {

                }
            }


            Label {
                text: qsTr("Flow")
            }

            SecondColumnLayout {
                ComboBox {
                    model: ["LeftToRight", "TopToBottom"]
                    backendValue: backendValues.flow
                    Layout.fillWidth: true
                }
            }

            Label {
                text: qsTr("Navigation wraps")
                toolTip: qsTr("Determines whether the grid wraps key navigation.")
            }

            SectionLayout {
                CheckBox {
                    backendValue: backendValues.keyNavigationWraps
                }

                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Layout Direction")
            }

            SecondColumnLayout {
                ComboBox {
                    model: ["LeftToRight", "RightToLeft"]
                    backendValue: backendValues.layoutDirection
                    Layout.fillWidth: true
                }
            }

            Label {
                text: qsTr("Snap mode")
                toolTip: qsTr("Determines how the view scrolling will settle following a drag or flick.")
            }

            SecondColumnLayout {
                ComboBox {
                    model: ["NoSnap", "SnapToRow", "SnapOneRow"]
                    backendValue: backendValues.snapMode
                    Layout.fillWidth: true
                }
            }

        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Grid View Highlight")

        SectionLayout {

            Label {
                text: qsTr("Range")
                toolTip: qsTr("Highlight range")
            }

            SecondColumnLayout {
                ComboBox {
                    model: ["NoHighlightRange", "ApplyRange", "StrictlyEnforceRange"]
                    backendValue: backendValues.highlightRangeMode
                    Layout.fillWidth: true
                }
            }


            Label {
                text: qsTr("Move duration")
                toolTip: qsTr("Move animation duration of the highlight delegate.")
            }

            SectionLayout {
                SpinBox {
                    backendValue: backendValues.highlightMoveDuration
                    minimumValue: 0;
                    maximumValue: 1000;
                    decimals: 0
                }

                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Move speed")
                toolTip: qsTr("Move animation speed of the highlight delegate.")
            }

            SectionLayout {
                SpinBox {
                    backendValue: backendValues.highlightMoveSpeed
                    minimumValue: 0;
                    maximumValue: 1000;
                    decimals: 0
                }

                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Preferred begin")
                toolTip: qsTr("Preferred highlight begin - must be smaller than Preferred end.")
            }

            SectionLayout {
                SpinBox {
                    backendValue: backendValues.preferredHighlightBegin
                    minimumValue: 0;
                    maximumValue: 1000;
                    decimals: 0
                }

                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Preferred end")
                toolTip: qsTr("Preferred highlight end - must be larger than Preferred begin.")
            }

            SectionLayout {
                SpinBox {
                    backendValue: backendValues.preferredHighlightEnd
                    minimumValue: 0;
                    maximumValue: 1000;
                    decimals: 0
                }

                ExpandingSpacer {

                }
            }

            Label {
                text: qsTr("Follows current")
                toolTip: qsTr("Determines whether the highlight is managed by the view.")
            }

            SectionLayout {
                CheckBox {
                    backendValue: backendValues.highlightFollowsCurrentItem
                }

                ExpandingSpacer {

                }
            }

        }
    }
}
