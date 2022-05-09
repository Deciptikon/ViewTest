#ifndef TYPEEDIT_H
#define TYPEEDIT_H

#include <QObject>

// ENUM для использования как в с++ так и в qml
class TypeEdit : public QObject
{
    Q_OBJECT
public:
    enum State {
        NONE_EDIT,
        BOOL_EDIT,
        DOUBLE_EDIT,
        STRING_EDIT
    };
    Q_ENUM(State)
};

#endif // TYPEEDIT_H
