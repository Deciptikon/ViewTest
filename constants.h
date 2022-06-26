#ifndef CONSTANTS_H
#define CONSTANTS_H

#define EMPTY_STRING                ""

#define ORGANIZATION_NAME           "Deciptikon"
#define ORGANIZATION_DOMAIN         "vk.com/deciptikon"
#define APPLICATION_NAME            "AgroSlave"


#define DIR_STATUSBAR               "StatusBar/" // дирректория группы настроек
#define KEY_COLOR_STATUSBAR         "ColorStatusBar" // ключ настройки
#define DEFAULT_COLOR_STATUSBAR     "#33223322" // значение по умолчанию (для сброса настроек)

#define DIR_LANGUAGE                "Language/"
#define KEY_CURRENT_LANGUAGE        "CurrentLanguage"
#define DEFAULT_CURRENT_LANGUAGE    "en"



#define DIR_GPS                     "GPS/"

#define KEY_ENABLE_GPS              "EnableGPS"
#define DEFAULT_ENABLE_GPS          "true"

#define KEY_CURRENT_GPS_PORTNAME    "PortName"
#define DEFAULT_CURRENT_GPS_PORTNAME "ttyUSB0"





#define DIR_CALIBRATION             "Calibration/"

#define SUBDIR_ACCELEROMETER        "Accelerometer/"

#define KEY_ACCEL_ZERO_POINT_X      "AccelZeroPointX"
#define DEFAULT_ACCEL_ZERO_POINT_X  "0"
#define KEY_ACCEL_ZERO_POINT_Y      "AccelZeroPointY"
#define DEFAULT_ACCEL_ZERO_POINT_Y  "0"
#define KEY_ACCEL_ZERO_POINT_Z      "AccelZeroPointZ"
#define DEFAULT_ACCEL_ZERO_POINT_Z  "0"


#define SUBDIR_GYROSCOPE            "Gyroscope/"

#define KEY_GYROS_ZERO_POINT_X      "GyrosZeroPointX"
#define DEFAULT_GYROS_ZERO_POINT_X  "0"
#define KEY_GYROS_ZERO_POINT_Y      "GyrosZeroPointY"
#define DEFAULT_GYROS_ZERO_POINT_Y  "0"
#define KEY_GYROS_ZERO_POINT_Z      "GyrosZeroPointZ"
#define DEFAULT_GYROS_ZERO_POINT_Z  "0"



//=======================================================
#define DIR_RESTORE_SETTINGS        "RestoreSettings/"
#define KEY_RESTORE_SETTINGS        "RestoreSettings"
#define DEFAULT_RESTORE_SETTINGS    "false"

//namespace constants
//{
//    inline constexpr double pi { 3.14159 };

////    inline constexpr auto ORGANIZATION_NAME { "Deciptikon" };
////    inline constexpr auto ORGANIZATION_DOMAIN { "vk.com/deciptikon" };
////    inline constexpr auto APPLICATION_NAME { "AgroSlave" };

////    inline constexpr auto DIR_STATUSBAR { "statusBar/" };
////    inline constexpr auto KEY_COLOR_STATUSBAR { "colorStatusBar" };

//}

#endif // CONSTANTS_H
