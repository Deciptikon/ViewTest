#include "autopilot.h"


Autopilot::Autopilot(QObject *parent) : QObject(parent)
{


}

void Autopilot::init(const int msecDeltaTime)
{
    if(msecDeltaTime > 1) {
        this->msecDeltaTime = msecDeltaTime;
    } else {
        this->msecDeltaTime = 1;
    }
}

void Autopilot::loop()
{
    qDebug() << "void Autopilot::loop()";
    //qDebug() << "threadAutopilot:" << this->thread();

    if(path2D.isEmpty()) {
        return;
    }

    if(path2D.size()<2) {
        return;
    }
    //update velocity, direct and translate
    //here calculate drive

    //derection = (last position - penultimate position) and normalized
    direction = path2D.last() - *(----path2D.end());
    QVector2D orthogonal{ direction.y(), -direction.x()};
    qDebug() << "direction:" << direction;
    qDebug() << "orthogonal:" << orthogonal;

    if(listPoint2D.isEmpty()) {
        return;
    }

    directionToPoint = listPoint2D.first() - path2D.last();
    qDebug() << "directionToPoint:" << directionToPoint;

    // если растояние до ключевой точки меньше 2,
    // удаляем её и испускаем сигнал с измененным скписком ключевых точек
    if(directionToPoint.length()<2) {
        listPoint2D.removeFirst();
        emit keyPointsChanged(listPoint2D);
        return;
    }

    //projection = cos(angle between a and b) for length(a)=length(b)=1
    float projection = QVector2D::dotProduct(directionToPoint.normalized(), orthogonal.normalized());
    qDebug() << "projectionOnOrthogonal:" << projection;

    int msec;
    int comm;
    if(abs(projection) < 0.1) {
        //поворачиваем на малый угол
        msec = (int)(projection * 5000.0);// msec from -500 to 500
        comm = (5000 + msec)/100;
    } else {
        if(projection>0) {
            //поворачиваем направо
            msec = 500;// msec from -5000 to 5000
            comm = (5000 + msec)/100;
        } else {
            //поворачиваем налево (знак минус)
            msec = -500;// msec from -5000 to 5000
            comm = (5000 + msec)/100;
        }
    }
    qDebug() << "CommandToSlave:" << comm;
    emit sendCommandToSlave14(comm);

}

void Autopilot::readFromGPS(const double &x, const double &y)
{
    //qDebug() << "Autopilot::readFromGPS(double &x, double &y)" << x << "\t |" << y;
    if(!isOrigin) {
        xOrigin = x;
        yOrigin = y;
        isOrigin = true;
    }

    float xp = x - xOrigin;
    float yp = y - yOrigin;
    QVector2D point{xp,yp};

    path2D.append(point);

    if(path2D.size() > 100) {
        path2D.removeFirst();
        emit signalAppPointToPathAndRemoveFirst(point);
    } else {
        emit signalAppPointToPath(point);
    }

    //emit pathChanged(path2D);
}

void Autopilot::addKeyPoint(const QVector2D &point)
{
    listPoint2D.append(point);
    emit keyPointsChanged(listPoint2D);
}

void Autopilot::slotCreateQuadroKeyPoint()
{
    // поворачивает вектор vec на угол angle (в радианах) против часовой
    // и возврвщвет вектор
    auto rotate = [](QVector2D vec, float angle) -> QVector2D {
        if(vec.isNull()) {
            return vec;
        }

        QVector2D ortho{ vec.y(), -vec.x()};
        QVector2D rot = vec*cos(angle) - ortho*sin(angle);

        return rot;
    };

    if(path2D.isEmpty()) {
        return;
    }
    if(direction.isNull()) {
        return;
    }

    QVector2D pos = path2D.last();//текущее положение
    float dist = 20; //растояние в метрах

    listPoint2D.clear();

    //первая точка: в 20 метрах спереди
    listPoint2D.append( pos + dist*direction.normalized() );

    //вторая точка: поворачиваем налево на 90' и едем 20 метров
    listPoint2D.append( listPoint2D.last() + dist*rotate(direction.normalized(), M_PI / 2.0) );

    //третья точка: поворачиваем налево на 90' и едем 20 метров
    listPoint2D.append( listPoint2D.last() + dist*rotate(direction.normalized(), M_PI ) );

    //четвертая точка: поворачиваем налево на 90' и едем 20 метров
    listPoint2D.append( listPoint2D.last() + dist*rotate(direction.normalized(), M_PI * 3.0/2.0) );
    //квадрат.

    emit keyPointsChanged(listPoint2D);
}

int Autopilot::getMSecDeltaTime() const
{
    return msecDeltaTime;
}

void Autopilot::restateRelay()
{
    stateRelay = !stateRelay;
    qDebug() << "Состояние реле изменено на " << stateRelay;

    int c=120;
    emit sendCommandToSlave14(c);
}
