#include "vlcd.h"
#include "ui_vlcd.h"
#include <QSettings>
#include <QDebug>
#include "hid.h"

VLCD::VLCD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VLCD)
{
    ui->setupUi(this);

    QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(appIsClosing()));

    QSettings settings(":/config.ini", QSettings::IniFormat);

    settings.beginGroup("application");

    bool ok;
    setWindowTitle(settings.value("name").toString());
    _hid.setVidPid(settings.value("vid").toString().toUShort(&ok, 16), settings.value("pid").toString().toUShort(&ok, 16));
    using namespace std::chrono;
    startTimer(seconds(1));
    settings.endGroup();

    QStringList groups = settings.childGroups();
    QStringListIterator iter(groups);

    while (iter.hasNext()) {
        QString group = iter.next();
        if (group.startsWith("color.")) {
            settings.beginGroup(group);
            QMap<QString, QVariant> colorSet;
            colorSet.insert("foreground", parseColor(settings.value("foreground").toString()));
            colorSet.insert("background", parseColor(settings.value("background").toString()));
            colorSet.insert("shadow", settings.value("shadow").toBool());
            ui->colorList->addItem(settings.value("name").toString(), colorSet);
            settings.endGroup();
        }
    }

    QObject::connect(&_hid, SIGNAL(packetReceived(uint8_t *, int )), this, SLOT(gotPacket(uint8_t *, int )));
}

unsigned int VLCD::parseColor(const QString &str) {
    if (str.startsWith("#")) {
        bool ok;
        return str.right(str.length() - 1).toUInt(&ok, 16);
    }
    return 0;
}

VLCD::~VLCD()
{
    delete ui;
}


void VLCD::timerEvent(QTimerEvent __attribute__((unused)) *event) {
    QMap<QString, QString> list = _hid.getCompatibleDevices();

    // First we want to add any new devices to the list
    QMapIterator<QString, QString> iter(list);
    while (iter.hasNext()) {
        iter.next();
        if ((ui->deviceList->findData(iter.key()) >= 0) && (ui->deviceList->findText(iter.value()) >= 0)) {
            continue;
        }
        qDebug() << "Adding item " << iter.key();
        ui->deviceList->addItem(iter.value(), iter.key());
    }

    // Now we can remove any devices that no longer exist
    bool found = false;
    do {
        found = false;
        for (int i = 0; i < ui->deviceList->count(); i++) {
            QString path = ui->deviceList->itemData(i).toString();
            QString text = ui->deviceList->itemText(i);

            if (list.contains(path)) {
                if (list[path] == text) {
                    continue;
                }
            }
            qDebug() << "Removing item " << path;
            ui->deviceList->removeItem(i);
            found = true;
            break;
        }
    } while (found);

}

void VLCD::on_deviceList_currentIndexChanged(int __attribute__((unused)) index)
{
    QString path = ui->deviceList->currentData().toString();
    _hid.connectToPath(path);
}

void VLCD::appIsClosing() {
    _hid.cleanup();
}

void VLCD::gotPacket(uint8_t *data, int len) {
    if (len <= 0) return;

    switch (data[0]) {
        case 1: // Control packet
            break;
        case 2: { // Page packet
            int page = data[1];
            ui->LCDDisplay->setPageData(page, &data[2], 256);
        } break;

    }

}

void VLCD::on_colorList_currentIndexChanged(int __attribute__((unused)) index)
{
    QMap<QString, QVariant> colorSet = ui->colorList->currentData().toMap();
    ui->LCDDisplay->setColor(QColor(colorSet["foreground"].toUInt()), QColor(colorSet["background"].toUInt()));
    ui->LCDDisplay->setShadow(colorSet["shadow"].toBool());
    ui->LCDDisplay->repaint();
}
