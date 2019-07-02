#ifndef LCD_H
#define LCD_H

#include <QWidget>

class LCD : public QWidget
{
        Q_OBJECT

    private:
        uint8_t _data[64*256/8];
        QColor _fg;
        QColor _bg;
        QColor _shadow;
        bool _shadowEnabled;
        bool _smallPixels;

        void drawPixel(QImage &image, int x, int y, bool state);

    public:
        LCD(QWidget *parent = nullptr);

        void setColor(QColor fg, QColor bg);
        void setShadow(bool shad);
        void setSmallPixels(bool sp);
        void setPageData(int page, uint8_t *data, int len);

    protected:
        void paintEvent(QPaintEvent *event) override;
};

#endif
