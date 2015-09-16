#ifndef POSITIONSOURCE_H
#define POSITIONSOURCE_H

#include <QObject>

class PositionSource : public QObject
{
    Q_OBJECT
public:
    explicit PositionSource(QObject *parent = 0);

signals:

public slots:

};

#endif // POSITIONSOURCE_H
