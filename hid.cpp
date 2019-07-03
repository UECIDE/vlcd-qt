#include "hid.h"
#include <QDebug>

HID::HID() : _vid(0xf055), _pid(0x1234), _serial(nullptr), _connectedDevice(nullptr) {
    this->start();
}

QMap<QString, QString> HID::getCompatibleDevices() {
    QMap<QString, QString> out;
    struct hid_device_info *devices = nullptr;
    struct hid_device_info *cur_dev = nullptr;
    devices = hid_enumerate(_vid, _pid);
    cur_dev = devices;
    while (cur_dev) {
        QString description = QString::fromWCharArray(cur_dev->manufacturer_string);
        description.append(" ");
        description.append(QString::fromWCharArray(cur_dev->product_string));
        description.append(" (");
        description.append(QString::fromWCharArray(cur_dev->serial_number));
        description.append(")");
        QString path = cur_dev->path;
        out.insert(path, description);
        cur_dev = cur_dev->next;
    }

    hid_free_enumeration(devices);

    return out;
}

void HID::connectToPath(QString &path) {
    if (_connectedDevice != nullptr) {
        hid_close(_connectedDevice);
    }

    if (path == "") {
        _connectedDevice = nullptr;
        return;
    }

    _connectedDevice = hid_open_path(path.toLatin1().data());
    if (!_connectedDevice) {
        _connectedDevice = nullptr;
        qDebug() << "Error connecting to device " << path;
    } else {
        qDebug() << "Connected to " << path;
    }
}

void HID::setVidPid(uint16_t vid, uint16_t pid, wchar_t *serial) {
    _vid = vid;
    _pid = pid;
    _serial = serial;
}

void HID::run() {
    _running = true;
    while (_running) {
        if (_connectedDevice == nullptr) {
            msleep(500);
            continue;
        }

        uint8_t data[1024];
        int res = hid_read_timeout(_connectedDevice, data, 1024, 500);
        if (res > 0) {
            emit packetReceived(data, res);
        }
    }
}

void HID::cleanup() {
    _running = false;
    terminate();
    msleep(100);
}
