/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
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
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeocodingmanager.h"
#include "qgeocodingmanager_p.h"
#include "qgeocodingmanagerengine.h"

#include "qgeorectangle.h"
#include "qgeocircle.h"

#include <QLocale>

QT_BEGIN_NAMESPACE

/*!
    \class QGeocodingManager
    \inmodule QtLocation
    \ingroup QtLocation-geocoding
    \since Qt Location 5.0

    \brief The QGeocodingManager class provides support for geocoding
    operations.

    The geocode() and reverseGeocode() functions return
    QGeocodeReply objects, which manage these operations and report on the
    result of the operations and any errors which may have occurred.

    The geocode() and reverseGeocode() functions can be used to convert
    QGeoAddress instances to QGeoCoordinate instances and vice-versa.

    The geocode() function is also overloaded to allow a user to perform a free text
    geocoding operation, if the string provided can be interpreted as
    an address it can be geocoded to coordinate information.

    Instances of QGeocodingManager can be accessed with
    QGeoServiceProvider::geocodingManager().
*/

/*!
    Constructs a new manager with the specified \a parent and with the
    implementation provided by \a engine.

    This constructor is used interally by QGeoServiceProviderFactory. Regular
    users should acquire instances of QGeocodingManager with
    QGeoServiceProvider::geocodingManager();
*/
QGeocodingManager::QGeocodingManager(QGeocodingManagerEngine *engine, QObject *parent)
    : QObject(parent),
      d_ptr(new QGeocodingManagerPrivate())
{
    d_ptr->engine = engine;
    if (d_ptr->engine) {
        d_ptr->engine->setParent(this);

        connect(d_ptr->engine,
                SIGNAL(finished(QGeocodeReply*)),
                this,
                SIGNAL(finished(QGeocodeReply*)));

        connect(d_ptr->engine,
                SIGNAL(error(QGeocodeReply*, QGeocodeReply::Error, QString)),
                this,
                SIGNAL(error(QGeocodeReply*, QGeocodeReply::Error, QString)));
    } else {
        qFatal("The geocoding manager engine that was set for this geocoding manager was NULL.");
    }
}

/*!
    Destroys this manager.
*/
QGeocodingManager::~QGeocodingManager()
{
    delete d_ptr;
}

/*!
    Returns the name of the engine which implements the behaviour of this
    geocoding manager.

    The combination of managerName() and managerVersion() should be unique
    amongst the plugin implementations.
*/
QString QGeocodingManager::managerName() const
{
//    if (!d_ptr->engine)
//        return QString();

    return d_ptr->engine->managerName();
}

/*!
    Returns the version of the engine which implements the behaviour of this
    geocoding manager.

    The combination of managerName() and managerVersion() should be unique
    amongst the plugin implementations.
*/
int QGeocodingManager::managerVersion() const
{
//    if (!d_ptr->engine)
//        return -1;

    return d_ptr->engine->managerVersion();
}

/*!
    Begins the geocoding of \a address. Geocoding is the process of finding a
    coordinate that corresponds to a given address.

    A QGeocodeReply object will be returned, which can be used to manage the
    geocoding operation and to return the results of the operation.

    This manager and the returned QGeocodeReply object will emit signals
    indicating if the operation completes or if errors occur.

    If supportsGeocoding() returns false an
    QGeocodeReply::UnsupportedOptionError will occur.

    Once the operation has completed, QGeocodeReply::locations() can be used to
    retrieve the results, which will consist of a list of QGeoLocation objects.
    These objects represent a combination of coordinate and address data.

    The address data returned in the results may be different from \a address.
    This will usually occur if the geocoding service backend uses a different
    canonical form of addresses or if \a address was only partially filled out.

    If \a bounds is non-null and is a valid QGeoShape it will be used to
    limit the results to those that are contained within \a bounds. This is
    particularly useful if \a address is only partially filled out, as the
    service will attempt to geocode all matches for the specified data.

    The user is responsible for deleting the returned reply object, although
    this can be done in the slot connected to QGeocodingManager::finished(),
    QGeocodingManager::error(), QGeocodeReply::finished() or
    QGeocodeReply::error() with deleteLater().
*/
QGeocodeReply *QGeocodingManager::geocode(const QGeoAddress &address, const QGeoShape &bounds)
{
//    if (!d_ptr->engine)
//        return new QGeocodeReply(QGeocodeReply::EngineNotSetError, "The geocoding manager was not created with a valid engine.", this);

    return d_ptr->engine->geocode(address, bounds);
}


