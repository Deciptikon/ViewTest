#ifndef TYPEEDIT_H
#define TYPEEDIT_H

#include <QObject>

// ENUM для использования как в с++ так и в qml
class TypeEdit : public QObject
{
    Q_OBJECT
public:
    enum State {
        NONE_EDIT,    // не редактируемое поле (просто заголовок)
        BOOL_EDIT,    // читаемое и редактируемое как bool
        DOUBLE_EDIT,  // как double
        STRING_EDIT   // как string
    };
    Q_ENUM(State)
};

#endif // TYPEEDIT_H
