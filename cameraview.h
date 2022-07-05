#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QCamera>
#include <QCameraInfo>
//#include <QCameraViewfinder>
//#include <QCameraViewfinderSettings>
#include <QVideoProbe>
//#include <QtMath>

class CameraView : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(qreal cameraIndex READ cameraIndex WRITE setCameraIndex NOTIFY cameraIndexChanged)
    Q_PROPERTY(QImage capture READ capture NOTIFY captureChanged)

public:
    explicit CameraView(QQuickItem *parent = 0);

    void paint(QPainter *painter) override;

    const QImage &capture() const;

    qreal cameraIndex() const;
    void setCameraIndex(qreal newCameraIndex);

signals:
    void videoFrameProbed(const QVideoFrame &frame);

    void captureChanged();

    void cameraIndexChanged();

public slots:
    //

private slots:
    void frameProbeCam(const QVideoFrame &frame);

private:
    void qtConvertYV12ToARGB32(const QVideoFrame &frame, uchar *output);
    QImage frameToQImage(const QVideoFrame &frame);

    qreal m_cameraIndex;
    QImage m_capture;
    QImage imgTemp;

    bool cameraNotAvailable = true;

    QCamera *cam;//
    QVideoProbe *videoProbe;
    //QCameraViewfinder *viewfinder;
    //QCameraViewfinderSettings *settings;

};

#endif // CAMERAVIEW_H
