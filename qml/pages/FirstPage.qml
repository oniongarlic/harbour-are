import QtQuick 2.0
import Sailfish.Silica 1.0
import net.sf.libosmscout.map 1.0

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            width: parent.width
            // height: page.height
            spacing: Theme.paddingMedium
            PageHeader {
                title: qsTr("Åre")
            }
            Row {
                Button {
                    text: "+"
                    onClicked: map.zoomIn(1.2);
                }
                Button {
                    text: "-"
                    onClicked: map.zoomOut(1.2);
                }
            }

            Map {
                id: map
                width: parent.width
                height: width;
                // anchors.fill: parent
                focus: true
            }

            Slider {
                width: parent.width
                minimumValue: -180
                maximumValue: 180
                value: 0
                onValueChanged: {
                    map.setAngle(value);
                }
            }

            Column {
                Label { text: root.gps.altitude }
                Label { text: root.gps.velocity }

                Label { id: pos; }
                Label { id: lLon; }

                Connections {
                    target: root.gps
                    onPositionChanged: {
                        pos.text=root.gps.getLatitude()+" / "+root.gps.getLongitude();
                    }
                }
            }
        }
    }
}

