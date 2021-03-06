#include "drawtrack.h"

DrawTrack::DrawTrack(QQuickItem *parent):
    m_originPoint(0,0),
    m_zoom(8),
    m_widthPath(2),
    m_colorPath(255,0,0),
    m_isCenteredLastPoint(false),
    m_msecUpdate(100),
    m_isPaintAxis(true),
    m_shiftCord(0,0),
    m_isUpdateFromChanged(true),
    m_colorGround(200, 255, 200),
    m_colorKeyPoint(0, 0, 255),
    m_lengthPath(100),
    m_widthFlat(1),
    m_colorFlat(255, 255, 0)
{
    internalTimer = new QTimer(this);
    connect(internalTimer, &QTimer::timeout, [=](){
        //здесь можно что-то обновить
        update();
    } );


    // связываем изменение переменной с работой таймера
    connect(this, SIGNAL(msecUpdateChanged()),
            this, SLOT(intervalChanged()) );

    // перерисовываем если изменились путь и\или ключевые точки
    connect(this, SIGNAL(pathChanged()),
            this, SLOT(checkAndUpdate()) );
    connect(this, SIGNAL(keypointChanged()),
            this, SLOT(checkAndUpdate()) );

    internalTimer->start(msecUpdate());

    // отслеживаем левую кнопку мыши
    setAcceptedMouseButtons(Qt::LeftButton);
}

void DrawTrack::paint(QPainter *painter)
{
    // заливаем фон

    painter->fillRect(            0,              0,
                      this->width(), this->height(),
                      colorGround() );
    drawAxis(painter);
    drawPath(painter);
    drawParallelLines(painter);
    drawKeypoint(painter);
    drawMouseEvent(painter);
}

void DrawTrack::swapCentered()
{
    setIsCenteredLastPoint(!isCenteredLastPoint());
    qDebug() << "current Centered" << isCenteredLastPoint();
}

void DrawTrack::appPointToPath(const QVector2D vec)
{
    this->m_path.append(vec);

    if(this->m_path.size() > m_lengthPath) {
        this->m_path.removeFirst();
    }

    pathToPaintedPath();

    emit pathChanged();
}

void DrawTrack::appPointToPathAndRemoveFirst(const QVector2D vec)
{
    this->m_path.append(vec);
    this->m_path.removeFirst();

    pathToPaintedPath();

    emit pathChanged();
}

void DrawTrack::updateKeyPoint(const ListVector points)
{
    //qDebug() << "keypoints ---> " << m_keypoint;
    //setKeypoint(points);

    m_keypoint = points;

    emit keypointChanged();
}

