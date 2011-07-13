/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
** This file is part of the Ovi services plugin for the Maps and
** Navigation API.  The use of these services, whether by use of the
** plugin or by other means, is governed by the terms and conditions
** described by the file OVI_SERVICES_TERMS_AND_CONDITIONS.txt in
** this package, located in the directory containing the Ovi services
** plugin source code.
**
****************************************************************************/

#include "qgeosearchreply_nokia.h"
#include "qgeocodexmlparser.h"

QT_BEGIN_NAMESPACE

QGeoSearchReplyNokia::QGeoSearchReplyNokia(QNetworkReply *reply, int limit, int offset, QGeoBoundingArea *viewport, QObject *parent)
        : QGeoSearchReply(parent),
        m_reply(reply)
{
    connect(m_reply,
            SIGNAL(finished()),
            this,
            SLOT(networkFinished()));

    connect(m_reply,
            SIGNAL(error(QNetworkReply::NetworkError)),
            this,
            SLOT(networkError(QNetworkReply::NetworkError)));

    setLimit(limit);
    setOffset(offset);
    setViewport(viewport);
}

QGeoSearchReplyNokia::~QGeoSearchReplyNokia()
{
    //TODO: possible mem leak -> m_reply->deleteLater() ?
}

void QGeoSearchReplyNokia::abort()
{
    if (!m_reply)
        return;

    m_reply->abort();

    m_reply->deleteLater();
    m_reply = 0;
}

void QGeoSearchReplyNokia::networkFinished()
{
    if (!m_reply)
        return;

    if (m_reply->error() != QNetworkReply::NoError) {
        // Removed because this is already done in networkError, which previously caused _two_ errors to be raised for every error.
        //setError(QGeoSearchReply::CommunicationError, m_reply->errorString());
        //m_reply->deleteLater();
        //m_reply = 0;
        return;
    }

    QGeoCodeXmlParser parser;
    if (parser.parse(m_reply)) {
        QList<QGeoLocation> locations = parser.results();
        QGeoBoundingArea *bounds = viewport();
        if (bounds) {
            for (int i = locations.size() - 1; i >= 0; --i) {
                if (!bounds->contains(locations[i].coordinate()))
                    locations.removeAt(i);
            }
        }
        setLocations(locations);
        setFinished(true);
    } else {
        setError(QGeoSearchReply::ParseError, parser.errorString());
    }

    m_reply->deleteLater();
    m_reply = 0;
}

void QGeoSearchReplyNokia::networkError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)

    if (!m_reply)
        return;

    setError(QGeoSearchReply::CommunicationError, m_reply->errorString());

    m_reply->deleteLater();
    m_reply = 0;
}

QT_END_NAMESPACE
