#include "lcd.h"
#include "lcdplugin.h"

#include <QtPlugin>

LCDPlugin::LCDPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void LCDPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool LCDPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *LCDPlugin::createWidget(QWidget *parent)
{
    return new LCD(parent);
}

QString LCDPlugin::name() const
{
    return QLatin1String("LCD");
}

QString LCDPlugin::group() const
{
    return QLatin1String("Graphics");
}

QIcon LCDPlugin::icon() const
{
    return QIcon(QLatin1String(":/chardevice.png"));
}

QString LCDPlugin::toolTip() const
{
    return QLatin1String("Virtual LCD Display");
}

QString LCDPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool LCDPlugin::isContainer() const
{
    return false;
}

QString LCDPlugin::domXml() const
{
    return QLatin1String(
                         "<ui language=\"c++\" displayname=\"LCD\">"
                            "<widget class=\"LCD\" name=\"LCD\">"
                            "</widget>"
                            "<customwidgets>"
                                "<customwidget>"
                                    "<class>LCD</class>"
                                "</customwidget>"
                            "</customwidgets>"
                         "</ui>"
                );
}

QString LCDPlugin::includeFile() const
{
    return QLatin1String("lcd.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lcdplugin, LCDPlugin)
#endif // QT_VERSION < 0x050000