void DrawTrack::drawAxis(QPainter *painter)
{
    if(!m_isPaintAxis) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

    float marginAxis=5;

    QPen penLine{Qt::NoBrush, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setCosmetic(true);
    penLine.setColor(QColor(0,0,0));//черные оси

    painter->save();
    painter->translate( this->width()/2, this->height()/2 );

    painter->setPen(penLine);

    painter->drawLine(QPoint(marginAxis - this->width()/2 , 0 ),//X
                      QPoint(this->width()/2 - marginAxis , 0));

    painter->drawLine(QPoint(0 , marginAxis - this->height()/2 ),//Y
                      QPoint(0 , this->height()/2 - marginAxis));

    QPointF n = QPointF(0, 0);

    auto paintEllipse = [&](qreal radius) {
        painter->drawEllipse(n, radius*m_zoom, radius*m_zoom);
        painter->drawText(QPointF{radius*m_zoom + 1, -5}, QString::number(radius));
    };

    paintEllipse(1);
    paintEllipse(5);
    paintEllipse(10);
    paintEllipse(25);
    paintEllipse(50);
    paintEllipse(100);
    paintEllipse(200);
    paintEllipse(400);


    painter->restore();
}

void DrawTrack::drawPath(QPainter *painter)
{
    qDebug() << "path.length" << pathForDraw.length();

    if(pathForDraw.isEmpty()) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

    //цвет центральной линии - траектории
    //QPen penLine{Qt::NoBrush, m_widthPath/m_zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    QPen penLine{Qt::NoBrush, m_widthPath, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setCosmetic(true);
    penLine.setColor(m_colorPath);

    // цвет пройденой площади
    //Qt::RoundJoin/Qt::MiterJoin/Qt::BevelJoin
    //Qt::FlatCap/Qt::SquareCap
    QBrush brush(Qt::SolidPattern);
    brush.setColor(m_colorFlat);
    QPen penFlat{brush, m_widthFlat, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin};

    painter->save();

    painter->translate( this->width()/2 - m_shiftCord.x()*m_zoom,
                        this->height()/2 + m_shiftCord.y()*m_zoom);

    painter->scale(m_zoom, -m_zoom);//отражаем ось Y

    if(m_widthFlat != 0) {
        painter->setPen(penFlat);///////////////
        painter->drawPath(pathForDraw);///////////
    }

    painter->setPen(penLine);
    painter->drawPath(pathForDraw);

    painter->restore();
}

void DrawTrack::drawKeypoint(QPainter *painter)
{
    if(m_keypoint.isEmpty()) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

    auto paintEllipse = [&]( QPointF pos, qreal radius, int num) {
        Q_UNUSED(num)
        painter->drawEllipse(pos, radius, radius);
//        painter->drawText(QPointF{pos.x() - 0.1*radius,
//                                  pos.y() + 0.5*radius}, QString::number(num));
    };

    //QPen penLine{Qt::NoBrush, 2.0/m_zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    QPen penLine{Qt::NoBrush, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setCosmetic(true);
    penLine.setColor(colorKeyPoint());
    painter->setPen(penLine);
    painter->save();

    painter->translate( this->width()/2 - m_shiftCord.x()*m_zoom,
                        this->height()/2 + m_shiftCord.y()*m_zoom);

    painter->scale(m_zoom, -m_zoom);//отражаем ось Y


    float r = 2;
    int count = 0;
    for (auto p: qAsConst(m_keypoint)) {
        //painter->drawEllipse(p.toPointF(), r, r);
        paintEllipse(p.toPointF(), r, count++);
    }

    painter->restore();
}

void DrawTrack::drawMouseEvent(QPainter *painter)
{
    if(!isPressed) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }

    QPen penLine{Qt::NoBrush, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penLine.setCosmetic(true);
    penLine.setColor(QColor(255,0,255));
    painter->save();
    painter->setPen(penLine);

    float r = 20;
    painter->drawEllipse(mouseEvent, r, r);
    painter->drawEllipse(mouseEvent, 2*r, 2*r);

    painter->restore();
}

void DrawTrack::drawParallelLines(QPainter *painter)
{
    if(m_path.isEmpty()) {
        return;
    }
    if(painter == nullptr) {
        return;
    }
    if(!painter->isActive()) {
        return;
    }
    if(dir.isNull()) {
        return;
    }


    QPen penCenterLine{Qt::NoBrush, 2.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    penCenterLine.setCosmetic(true);
    penCenterLine.setColor(QColor(0,0,0));
    QPen penPerepheriaLine{Qt::NoBrush, 1.5, Qt::PenStyle::DotLine, Qt::RoundCap, Qt::RoundJoin};
    penPerepheriaLine.setCosmetic(true);
    penPerepheriaLine.setColor(QColor(0,0,0));

    float lenLine = 1000;
    int numLines = 5;




    QVector2D dist = m_path.last() - m_originPoint-pointA;
    float p = QVector2D::dotProduct(dist, orthoDir)/QVector2D::dotProduct(orthoDir, orthoDir);
    int k = rint(p);

    //QVector2D sift = pointA + k*orthoDir - m_shiftCord;


    painter->save();
    painter->setPen(penCenterLine);
    painter->translate( this->width()/2 - m_shiftCord.x()*m_zoom,
                        this->height()/2 + m_shiftCord.y()*m_zoom);

    painter->scale(m_zoom, -m_zoom);//отражаем ось Y


//    painter->drawLine(( pointA + k*orthoDir).toPointF(),////////////////////////////
//                      ( pointA).toPointF());////////////////////////////////////////
//    painter->drawLine(( m_path.last() - m_originPoint).toPointF(),////////////////////////////
//                      ( pointA).toPointF());////////////////////////////////////////

    painter->drawLine(( pointA + k*orthoDir + dir.normalized()*lenLine).toPointF(),
                      ( pointA + k*orthoDir - dir.normalized()*lenLine).toPointF());

    painter->setPen(penPerepheriaLine);

    for(int i = 1; i< numLines; i++) {
        painter->drawLine(( pointA + (k-i)*orthoDir + dir.normalized()*lenLine).toPointF(),
                          ( pointA + (k-i)*orthoDir - dir.normalized()*lenLine).toPointF());
        painter->drawLine(( pointA + (k+i)*orthoDir + dir.normalized()*lenLine).toPointF(),
                          ( pointA + (k+i)*orthoDir - dir.normalized()*lenLine).toPointF());
    }


//    painter->drawLine(QPoint(this->width()/2 , 0 ),
//                      QPoint(this->width(), 22));

    painter->restore();
}

void DrawTrack::pathToPaintedPath()
{
    pathForDraw.clear();

    if(m_path.isEmpty()) {
        return;
    }

    if(m_isCenteredLastPoint) {
        m_shiftCord = m_path.last() - m_originPoint;
    } else {
        m_shiftCord = {0,0};
    }

    pathForDraw.moveTo((m_path.first() - m_originPoint).toPointF());
    for(auto p: m_path) {
        pathForDraw.lineTo((p - m_originPoint).toPointF());
    }
}

qreal DrawTrack::zoom() const
{
    return m_zoom;
}

void DrawTrack::setZoom(qreal newZoom)
{
    if (qFuzzyCompare(0, newZoom))
        return;
    if (qFuzzyCompare(m_zoom, newZoom))
        return;
    if(newZoom > 128)
        return;
    if(newZoom < 0.01)
        return;
    m_zoom = newZoom;
    emit zoomChanged();
}

void DrawTrack::zoomIn()
{
    setZoom(zoom()*2);
}

void DrawTrack::zoomOut()
{
    setZoom(zoom()*0.5);
}

QString DrawTrack::getZoom()
{
    return QString::number(zoom(),'g',3);
}

void DrawTrack::setPointA(const QVector2D point)
{
    if(point.isNull()) {
        return;
    }
    pointA = point;
}

void DrawTrack::setDirect(const QVector2D dir)
{
    if(dir.isNull()) {
        return;
    }
    this->dir = dir;
    orthoDir = {this->dir.y(), - this->dir.x()};
}

qreal DrawTrack::widthPath() const
{
    return m_widthPath;
}

void DrawTrack::setWidthPath(qreal newWidthPath)
{
    if (qFuzzyCompare(m_widthPath, newWidthPath))
        return;
    m_widthPath = newWidthPath;
    emit widthPathChanged();
}

const QColor &DrawTrack::colorPath() const
{
    return m_colorPath;
}

void DrawTrack::setColorPath(const QColor &newColorPath)
{
    if (m_colorPath == newColorPath)
        return;
    m_colorPath = newColorPath;
    emit colorPathChanged();
}

bool DrawTrack::isCenteredLastPoint() const
{
    return m_isCenteredLastPoint;
}

void DrawTrack::setIsCenteredLastPoint(bool newIsCenteredLastPoint)
{
    if (m_isCenteredLastPoint == newIsCenteredLastPoint)
        return;
    m_isCenteredLastPoint = newIsCenteredLastPoint;
    emit isCenteredLastPointChanged();
}

qreal DrawTrack::msecUpdate() const
{
    return m_msecUpdate;
}

void DrawTrack::setMsecUpdate(qreal newMsecUpdate)
{
    if (qFuzzyCompare(m_msecUpdate, newMsecUpdate))
        return;
    m_msecUpdate = newMsecUpdate;
    emit msecUpdateChanged();
}

bool DrawTrack::isPaintAxis() const
{
    return m_isPaintAxis;
}

void DrawTrack::setIsPaintAxis(bool newIsPaintAxis)
{
    if (m_isPaintAxis == newIsPaintAxis)
        return;
    m_isPaintAxis = newIsPaintAxis;
    emit isPaintAxisChanged();
}

void DrawTrack::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        isPressed = true;
        mouseEvent = event->localPos();
        update();
    }
}

