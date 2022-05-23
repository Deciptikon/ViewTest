QT += quick sql serialport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        appsettings.cpp \
        autopilot.cpp \
        databasefield.cpp \
        drawtrack.cpp \
        fieldmodel.cpp \
        gps.cpp \
        main.cpp \
        model.cpp

RESOURCES += qml.qrc


# файлы с переводами на разные языки
TRANSLATIONS = QtLanguage_ru.ts
# кодировка
CODECFORSRC = UTF-8


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    DriveMode.h \
    TypeEdit.h \
    appsettings.h \
    autopilot.h \
    constants.h \
    databasefield.h \
    drawtrack.h \
    fieldmodel.h \
    gps.h \
    model.h
