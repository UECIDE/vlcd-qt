#include "lcd.h"
#include <QPainter>

LCD::LCD(QWidget *parent) :
    QWidget(parent)
{
    setColor(QColor(0, 255, 0.0), QColor(0, 0, 0));
    setShadow(true);
    setSmallPixels(true);
    memset(_data, 0, 256*64/8);
}

void LCD::drawPixel(QImage &image, int x, int y, bool state) {
    if (state) {
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                uint32_t rgb = _fg.rgb();
                if (_shadowEnabled) {
                    if (((px == 3) && (py > 0)) || ((px > 0) && (py == 3))) {
                        rgb = _shadow.rgb();
                    } else if (((px == 0) && (py == 3)) || ((px == 3) && (py == 0))) {
                        rgb = _bg.rgb();
                    }
                } else if (_smallPixels) {
                    if ((px == 3) || (py == 3)) {
                        rgb = _bg.rgb();
                    }
                }
                image.setPixel(x * 4 + px, y * 4 + py, rgb);
            }
        }
    } else {
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                image.setPixel(x * 4 + px, y * 4 + py, _bg.rgb());
            }
        }
    }
}

void LCD::paintEvent(QPaintEvent __attribute__((unused)) *event) {
    QPainter painter(this);

    QImage image(1024, 256, QImage::Format_RGB32);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 255; x++) {
            uint8_t b = _data[y * 256 + x];
            for (uint8_t z = 0; z < 8; z++) {
                uint8_t m = uint8_t(1 << z);
                drawPixel(image, x, y * 8 + z, (b & m) != 0);
            }
        }
    }
    painter.drawImage(rect(), image);
}

void LCD::setColor(QColor fg, QColor bg) {
    _fg = fg;
    _bg = bg;
    _shadow.setRed((fg.red() + bg.red()) / 2);
    _shadow.setGreen((fg.green() + bg.green()) / 2);
    _shadow.setBlue((fg.blue() + bg.blue()) / 2);
}

void LCD::setSmallPixels(bool sp) {
    _smallPixels = sp;
}

void LCD::setShadow(bool shad) {
    _shadowEnabled = shad;
}

void LCD::setPageData(int page, uint8_t *data, int len) {
    int offset = page * 256;
    for (int i = 0; i < len; i++) {
        _data[offset + i] = data[i];
    }
    repaint();
}
