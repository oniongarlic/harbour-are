#ifndef POSITIONSOURCE_H
#define POSITIONSOURCE_H

#include <QObject>
#include <QDBusInterface>

class PositionSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double altitude READ getAltitude NOTIFY altitudeChanged)
    Q_PROPERTY(double velocity READ getVelocity() NOTIFY velocityChanged)    

public:
    explicit PositionSource(QObject *parent = 0);    

    ~PositionSource();
    Q_INVOKABLE bool start();
    Q_INVOKABLE double getAltitude() { return altitude; }
    Q_INVOKABLE double getVelocity() { return velocity; }
    Q_INVOKABLE double getLatitude() { return lat; }
    Q_INVOKABLE double getLongitude() { return lon; }

signals:
    void altitudeChanged(double altitude);
    void velocityChanged(double velocity);
    void positionChanged(double lat, double lon);

public slots:       

protected slots:
    void gcSatelliteChanged(QDBusMessage msg);
    void gcPositionChanged(QDBusMessage msg);
    void gcVelocityChanged(QDBusMessage msg);

private:
    QDBusInterface *gpsInterface;
    QDBusInterface *gcmInterface;
    QDBusInterface *gcInterface;

    bool valid;
    double lat,lon;
    double altitude;
    double velocity;
    int flags;
    int gpsTime;
    int satellitesView;
    int satellitesUsed;
};

#endif // POSITIONSOURCE_H
