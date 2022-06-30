#ifndef DRAWTRACK_H
#define DRAWTRACK_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QPainterPath>
#include <QtMath>

typedef QList<QVector2D> ListVector;

class DrawTrack : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(qreal  msecUpdate  READ msecUpdate  WRITE setMsecUpdate  NOTIFY msecUpdateChanged)
    Q_PROPERTY(qreal  zoom        READ zoom        WRITE setZoom        NOTIFY zoomChanged)
    Q_PROPERTY(qreal  widthPath   READ widthPath   WRITE setWidthPath   NOTIFY widthPathChanged)
    Q_PROPERTY(qreal  lengthPath  READ lengthPath  WRITE setLengthPath  NOTIFY lengthPathChanged)
    Q_PROPERTY(qreal  widthFlat   READ widthFlat   WRITE setWidthFlat   NOTIFY widthFlatChanged)
    Q_PROPERTY(QColor colorFlat   READ colorFlat   WRITE setColorFlat   NOTIFY colorFlatChanged)
    Q_PROPERTY(QColor colorPath   READ colorPath   WRITE setColorPath   NOTIFY colorPathChanged)
    Q_PROPERTY(QColor colorKeyPoint READ colorKeyPoint WRITE setColorKeyPoint NOTIFY colorKeyPointChanged)
    Q_PROPERTY(QColor colorGround READ colorGround WRITE setColorGround NOTIFY colorGroundChanged)
    Q_PROPERTY(bool   isPaintAxis READ isPaintAxis WRITE setIsPaintAxis NOTIFY isPaintAxisChanged)
    Q_PROPERTY(bool   isCenteredLastPoint READ isCenteredLastPoint WRITE setIsCenteredLastPoint NOTIFY isCenteredLastPointChanged)
    Q_PROPERTY(bool   isUpdateFromChanged READ isUpdateFromChanged WRITE setIsUpdateFromChanged NOTIFY isUpdateFromChangedChanged)

public:
    explicit DrawTrack(QQuickItem *parent = 0);

    void paint(QPainter *painter) override;

    /// Переключается с вида "на устройстве" на вид "в центре координат"
    /// и обратно.
    Q_INVOKABLE void swapCentered();

    /// Добавляет точку в конец отрисовываемого пути.
    Q_INVOKABLE void appPointToPath(const QVector2D vec);
    /// Добавляет точку в конец отрисовываемого пути и удаляет
    /// первую точку пути.
    Q_INVOKABLE void appPointToPathAndRemoveFirst(const QVector2D vec);
    /// перезаписывает набор отрисовываемых ключевых точек.
    Q_INVOKABLE void updateKeyPoint(const ListVector points);

    /// Увеличивает приближение.
    Q_INVOKABLE void zoomIn();
    /// Уменьшает приближение.
    Q_INVOKABLE void zoomOut();
    /// Возвращает текущий уровень приближения в формате строки.
    Q_INVOKABLE QString getZoom();

    /// Устанавливает точку А (для отрисовки параллельных линий).
    Q_INVOKABLE void setPointA(const QVector2D point);
    /// Устанавливает направление параллельных линий.
    Q_INVOKABLE void setDirect(const QVector2D dir);


    /// Возвращает текущий уровень приближения в виде числа.
    qreal zoom() const;
    /// Устанавливает уровень текущего приближения.
    void setZoom(qreal newZoom);

    /// Возвращает ширину центральной линии пройденного пути.
    qreal widthPath() const;
    /// Устанавливает ширину центральной линии пройденного пути.
    void setWidthPath(qreal newWidthPath);

    /// Возвращает цвет центральной линии пройденного пути.
    const QColor &colorPath() const;
    /// Устанавливает цвет центральной линии пройденного пути.
    void setColorPath(const QColor &newColorPath);

    /// Возвращает значение флага центрирования на последней
    /// точке пройденного пути.
    bool isCenteredLastPoint() const;
    /// Устанавливает значение флага центрирования на последней
    /// точке пройденного пути.
    void setIsCenteredLastPoint(bool newIsCenteredLastPoint);

    /// Возвращает интервал принудительного обновления поля отрисовки.
    qreal msecUpdate() const;
    /// Устанавливает интервал принудительного обновления поля отрисовки.
    void setMsecUpdate(qreal newMsecUpdate);

    /// Возвращает значение флага рисования осей.
    bool isPaintAxis() const;
    /// Устанавливает значение флага рисования осей.
    void setIsPaintAxis(bool newIsPaintAxis);

    /// Возвращает значение флага перерисовки отображаемого поля
    /// в результате изменнения некоторых параметров.
    bool isUpdateFromChanged() const;
    /// Устанавливает значение флага перерисовки отображаемого поля
    /// в результате изменнения некоторых параметров.
    void setIsUpdateFromChanged(bool newIsUpdateFromChanged);

    /// Возвращает цвет заливки поля по умолчанию.
    const QColor &colorGround() const;
    /// Устанавливает цвет заливки поля по умолчанию.
    void setColorGround(const QColor &newColorGround);

    /// Возвращает цвет рисования ключевых точек.
    const QColor &colorKeyPoint() const;
    /// Устанавливает цвет рисования ключевых точек.
    void setColorKeyPoint(const QColor &newColorKeyPoint);

    /// Возвращает максимально допустимую длину пути.
    qreal lengthPath() const;
    /// Устанавливает максимально допустимую длину пути.
    void setLengthPath(qreal newLengthPath);

    /// Возвращает значение ширины закрашиваемой области
    /// обработанного участка.
    qreal widthFlat() const;
    /// Устанавливает значение ширины закрашиваемой области
    /// обработанного участка.
    void setWidthFlat(qreal newWidthFlat);

    /// Возвращает цвет закрашиваемой области
    /// обработанного участка.
    const QColor &colorFlat() const;
    /// Устанавливает цвет закрашиваемой области
    /// обработанного участка.
    void setColorFlat(const QColor &newColorFlat);

