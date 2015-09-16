#include "positionsource.h"
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QDebug>


typedef enum {
    GEOCLUE_POSITION_FIELDS_NONE = 0,
    GEOCLUE_POSITION_FIELDS_LATITUDE = 1 << 0,
    GEOCLUE_POSITION_FIELDS_LONGITUDE = 1 << 1,
    GEOCLUE_POSITION_FIELDS_ALTITUDE = 1 << 2
} GeocluePositionFields;

typedef enum {
    GEOCLUE_VELOCITY_FIELDS_NONE = 0,
    GEOCLUE_VELOCITY_FIELDS_SPEED = 1 << 0,
    GEOCLUE_VELOCITY_FIELDS_DIRECTION = 1 << 1,
    GEOCLUE_VELOCITY_FIELDS_CLIMB = 1 << 2
} GeoclueVelocityFields;


PositionSource::PositionSource(QObject *parent) :
    QObject(parent),
    valid(false),
    altitude(0),
    lat(0.0),
    lon(0.0)
{
    QDBusMessage msg;

    gcmInterface = new QDBusInterface("org.freedesktop.Geoclue.Master", "/org/freedesktop/Geoclue/Master", "", QDBusConnection::sessionBus(), this);
    QDBusReply<QDBusObjectPath> createReply = gcmInterface->call("Create");
    if (createReply.isValid()==false)
        qWarning() << createReply.error();
    else
        qDebug() << createReply.value().path();

    gcInterface = new QDBusInterface( "org.freedesktop.Geoclue.Master", createReply.value().path(), "", QDBusConnection::sessionBus(), this);
    if (gcInterface->isValid()==false)
        qWarning() << "GCMI" << gcInterface->lastError();
}

bool PositionSource::start() {
    QDBusMessage msg;
    bool ok;

    msg=gcInterface->call("PositionStart");
    qDebug() << "PS: " << msg;

    gpsInterface = new QDBusInterface("org.freedesktop.Geoclue.Providers.Hybris", "/org/freedesktop/Geoclue/Providers/Hybris", "", QDBusConnection::sessionBus());
    if (gpsInterface->isValid()==false)
        qWarning() << gcInterface->lastError();

    msg = gpsInterface->call("AddReference");
    if (msg.type() != QDBusMessage::ReplyMessage) {
        qWarning() << "Couldn't access Hybris : " << msg;
    }

    msg=gpsInterface->call("GetProviderInfo");
    qDebug() << msg;

    msg=gpsInterface->call("GetStatus");
    qDebug() << msg;

    ok=QDBusConnection::sessionBus().connect("org.freedesktop.Geoclue.Position", "/org/freedesktop/Geoclue/Position", "", "PositionChanged", this, SLOT(positionChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCP");

    ok=QDBusConnection::sessionBus().connect("org.freedesktop.Geoclue.Providers.Hybris", "/org/freedesktop/Geoclue/Providers/Hybris", "", "PositionChanged", this, SLOT(gcPositionChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCPH");

    ok=QDBusConnection::sessionBus().connect(QString(), QString(), QStringLiteral("org.freedesktop.Geoclue.Satellite"), QStringLiteral("SatelliteChanged"), this, SLOT(gcSatelliteChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCPS");

    ok=QDBusConnection::sessionBus().connect(QString(), QString(), QStringLiteral("org.freedesktop.Geoclue.Velocity"), QStringLiteral("VelocityChanged"), this, SLOT(gcVelocityChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCPV");

    return true;
}

PositionSource::~PositionSource() {
    QDBusMessage msg;

    msg = gpsInterface->call("RemoveReference");
    qDebug() << msg;
}

void PositionSource::gcSatelliteChanged(QDBusMessage msg) {
    QVariantList arguments = msg.arguments();

    uint timestamp = arguments.at(0).toInt();
    satellitesUsed = arguments.at(1).toInt();
    satellitesView = arguments.at(2).toInt();

    qDebug() << "*** Satellites: " << timestamp << "@ " << satellitesUsed << "/" << satellitesView;
}

void PositionSource::gcVelocityChanged(QDBusMessage msg) {
    int vflags;

    QVariantList a = msg.arguments();
    vflags=a.at(0).toInt();

    if (vflags & GEOCLUE_VELOCITY_FIELDS_SPEED)
        velocity=a.at(2).toDouble();
    else
        velocity=0.0;

    qDebug() << "Vel: " << velocity;

    emit velocityChanged(velocity);
}

void PositionSource::gcPositionChanged(QDBusMessage msg)
{
    // expecing int,int,double,double,double - flags (which data is valid), time, lat, lon, altitude
    qDebug() << "*** PositionChanged:" << msg;
    QVariantList a = msg.arguments();

    flags=a.at(0).toInt();

    valid=(flags & GEOCLUE_POSITION_FIELDS_LATITUDE) && (flags & GEOCLUE_POSITION_FIELDS_LONGITUDE);

    gpsTime=a.at(1).toInt();
    if (flags & GEOCLUE_POSITION_FIELDS_LATITUDE)
        lat=a.at(2).toDouble();

    if (flags & GEOCLUE_POSITION_FIELDS_LONGITUDE)
        lon=a.at(3).toDouble();

    if (flags & GEOCLUE_POSITION_FIELDS_ALTITUDE)
        altitude=a.at(4).toDouble();

    if (valid) {
        emit positionChanged(lat, lon);
        emit altitudeChanged(altitude);
    }

    qDebug() << "Position: " << valid;
    qDebug() << "Lat: " << lat;
    qDebug() << "Lon: " << lon;
    qDebug() << "Alt: " << altitude;
}
