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

    //read from QSettings
    widthBetweenLines = 5;
}

void Autopilot::loop()
{
    //qDebug() << "void Autopilot::loop()";

    //emit signalRotateFromAutopilot(-100);

    if(path2D.size()<2) {
        return;
    }
    //update velocity, direct and translate
    //here calculate drive

    //derection = (last position - penultimate position) and normalized
    direction = path2D.last() - *(----path2D.end());


//======================================================================================
    if(currentDriveMode == DriveMode::KEYPOINTS_MODE) {
        driveKeyPoint();
    }
    if(currentDriveMode == DriveMode::PARALLEL_MODE) {
        driveParallel();
    }
    if(currentDriveMode == DriveMode::SPIRAL_MODE) {
        //driveSpiral();
    }
//======================================================================================

}

void Autopilot::readFromGPS(const double &x, const double &y)
{
    //qDebug() << "Autopilot::readFromGPS(double &x, double &y)" << x << "\t |" << y;
    if(!isOrigin) {
        xOrigin = x;
        yOrigin = y;
        qDebug() << "Set Origin Point. X:" << xOrigin << "\tY:" << yOrigin;
        isOrigin = true;
    }

    float xp = x - xOrigin;
    float yp = y - yOrigin;
    QVector2D point{xp,yp};

   // qDebug() << "accept new point" << point;

    path2D.append(point);

    // здесь можно сохранять траекторию в некоторый класс с записью в фаил
    // имеющий возможность удобно подгружать части траектории из сохраненного файла
    if(path2D.size() > 100) {
        path2D.removeFirst();
        //emit signalAppPointToPathAndRemoveFirst(point);
    } else {
        //emit signalAppPointToPath(point);
    }

    if(!pointA.isNull() && pointB.isNull()) {//если не установлена точка В, определяем её как текущую на каждом шаге
        dir = (path2D.last() - pointA).normalized() * widthBetweenLines;
        orthoDir = {dir.y(), -dir.x()};

        emit sendDirectToDraw(dir);
    }

    emit signalAppPointToPath(point);

    //emit pathChanged(path2D);
}

void Autopilot::addKeyPoint(const QVector2D &point)
{
    if(currentDriveMode == DriveMode::KEYPOINTS_MODE) {
        listPoint2D.append(point);
        emit keyPointsChanged(listPoint2D);
    }
}

void Autopilot::acceptDriveMode(const QVariant &mode)
{
    currentDriveMode = (DriveMode::State)mode.toUInt();
    qDebug() << "currentDriveMode = " << currentDriveMode;
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

void Autopilot::slotSetPointA()
{
    qDebug() << "Autopilot::slotSetPointA()";
    if(path2D.isEmpty()) {
        return;
    }
    //read from QSettings
    widthBetweenLines = 5;

    pointA = path2D.last();
    pointB = {0,0};

    emit sendPointAToDraw(pointA);

}

void Autopilot::slotSetPointB()
{
    qDebug() << "Autopilot::slotSetPointB()";
    if(path2D.isEmpty()) {
        return;
    }
    if(pointA.isNull()) {
        return;
    }
    pointB = path2D.last();

    dir = (pointB - pointA).normalized() * widthBetweenLines;
    orthoDir = {dir.y(), -dir.x()};

    emit sendDirectToDraw(dir);

}

void Autopilot::driveKeyPoint()
{
    qDebug() << "DRIVE KEYPOINT";

    if(listPoint2D.isEmpty()) {
        return;
    }

    QVector2D orthogonal{ direction.y(), -direction.x()};
    //qDebug() << "direction:" << direction;
    //qDebug() << "orthogonal:" << orthogonal;

    directionToPoint = listPoint2D.first() - path2D.last();
    //qDebug() << "directionToPoint:" << directionToPoint;

    // если растояние до ключевой точки меньше 2,
    // удаляем её и испускаем сигнал с измененным скписком ключевых точек
    if(directionToPoint.length()<2) {
        listPoint2D.removeFirst();
        emit keyPointsChanged(listPoint2D);
        return;
    }

    //projection = cos(angle between a and b) for length(a)=length(b)=1
    float projection = QVector2D::dotProduct(directionToPoint.normalized(), orthogonal.normalized());
    //qDebug() << "projectionOnOrthogonal:" << projection;

    int msec;
    int comm = 50;
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
    emit signalRotateFromAutopilot(msec);

    qDebug() << "CommandToSlave:" << comm;
    emit sendCommandToSlave14(comm);
}

void Autopilot::driveParallel()
{
    qDebug() << "DRIVE PARALLEL";

    if(pointA.isNull()) {
        return;
    }
    if(pointB.isNull()) {
        return;
    }
    if(dir.isNull()) {
        return;
    }

    int comm = 50;

    //to do

    qDebug() << "CommandToSlave:" << comm;
    emit sendCommandToSlave14(comm);
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