protected:
    // QQuickItem interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    /// Сигнал о изменении пути (добавление\удаление точек).
    void pathChanged();

    /// Сигнал о изменнии списка ключевых точек (добавление\удаление точек).
    void keypointChanged();

    /// Сигнал о изменении положения начала координат.
    void originPointChanged();

    /// Сигнал о изменении уровня приближения.
    void zoomChanged();

    /// Сигнал о изменении ширины центральной линии пройденного пути.
    void widthPathChanged();

    /// Сигнал о изменении цвета центральной линии пройденного пути.
    void colorPathChanged();

    /// Сигнал о изменении флага центрирования на последней точке пути.
    void isCenteredLastPointChanged();

    /// Сигнал о изменении интервала принудительного перерисовывания поля.
    void msecUpdateChanged();

    /// Сигнал о изменении флага отрисовывания осей.
    void isPaintAxisChanged();

    /// Сигнал передающий координаты(в проекции Меркатора) при окончании
    /// касания экрана в произвольной точке.
    void releaseCoordinate(const QVector2D &coord);

    /// Сигнал о изменнеии смещения координат отрисовывания.
    void shiftCordChanged();

    /// Сигнал о изменении флага перерисовывания поля в результате
    /// изменения некоторых величин.
    void isUpdateFromChangedChanged();

    /// Сигнал изменения цвета заливки поля.
    void colorGroundChanged();

    /// Сигнал о изменении цвета отображения ключевых точек.
    void colorKeyPointChanged();

    /// Сигнал о изменении максимальной длины отрисовываемого пути.
    void lengthPathChanged();

    /// Сигнал о изменении ширины закрашиваемой обработанной области.
    void widthFlatChanged();

    /// Сигнал о изменении цвета закрашиваемой обработанной области.
    void colorFlatChanged();



private slots:
    /// Слот изменения интервала таймера в соответствии с принятой
    /// величиной перерисовки.
    void intervalChanged();
    /// Слот проверки изменений и последующей перерисовки поля если
    /// флаг перерисовки под действием изменения некоторых величин опущен.
    void checkAndUpdate();

private:
    /// Процедура рисования осей координат с справочными эллипсами.
    void drawAxis(QPainter *painter);
    /// Процедура рисования обработанной области и траектории.
    void drawPath(QPainter *painter);
    /// Процедура рисования ключевых точек.
    void drawKeypoint(QPainter *painter);
    /// Процедура рисования точки касания дисплея.
    void drawMouseEvent(QPainter *painter);
    /// Процедура рисования параллельных линий.
    void drawParallelLines(QPainter *painter);

    /// Процедура формирования пути для отображения.
    void pathToPaintedPath();

    QTimer      *internalTimer;

    ListVector m_path;// точки пути
    ListVector m_keypoint;// точки ключевые

    // от этой точки будут вестись все расчеты
    // это локальное начало координат
    QVector2D m_originPoint;

    QPainterPath pathForDraw;// путь отрисовываемый

    qreal m_zoom;// приближение

    qreal m_widthPath;// ширина пути
    QColor m_colorPath;// цвет пути

    // центрировать ли на текущем положении
    // или на начале координат
    bool m_isCenteredLastPoint;

    qreal m_msecUpdate;// интевал обновления отрисовки

    bool m_isPaintAxis;// рисовать ли оси

    // события нажатия на экран мышью/пальцем
    bool isPressed = false;
    QPointF mouseEvent;

    // смещение центра экрана от начала координат
    // равно нулю при центрировании на начале координат
    // не равно нулю при центрировании на текущем положении
    QVector2D m_shiftCord;

    // обновление при изменении пути или клюяевых точек
    bool m_isUpdateFromChanged;

    QColor m_colorGround;// цвет фона

    QColor m_colorKeyPoint;// цвет ключевых точек
    qreal m_lengthPath;
    qreal m_widthFlat;
    QColor m_colorFlat;

    QVector2D pointA;//{12,18};//////////////
    QVector2D dir;//{6, -2};/////////////////
    QVector2D orthoDir;//{dir.y(), -dir.x()};
};

#endif // DRAWTRACK_H
