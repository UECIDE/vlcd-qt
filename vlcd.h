#ifndef VLCD_H
#define VLCD_H

#include <QWidget>

#include "hid.h"

namespace Ui {
class VLCD;
}

class VLCD : public QWidget
{
    Q_OBJECT
protected:
    void timerEvent(QTimerEvent *event) override;

public:
    explicit VLCD(QWidget *parent = nullptr);
    ~VLCD();


private slots:
    void on_deviceList_currentIndexChanged(int index);
    void appIsClosing();
    void on_colorList_currentIndexChanged(int index);

    public slots:
    void gotPacket(uint8_t *data, int len);

private:
    Ui::VLCD *ui;

    HID _hid;
    unsigned int parseColor(const QString &str);
};

#endif // VLCD_H
