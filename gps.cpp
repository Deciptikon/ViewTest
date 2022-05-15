#include "gps.h"

GPS::GPS(QObject *parent) : QObject(parent)
{
    msecUpdate = 1000;



    //если сообщение с координатами получено - читаем из него координаты
    connect(this, SIGNAL(parseMessage()), this, SLOT(ubxParser()) );
}

void GPS::setMsecUpdate(size_t value)
{
    if(value < 0) {
        return;
    }
    if(value < 10000) {
        msecUpdate = value;
    }
}

void GPS::init()
{
    qDebug() << "threadGPS:" << this->thread();
    serial = new QSerialPort();
    serial->setPortName("COM4");//"ttyACM0"//"COM4"//"ttyUSB0"
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud38400);//Baud9600
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if(!serial->isOpen()) {
        serial->open(QIODevice::ReadWrite);
    }
    if(serial->isOpen()) {
        qDebug() << "SerialPort is open";

        checker = new QTimer();
        lastElapsedTimeUpdate = new QElapsedTimer();
        connect(checker, SIGNAL(timeout()), this, SLOT(checkAction()) );
        checker->start(msecUpdate);

        connect(serial, SIGNAL(readyRead()), this, SLOT(readPort()));
    } else {
        qDebug() << "SerialPort is not open";
        //Если порт не открыт, попытка через 100мс открыть его снова
        QTimer::singleShot(1000, this, SLOT(init()));
    }
}

void GPS::write(const QByteArray &bytes)
{
    qDebug() << "SerialPort writing";
    if(bytes.isEmpty()) {
        return;
    }
    if(!serial->isOpen()) {
        return;
    }
    if(!serial->isWritable()) {
        return;
    }

    if(*bytes.end() == '\n') {
        serial->write(bytes);
        return;
    }

    QByteArray b;
    b.append('\n');
    serial->write(b);
}

void GPS::readPort()
{
    //qDebug() << "SerialPort: SIGNAL(readyRead())";

    if(serial->bytesAvailable() == 0) {
        qDebug() << "SerialPort null data";
        return;
    }

    QByteArray bytes;
    bytes = serial->readLine();

    if(bytes.length()>0) {

        for(auto b: bytes) {
            if((uchar)b == 0xb5) {
                if(messageBuffer.size() == 36) {
                    messageCurrent.clear();
                    messageCurrent = messageBuffer;
                    emit parseMessage();
                }
                messageBuffer.clear();
            }
            messageBuffer.append(b);
        }
        emit readyRead(bytes);//сигнал прочитанной строкой
    }
}

void GPS::ubxParser()
{
    if(messageCurrent.isEmpty()) {
        return;
    }
    if(messageCurrent.size() != 36) {
        return;
    }

    uint8_t chA = 0, chB = 0;
    QByteArray data = messageCurrent;
    data.remove(34,2);// delete check summ: checkA checkB
    data.remove(0,2); // delete header: 0xB5 0x62
    //qDebug() << "Data size" << data.size();

    for(uint8_t b: data) {
        chA += b;
        chB += chA;
        //qDebug() << "  chA =" << chA << "\t chB =" << chB;
    }

    uint8_t mA = (uint8_t)messageCurrent.at(34);
    uint8_t mB = (uint8_t)messageCurrent.at(35);

    if(chA - mA || chB - mB ) {
        qDebug() << "Data invalid";
        if(gpsIsOn) {
            gpsIsOn = !gpsIsOn;
            emit gpsOff();// сигнал о "разрыве" связи
        }
        return;
    }

    if(!gpsIsOn) {
        gpsIsOn = !gpsIsOn;
        emit gpsOn();// сигнал о приёме корректных данных
    }

    //lastElapsedTimeUpdate = new QElapsedTimer();
    lastElapsedTimeUpdate->start();

    //qDebug() << "Data is valid";
    data.remove(0,4);// delete class & id &  2 bite length payload

    double lon = ((data[7] << 24) + (data[6] << 16) + (data[5] << 8) + data[4]);
    this->lon = lon * 0.0000001;
    double lat = ((data[11]<< 24) + (data[10]<< 16) + (data[9] << 8) + data[8]);
    this->lat = lat * 0.0000001;
    //qDebug() << "Latitude:" << QString::number(lat, 'd', 7) << "\tLongitude:" << QString::number(lon, 'd', 7);

    latLonToXY(this->lat, this->lon);

    qDebug() << "coord.latitude(): " << QString::number(this->lat, 'g', 9);
    qDebug() << "coord.longitude(): " << QString::number(this->lon, 'g', 9);

    emit updatePositionXY(this->x, this->y);
    emit updatePositionLatLon(this->lat, this->lon);
}

void GPS::checkAction()
{
    //QTime currTime = QTime::currentTime();
    qDebug() << "=======================================GPS::checkAction()";
    if(!gpsIsOn) {
        return;
    }
    if(lastElapsedTimeUpdate->elapsed() > 1000 ) {
        gpsIsOn = !gpsIsOn;
        emit gpsOff();// сигнал о приёме корректных данных
    }
}

void GPS::latLonToXY(double lat, double lon)
{
    if(lat>360 || lat<0) {
        qDebug() << "Latitude not range";
    }
    if(lon>MAX_LONGITUDE || lon<-MAX_LONGITUDE) {
        qDebug() << "Longitude not range";
    }

    this->y = RADIUS_EARTH*(lon + 180.0)*(M_PI/180.0);
    this->x = RADIUS_EARTH*qLn(qTan(M_PI/4 + lat*(M_PI/180.0)/2.0));
}