void DrawTrack::mouseMoveEvent(QMouseEvent *event)
{
    if(isPressed) {
        mouseEvent = event->localPos();
        update();
    }
}

void DrawTrack::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        //qDebug() <<" coord:" << event->pos();
        isPressed = false;
        mouseEvent = event->localPos();

        QVector2D p;
        p.setX( (mouseEvent.x() - this->width() /2.0)/m_zoom  + m_shiftCord.x());
        p.setY(-(mouseEvent.y() - this->height()/2.0)/m_zoom  + m_shiftCord.y());//отражаем ось Y

        emit releaseCoordinate(p);

        update();
    }
}

void DrawTrack::intervalChanged()
{
    if(internalTimer == nullptr) {
        return;
    }
    internalTimer->setInterval(msecUpdate());
}

void DrawTrack::checkAndUpdate()
{
    if(m_isUpdateFromChanged) {
        update();
    }
}

bool DrawTrack::isUpdateFromChanged() const
{
    return m_isUpdateFromChanged;
}

void DrawTrack::setIsUpdateFromChanged(bool newIsUpdateFromChanged)
{
    if (m_isUpdateFromChanged == newIsUpdateFromChanged)
        return;
    m_isUpdateFromChanged = newIsUpdateFromChanged;
    emit isUpdateFromChangedChanged();
}

const QColor &DrawTrack::colorGround() const
{
    return m_colorGround;
}

void DrawTrack::setColorGround(const QColor &newColorGround)
{
    if (m_colorGround == newColorGround)
        return;
    m_colorGround = newColorGround;
    emit colorGroundChanged();
}

const QColor &DrawTrack::colorKeyPoint() const
{
    return m_colorKeyPoint;
}

void DrawTrack::setColorKeyPoint(const QColor &newColorKeyPoint)
{
    if (m_colorKeyPoint == newColorKeyPoint)
        return;
    m_colorKeyPoint = newColorKeyPoint;
    emit colorKeyPointChanged();
}

qreal DrawTrack::lengthPath() const
{
    return m_lengthPath;
}

void DrawTrack::setLengthPath(qreal newLengthPath)
{
    if (qFuzzyCompare(m_lengthPath, newLengthPath))
        return;
    m_lengthPath = newLengthPath;
    emit lengthPathChanged();
}

qreal DrawTrack::widthFlat() const
{
    return m_widthFlat;
}

void DrawTrack::setWidthFlat(qreal newWidthFlat)
{
    if (qFuzzyCompare(m_widthFlat, newWidthFlat))
        return;
    m_widthFlat = newWidthFlat;
    emit widthFlatChanged();
}

const QColor &DrawTrack::colorFlat() const
{
    return m_colorFlat;
}

void DrawTrack::setColorFlat(const QColor &newColorFlat)
{
    if (m_colorFlat == newColorFlat)
        return;
    m_colorFlat = newColorFlat;
    emit colorFlatChanged();
}
