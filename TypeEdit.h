#ifndef TYPEEDIT_H
#define TYPEEDIT_H

#include <QObject>

// ENUM для использования как в с++ так и в qml
class TypeEdit : public QObject
{
    Q_OBJECT
public:
    enum State {
        /// Не редактируемое поле (просто заголовок).
        NONE_EDIT,
        /// Читаемое и редактируемое как bool
        BOOL_EDIT,
        /// Читаемое и редактируемое как double
        DOUBLE_EDIT,
        /// Читаемое и редактируемое как string
        STRING_EDIT
    };
    Q_ENUM(State)
};

#endif // TYPEEDIT_H
