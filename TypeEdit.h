#ifndef TYPEEDIT_H
#define TYPEEDIT_H

#include <QObject>

class TypeEdit : public QObject
{
    Q_OBJECT
public:
    enum State {
        BOOL_EDIT,
        DOUBLE_EDIT,
        STRING_EDIT
    };
    Q_ENUM(State)
};

#endif // TYPEEDIT_H
