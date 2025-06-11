#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void leerSerial();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *ttl = nullptr;
    void abrirPuerto();
    void ajustaColor1(int color);
    void ajustaColor2(int color);
    void ajustaColor3(int color);
    void ajustaColor4(int color);
    void ajustaColor5(int color);
    int adc1=0,adc2=0,adc3=0,adc4=0,adc5=0;
    float adcV=0;
    int a=0;
};

#endif // WIDGET_H
