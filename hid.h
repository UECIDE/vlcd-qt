#ifndef HID_H
#define HID_H
#include <hidapi/hidapi.h>
#include <QMap>
#include <QThread>

class HID : public QThread {
    Q_OBJECT
    void run() override;

private:
    uint16_t _vid;
    uint16_t _pid;
    wchar_t *_serial;
    bool _running;

    hid_device *_connectedDevice;

public:
    HID();
    QMap<QString, QString> getCompatibleDevices();
    void connectToPath(QString &path);
    void setVidPid(uint16_t vid, uint16_t pid, wchar_t *serial = nullptr);
    void cleanup();

signals:
    void packetReceived(uint8_t *data, int len);
};



#endif // HID_H
