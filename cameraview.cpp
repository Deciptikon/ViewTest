#include "cameraview.h"
#include <QString>

#define CLAMP(n) (n > 255 ? 255 : (n < 0 ? 0 : n))
#define EXPAND_UV(u, v) \
    int uu = u - 128; \
    int vv = v - 128; \
    int rv = 409 * vv + 128; \
    int guv = 100 * uu + 208 * vv + 128; \
    int bu = 516 * uu + 128; \

static inline quint32 qYUVToARGB32(int y, int rv, int guv, int bu, int a = 0xff)
{
    int yy = (y - 16) * 298;
    return (a << 24)
            | CLAMP((yy + rv) >> 8) << 16
            | CLAMP((yy - guv) >> 8) << 8
            | CLAMP((yy + bu) >> 8);
}

static inline void planarYUV420_to_ARGB32(const uchar *y, int yStride,
                                          const uchar *u, int uStride,
                                          const uchar *v, int vStride,
                                          int uvPixelStride,
                                          quint32 *rgb,
                                          int width, int height)
{
    quint32 *rgb0 = rgb;
    quint32 *rgb1 = rgb + width;
    for (int j = 0; j < height; j += 2) {
        const uchar *lineY0 = y;
        const uchar *lineY1 = y + yStride;
        const uchar *lineU = u;
        const uchar *lineV = v;
        for (int i = 0; i < width; i += 2) {
            EXPAND_UV(*lineU, *lineV);
            lineU += uvPixelStride;
            lineV += uvPixelStride;
            *rgb0++ = qYUVToARGB32(*lineY0++, rv, guv, bu);
            *rgb0++ = qYUVToARGB32(*lineY0++, rv, guv, bu);
            *rgb1++ = qYUVToARGB32(*lineY1++, rv, guv, bu);
            *rgb1++ = qYUVToARGB32(*lineY1++, rv, guv, bu);
        }
        y += yStride << 1; // stride * 2
        u += uStride;
        v += vStride;
        rgb0 += width;
        rgb1 += width;
    }
}

CameraView::CameraView(QQuickItem *parent)
    //m_cameraIndex(1)
  //, ...
{

    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if(cameras.isEmpty()) {
        qDebug() << "Список доступных камер пуст";
        cameraNotAvailable = true;
        return;
    }
    if(cameras.size() < cameraIndex()) {
        qDebug() << "Камера под номером" << cameraIndex() << "отсутствует";
        cameraNotAvailable = true;
        return;
    }

    qDebug() << "Камера выбрана: " << cameras.at(cameraIndex() - 1).deviceName();
    cam = new QCamera(cameras.at(cameraIndex() - 1));
    cam->setCaptureMode(QCamera::CaptureMode::CaptureViewfinder);//CaptureVideo//CaptureViewfinder
    qDebug() << " availableCameras()" << cameras.size();

    //viewfinder = new QCameraViewfinder();

    //cam->setViewfinder(viewfinder);
    //qDebug() << " start set source 1";

    //settings = new QCameraViewfinderSettings();
    //settings->setMinimumFrameRate(1);

//    cam->setViewfinderSettings(*settings);
//    qDebug() << "set settings source 1";

    videoProbe = new QVideoProbe();//this

    if (videoProbe->setSource((QMediaObject *)cam)) {
        qDebug() << " set source 1 succeed" << videoProbe->isActive();
        qDebug() << "cam->captureMode() =" << cam->captureMode();

        QObject::connect(videoProbe, SIGNAL(videoFrameProbed(QVideoFrame)),
                         this,       SLOT(frameProbeCam(QVideoFrame)));
        cameraNotAvailable = false;


    }
    cam->start();

}

void CameraView::paint(QPainter *painter)
{
    qDebug() << "//////////////////////////////////////////////////////////////////////";
    if(painter == nullptr) {
        qDebug() << "CameraView::paint:: painter == nullptr";
        return;
    }
    painter->fillRect(            0,              0,
                      this->width(), this->height(),
                      QColor(128,128,128) );
    qDebug() << "//////////////////////////////////////////////////////////////////////";
    if(cameraNotAvailable) {

        QString text = "Camera Not Available";
        qDebug() << text;
        //painter->drawText(this->width()/2, this->height()/2, text);
        return;
    }
    qDebug() << "//////////////////////////////////////////////////////////////////////";
    if(m_capture.isNull()) {
        return;
    }
    qDebug() << "//////////////////////////////////////////////////////////////////////";

    painter->drawImage(0, 0, m_capture);
}

const QImage &CameraView::capture() const
{
    return m_capture;
}

qreal CameraView::cameraIndex() const
{
    return m_cameraIndex;
}

void CameraView::setCameraIndex(qreal newCameraIndex)
{
    if (qFuzzyCompare(m_cameraIndex, newCameraIndex))
        return;
    if(m_cameraIndex<1) {
        return;
    }
    m_cameraIndex = newCameraIndex;
    emit cameraIndexChanged();
}

void CameraView::frameProbeCam(const QVideoFrame &frame)
{
    QImage imgTemp = frameToQImage(frame);
    qDebug() << "frameProbeCam";

    if(imgTemp.isNull()) {
        qDebug() << "img is NULL: ";
        return;
    }
    if(imgTemp.sizeInBytes() == NULL){
        qDebug() << "img.sizeInBytes()==0";
        return;
    }

    //Обработка изображения-------------------------------------
    //
    //-----------------------------------------------------------

    m_capture = imgTemp.scaled(QSize(this->width(), this->height()),
                               Qt::KeepAspectRatio).copy();
    emit captureChanged();
    update();
}

void CameraView::qtConvertYV12ToARGB32(const QVideoFrame &frame, uchar *output)
{
    const uchar *plane1 = frame.bits(0); \
    const uchar *plane2 = frame.bits(1); \
    const uchar *plane3 = frame.bits(2); \
    int plane1Stride = frame.bytesPerLine(0); \
    int plane2Stride = frame.bytesPerLine(1); \
    int plane3Stride = frame.bytesPerLine(2); \
    int width = frame.width(); \
    int height = frame.height(); \

    planarYUV420_to_ARGB32(plane1, plane1Stride,
                           plane3, plane3Stride,
                           plane2, plane2Stride,
                           1,
                           reinterpret_cast<quint32*>(output),
                           width, height);
}

QImage CameraView::frameToQImage(const QVideoFrame &frame)
{
    QVideoFrame f(frame);
    QImage img(f.size(), QImage::Format_ARGB32);

    f.map(QAbstractVideoBuffer::ReadOnly);

    qtConvertYV12ToARGB32(f, (uchar *)img.bits());

    f.unmap();
    return img;
}
