#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QDebug>

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

    Q_INVOKABLE void testMetod(QString str);

signals:
    void signalSendToQML(QString str);

public slots:
    void slotTakeFromQML(QString str);

private:
    QString testData;

};

#endif // MODEL_H
