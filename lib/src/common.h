/*
# Copyright Ole-André Rodlie.
#
# ole.andre.rodlie@gmail.com
#
# This software is governed by the CeCILL license under French law and
# abiding by the rules of distribution of free software. You can use,
# modify and / or redistribute the software under the terms of the CeCILL
# license as circulated by CEA, CNRS and INRIA at the following URL
# "https://www.cecill.info".
#
# As a counterpart to the access to the source code and rights to
# modify and redistribute granted by the license, users are provided only
# with a limited warranty and the software's author, the holder of the
# economic rights and the subsequent licensors have only limited
# liability.
#
# In this respect, the user's attention is drawn to the associated risks
# with loading, using, modifying and / or developing or reproducing the
# software by the user in light of its specific status of free software,
# that can mean that it is complicated to manipulate, and that also
# so that it is for developers and experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to test and test the software's suitability
# Requirements in the conditions of their systems
# data to be ensured and, more generally, to use and operate
# same conditions as regards security.
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QMap>
#include <QDateTime>

#include <list>
#include <Magick++.h>

#include "tileitem.h"

#define CYAN_PROJECT_VERSION 1.0
#define CYAN_LAYER_VERSION 1.0

#define CYAN_PROJECT "cyan-project"
#define CYAN_LAYER "cyan-layer"

#define CYAN_LAYER_VISIBILITY "cyan-visibility"
#define CYAN_LAYER_OPACITY "cyan-opacity"
#define CYAN_LAYER_COMPOSE "cyan-compose"
#define CYAN_LAYER_X "cyan-layer-x"
#define CYAN_LAYER_Y "cyan-layer-y"


class Common: public QObject
{
    Q_OBJECT

public:

    enum RenderingIntent {
        UndefinedRenderingIntent,
        SaturationRenderingIntent,
        PerceptualRenderingIntent,
        AbsoluteRenderingIntent,
        RelativeRenderingIntent
    };

    struct Tile
    {
        TileItem *rect;
    };

    struct Layer
    {
        Magick::Image image;
        QMap<int, Common::Layer> layers;
        Magick::CompositeOperator composite = Magick::OverCompositeOp;
        QSize pos = QSize(0, 0);
        bool visible = true;
        double opacity = 1.0;
        QString label = QObject::tr("New Layer");
    };

    struct Canvas
    {
        Magick::Image image;
        QMap<int, Common::Layer> layers;
        QString error;
        QString warning;
        QString label = QObject::tr("New Image");
        QMap<int, Common::Tile> tiles;
        QSize tileSize;
        QColor brushColor;
        bool brushAA = true;
        Magick::LineCap brushLineCap = Magick::RoundCap;
        Magick::LineJoin brushLineJoin = Magick::MiterJoin;
        QString timestamp;
    };

    Common(QObject *parent = nullptr);

    static QString timestamp();

    static QMap<Magick::CompositeOperator, QString> compositeModes();
    static Magick::CompositeOperator compositeModeFromString(const QString &name);

    static Magick::Image compLayers(Magick::Image canvas,
                                    QMap<int, Common::Layer> layers,
                                    Magick::Geometry crop = Magick::Geometry());

    static const QString canvasWindowTitle(Magick::Image image);

    static int getDiskResource();
    static void setDiskResource(int gib);

    static int getMemoryResource();
    static void setMemoryResource(int gib);

    static void setThreadResources(int thread);

    static bool writeCanvas(Common::Canvas canvas,
                            const QString &filename,
                            Magick::CompressionType compress = Magick::LZMACompression);
    static Common::Canvas readCanvas(const QString &filename);

    static Magick::Image renderCanvasToImage(Common::Canvas canvas);
    static bool renderCanvasToFile(Common::Canvas canvas,
                            const QString &filename,
                            Magick::CompressionType compress = Magick::NoCompression,
                            QMap<QString, QString> attr = QMap<QString, QString>(),
                            QMap<QString, QString> arti = QMap<QString, QString>());

    static bool isValidCanvas(const QString &filename);

    static Common::Canvas readImage(const QString &filename);
};

#endif // COMMON_H