import QtQuick 2.0
import Sailfish.Silica 1.0
import org.tal.gps 1.0
import "pages"

ApplicationWindow
{
    id: root

    property PositionSource gps: rootGps

    initialPage: Component {
        FirstPage {

        }
    }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    PositionSource {
        id: rootGps
    }

    Component.onCompleted: {
        rootGps.start();
    }

}