/*!
    Begins the reverse geocoding of \a coordinate. Reverse geocoding is the
    process of finding an address that corresponds to a given coordinate.

    A QGeocodeReply object will be returned, which can be used to manage the
    reverse geocoding operation and to return the results of the operation.

    This manager and the returned QGeocodeReply object will emit signals
    indicating if the operation completes or if errors occur.

    If supportsReverseGeocoding() returns false an
    QGeocodeReply::UnsupportedOptionError will occur.

    At that point QGeocodeReply::locations() can be used to retrieve the
    results, which will consist of a list of QGeoLocation objects. These objects
    represent a combination of coordinate and address data.

    The coordinate data returned in the results may be different from \a
    coordinate. This will usually occur if the reverse geocoding service
    backend shifts the coordinates to be closer to the matching addresses, or
    if the backend returns results at multiple levels of detail.

    If multiple results are returned by the reverse geocoding service backend
    they will be provided in order of specificity. This normally occurs if the
    backend is configured to reverse geocode across multiple levels of detail.
    As an example, some services will return address and coordinate pairs for
    the street address, the city, the state and the country.

    If \a bounds is non-null and a valid QGeoRectangle it will be used to
    limit the results to those that are contained within \a bounds.

    The user is responsible for deleting the returned reply object, although
    this can be done in the slot connected to QGeocodingManager::finished(),
    QGeocodingManager::error(), QGeocodeReply::finished() or
    QGeocodeReply::error() with deleteLater().
*/
QGeocodeReply *QGeocodingManager::reverseGeocode(const QGeoCoordinate &coordinate, const QGeoShape &bounds)
{
//    if (!d_ptr->engine)
//        return new QGeocodeReply(QGeocodeReply::EngineNotSetError, "The geocoding manager was not created with a valid engine.", this);

    return d_ptr->engine->reverseGeocode(coordinate, bounds);
}

/*!
    Begins geocoding for a location matching \a address.

    A QGeocodeReply object will be returned, which can be used to manage the
    geocoding operation and to return the results of the operation.

    This manager and the returned QGeocodeReply object will emit signals
    indicating if the operation completes or if errors occur.

    Once the operation has completed, QGeocodeReply::locations() can be used to
    retrieve the results, which will consist of a list of QGeoLocation objects.
    These objects represent a combination of coordinate and address data.

    If \a limit is -1 the entire result set will be returned, otherwise at most
    \a limit results will be returned.

    The \a offset parameter is used to ask the geocoding service to not return the
    first \a offset results.

    The \a limit and \a offset results are used together to implement paging.

    If \a bounds is non-null and a valid QGeoShape it will be used to
    limit the results to those that are contained within \a bounds.

    The user is responsible for deleting the returned reply object, although
    this can be done in the slot connected to QGeocodingManager::finished(),
    QGeocodingManager::error(), QGeocodeReply::finished() or
    QGeocodeReply::error() with deleteLater().
*/
QGeocodeReply *QGeocodingManager::geocode(const QString &address,
        int limit,
        int offset,
        const QGeoShape &bounds)
{
//    if (!d_ptr->engine)
//        return new QGeocodeReply(QGeocodeReply::EngineNotSetError, "The geocoding manager was not created with a valid engine.", this);

    QGeocodeReply *reply = d_ptr->engine->geocode(address,
                             limit,
                             offset,
                             bounds);
    return reply;
}

/*!
    Sets the locale to be used by this manager to \a locale.

    If this geocoding manager supports returning the results
    in different languages, they will be returned in the language of \a locale.

    The locale used defaults to the system locale if this is not set.
*/
void QGeocodingManager::setLocale(const QLocale &locale)
{
    d_ptr->engine->setLocale(locale);
}

/*!
    Returns the locale used to hint to this geocoding manager about what
    language to use for the results.
*/
QLocale QGeocodingManager::locale() const
{
    return d_ptr->engine->locale();
}

/*!
\fn void QGeocodingManager::finished(QGeocodeReply *reply)

    This signal is emitted when \a reply has finished processing.

    If reply::error() equals QGeocodeReply::NoError then the processing
    finished successfully.

    This signal and QGeocodeReply::finished() will be emitted at the same
    time.

    \note Do not delete the \a reply object in the slot connected to this
    signal. Use deleteLater() instead.
*/

/*!
\fn void QGeocodingManager::error(QGeocodeReply *reply, QGeocodeReply::Error error, QString errorString)

    This signal is emitted when an error has been detected in the processing of
    \a reply. The QGeocodingManager::finished() signal will probably follow.

    The error will be described by the error code \a error. If \a errorString is
    not empty it will contain a textual description of the error.

    This signal and QGeocodeReply::error() will be emitted at the same time.

    \note Do not delete the \a reply object in the slot connected to this
    signal. Use deleteLater() instead.
*/

/*******************************************************************************
*******************************************************************************/

QGeocodingManagerPrivate::QGeocodingManagerPrivate()
    : engine(0) {}

QGeocodingManagerPrivate::~QGeocodingManagerPrivate()
{
    delete engine;
}

/*******************************************************************************
*******************************************************************************/

#include "moc_qgeocodingmanager.cpp"

QT_END_NAMESPACE
