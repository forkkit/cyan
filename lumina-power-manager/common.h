/*
# Copyright (c) 2018, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
*/

#ifndef COMMON_H
#define COMMON_H

#include <QSettings>
#include <QVariant>

enum lidAction
{
    lidNone,
    lidLock,
    lidSleep,
    lidHibernate
};

enum criticalAction
{
    criticalNone,
    criticalHibernate,
    criticalShutdown
};

#define LID_BATTERY_DEFAULT lidSleep
#define LID_AC_DEFAULT lidLock
#define CRITICAL_DEFAULT criticalNone

#define LOW_BATTERY 15
#define CRITICAL_BATTERY 10
#define AUTO_SLEEP_BATTERY 15
#define DEFAULT_BATTERY_ICON "battery"
#define DEFAULT_BATTERY_ICON_LOW "battery-caution"

#define PM_SERVICE "org.freedesktop.PowerManagement"
#define PM_PATH "/PowerManagement"

class Common
{
public:
    static void savePowerSettings(QString type, QVariant value)
    {
        QSettings settings("lumina-desktop", "lumina-power");
        settings.setValue(type, value);
    }
    static QVariant loadPowerSettings(QString type)
    {
        QSettings settings("lumina-desktop", "lumina-power");
        return settings.value(type);
    }
    static bool validPowerSettings(QString type)
    {
        QSettings settings("lumina-desktop", "lumina-power");
        return settings.value(type).isValid();
    }
};

#endif // COMMON_H
