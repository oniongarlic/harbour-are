#ifndef MAPWIDGET_H
#define MAPWIDGET_H

/*
 OSMScout - a Qt backend for libosmscout and libosmscout-map
 Copyright (C) 2010  Tim Teulings

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <QQuickPaintedItem>

#include "DBThread.h"
#include "SearchLocationModel.h"

class MapWidget : public QQuickPaintedItem
{
  Q_OBJECT
  Q_PROPERTY(double lat READ GetLat NOTIFY latChanged)
  Q_PROPERTY(double lon READ GetLon NOTIFY lonChanged)
  Q_PROPERTY(int zoom READ GetZoom NOTIFY zoomChanged)
  Q_PROPERTY(double angle READ GetAngle WRITE setAngle NOTIFY angleChanged)

private:
  double                       lon;
  double                       lat;
  double                       angle;
  osmscout::Magnification      magnification;
  size_t      min_magnification;

  // Drag and drop
  int                          startX;
  int                          startY;
  osmscout::MercatorProjection startProjection;

  // Controlling rerendering...
  bool                          requestNewMap;

signals:
  void TriggerMapRenderingSignal();
  void latChanged();
  void lonChanged();
  void zoomChanged();
  void angleChanged();

public slots:
  void initialisationFinished(const DatabaseLoadedResponse& response);
  void redraw();
  Q_INVOKABLE void zoomIn(double zoomFactor);
  Q_INVOKABLE void zoomOut(double zoomFactor);
  Q_INVOKABLE void left();
  Q_INVOKABLE void right();
  Q_INVOKABLE void up();
  Q_INVOKABLE void down();
  Q_INVOKABLE void rotateLeft();
  Q_INVOKABLE void rotateRight();
  Q_INVOKABLE void showCoordinates(double lat, double lon);
  void showLocation(Location* location);

private:
  void TriggerMapRendering();
  void HandleMouseMove(QMouseEvent* event);

public:
  MapWidget(QQuickItem* parent = 0);
  virtual ~MapWidget();

  inline double GetLat() const
  {
      return lat;
  }

  inline double GetLon() const
  {
      return lon;
  }

  inline int GetZoom() const
  {
      //return (int)magnification;
      return 1;
  }

  inline double GetAngle() const
  {
      return angle;
  }
  Q_INVOKABLE void setAngle(double nangle);

  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);
  void paint(QPainter *painter);  
};

#endif
