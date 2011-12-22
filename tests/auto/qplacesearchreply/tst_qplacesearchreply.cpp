/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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
****************************************************************************/

#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QtLocation/QPlaceSearchReply>
#include <QtLocation/QGeoBoundingCircle>

QT_USE_NAMESPACE

class TestSearchReply : public QPlaceSearchReply
{
    Q_OBJECT
public:
    TestSearchReply(QObject *parent) : QPlaceSearchReply(parent) {}
    TestSearchReply() {}

    void setResults(const QList<QPlaceSearchResult> &results) {
        QPlaceSearchReply::setResults(results);
    }

    void setRequest(const QPlaceSearchRequest &request) {
        QPlaceSearchReply::setRequest(request);
    }
};

class tst_QPlaceSearchReply : public QObject
{
    Q_OBJECT

public:
    tst_QPlaceSearchReply();

private Q_SLOTS:
    void constructorTest();
    void typeTest();
    void requestTest();
    void resultsTest();
};

tst_QPlaceSearchReply::tst_QPlaceSearchReply()
{
}

void tst_QPlaceSearchReply::constructorTest()
{
    TestSearchReply *reply = new TestSearchReply(this);
    QVERIFY(reply->parent() == this);
    delete reply;
}

void tst_QPlaceSearchReply::typeTest()
{
    TestSearchReply *reply = new TestSearchReply(this);
    QVERIFY(reply->type() == QPlaceReply::SearchReply);
    delete reply;
}

void tst_QPlaceSearchReply::requestTest()
{
    TestSearchReply *reply = new TestSearchReply(this);
    QPlaceSearchRequest request;
    request.setLimit(10);
    request.setOffset(50);

    QGeoBoundingCircle *circle = new QGeoBoundingCircle();
    circle->setCenter(QGeoCoordinate(10,20));
    request.setSearchArea(circle);

    request.setSearchTerm("pizza");

    reply->setRequest(request);
    QCOMPARE(reply->request(), request);
    reply->setRequest(QPlaceSearchRequest());
    QCOMPARE(reply->request(), QPlaceSearchRequest());
    delete reply;
}

void tst_QPlaceSearchReply::resultsTest()
{
    TestSearchReply *reply = new TestSearchReply(this);
    QList<QPlaceSearchResult> results;
    QPlace winterfell;
    winterfell.setName("Winterfell");
    QPlace casterlyRock;
    casterlyRock.setName("Casterly Rock");
    QPlace stormsEnd;
    stormsEnd.setName("Storm's end");

    QPlaceSearchResult result1;
    result1.setPlace(winterfell);
    QPlaceSearchResult result2;
    result2.setPlace(casterlyRock);
    QPlaceSearchResult result3;
    result3.setPlace(stormsEnd);
    results << result1 << result2 << result3;

    reply->setResults(results);
    QCOMPARE(reply->results(), results);
    reply->setResults(QList<QPlaceSearchResult>());
    QCOMPARE(reply->results(), QList<QPlaceSearchResult>());
    delete reply;
}

QTEST_APPLESS_MAIN(tst_QPlaceSearchReply)

#include "tst_qplacesearchreply.moc"