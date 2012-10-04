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

#ifndef KDEPASTE_H
#define KDEPASTE_H

#include "protocol.h"

namespace CodePaster {

class KdePasteProtocol : public NetworkProtocol
{
    Q_OBJECT
public:
    explicit KdePasteProtocol(const NetworkAccessManagerProxyPtr &nw);

    static QString protocolName();
    QString name() const { return protocolName(); }

    virtual unsigned capabilities() const;

    virtual void fetch(const QString &id);
    virtual void paste(const QString &text,
                       ContentType ct = Text,
                       const QString &username = QString(),
                       const QString &comment = QString(),
                       const QString &description = QString());
    virtual void list();

public slots:
    void fetchFinished();
    void pasteFinished();
    void listFinished();

protected:
    virtual bool checkConfiguration(QString *errorMessage = 0);

private:
    QNetworkReply *m_fetchReply;
    QNetworkReply *m_pasteReply;
    QNetworkReply *m_listReply;

    QString m_fetchId;
    int m_postId;
    bool m_hostChecked;
};

} // namespace CodePaster

#endif // KDEPASTE_H
