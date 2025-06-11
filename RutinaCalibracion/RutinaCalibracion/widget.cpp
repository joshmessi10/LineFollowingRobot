#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDataStream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug()<<"Inicia el programa";

    ttl = new QSerialPort(this);//apuntador con formato de puerto serial
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        QString pname = serialPortInfo.portName();
        ui->comboBox->addItem(pname);
    }
}

Widget::~Widget()
{
    delete ui;
    qDebug()<<"Termina el programa";
    ttl->close();
}

void Widget::on_pushButton_clicked()
{
    abrirPuerto();
}

void Widget::abrirPuerto()
{
    QString ttlPortName = ui->comboBox->currentText();
    if(ui->pushButton->text() == "Abrir Puerto"){
        ttl->setPortName(ttlPortName);
        ttl->setBaudRate(QSerialPort::Baud9600);//el profe le puso 115200 en el ejemplo
        ttl->setDataBits(QSerialPort::Data8);
        ttl->setFlowControl(QSerialPort::NoFlowControl);
        ttl->setParity(QSerialPort::NoParity);
        ttl->setStopBits(QSerialPort::OneStop);
        ttl->open(QSerialPort::ReadWrite);
        connect(ttl,SIGNAL(readyRead()),this,SLOT(leerSerial()));//este es el connect
        ui->pushButton->setText("Cerrar Puerto");
    }
    else{
        ttl->close();
        disconnect(ttl,SIGNAL(readyRead()),this,SLOT(leerSerial()));//el disconnect
        ui->pushButton->setText("Abrir Puerto");
    }
}

void Widget::leerSerial()
{
    //QByteArray buffer = ttl->readAll();//este comando es pesimo, ya que no toma los ceros. Cuanto no sufri por eso :(.
    QByteArray buffer(QByteArray::fromRawData(ttl->readAll(), 32));
    if(buffer=="NO"){
        qDebug()<<"Error al recibir el dato";
    }
    else{
        adc1=(buffer[2]&0xff);
        adc2=(buffer[3]&0xff);
        adc3=(buffer[4]&0xff);
        adc4=(buffer[5]&0xff);
        adc5=(buffer[6]&0xff);
        uint8_t tipoDato=buffer[7]&0xff;
        adcV=((buffer[8]&0xff));
        //sensado del seguidor
        if(tipoDato==1){
            ajustaColor1(adc1);
            ajustaColor2(adc2);
            ajustaColor3(adc3);
            ajustaColor4(adc4);
            ajustaColor5(adc5);
            adcV=adcV*12.455*3.3/1023;
            QString voltaje_txt= QString::number(adcV);
            ui->label_7->setText(voltaje_txt);
            qDebug()<<adc1<<"---"<<adc2<<"---"<<adc3<<"---"<<adc4<<"---"<<adc5<<" || Voltaje: "<<adcV;
        }
        if(tipoDato==2){//maximos label_19 - lable_23
            QString max_txt= QString::number(adc1*4);
            ui->label_19->setText(max_txt);
            max_txt= QString::number(adc2*4);
            ui->label_20->setText(max_txt);
            max_txt= QString::number(adc3*4);
            ui->label_21->setText(max_txt);
            max_txt= QString::number(adc4*4);
            ui->label_22->setText(max_txt);
            max_txt= QString::number(adc5*4);
            ui->label_23->setText(max_txt);
        }
        if(tipoDato==3){//minimos label_13 - lable_17
            QString min_txt= QString::number(adc1*4);
            ui->label_13->setText(min_txt);
            min_txt= QString::number(adc2*4);
            ui->label_14->setText(min_txt);
            min_txt= QString::number(adc3*4);
            ui->label_15->setText(min_txt);
            min_txt= QString::number(adc4*4);
            ui->label_16->setText(min_txt);
            min_txt= QString::number(adc5*4);
            ui->label_17->setText(min_txt);
        }

    }
    qDebug()<<"================================================";
}

void Widget::ajustaColor1(int color){
    //int valor = ui->horizontalSlider->value();
    double intensidad = color;
    double Y = intensidad*0.3+intensidad*0.59+intensidad*0.11;
    int z = (int) Y;
    QString r = QString::number(z);
    QString g = QString::number(z);
    QString b = QString::number(z);
    QString color2 = "rgb("+r+","+g+","+b+")";
    QString a="{ background-color : "+color2+"; color :"+color2+"; }";
    ui->label->setStyleSheet("QLabel "+a);
}

void Widget::ajustaColor2(int color){
    //int valor = ui->horizontalSlider->value();
    double intensidad = color;
    double Y = intensidad*0.3+intensidad*0.59+intensidad*0.11;
    int z = (int) Y;
    QString r = QString::number(z);
    QString g = QString::number(z);
    QString b = QString::number(z);
    QString color2 = "rgb("+r+","+g+","+b+")";
    QString a="{ background-color : "+color2+"; color :"+color2+"; }";
    ui->label_2->setStyleSheet("QLabel "+a);
}

void Widget::ajustaColor3(int color){
    //int valor = ui->horizontalSlider->value();
    double intensidad = color;
    double Y = intensidad*0.3+intensidad*0.59+intensidad*0.11;
    int z = (int) Y;
    QString r = QString::number(z);
    QString g = QString::number(z);
    QString b = QString::number(z);
    QString color2 = "rgb("+r+","+g+","+b+")";
    QString a="{ background-color : "+color2+"; color :"+color2+"; }";
    ui->label_3->setStyleSheet("QLabel "+a);
}

void Widget::ajustaColor4(int color){
    //int valor = ui->horizontalSlider->value();
    double intensidad = color;
    double Y = intensidad*0.3+intensidad*0.59+intensidad*0.11;
    int z = (int) Y;
    QString r = QString::number(z);
    QString g = QString::number(z);
    QString b = QString::number(z);
    QString color2 = "rgb("+r+","+g+","+b+")";
    QString a="{ background-color : "+color2+"; color :"+color2+"; }";
    ui->label_4->setStyleSheet("QLabel "+a);
}

void Widget::ajustaColor5(int color){
    //int valor = ui->horizontalSlider->value();
    double intensidad = color;
    double Y = intensidad*0.3+intensidad*0.59+intensidad*0.11;
    int z = (int) Y;
    QString r = QString::number(z);
    QString g = QString::number(z);
    QString b = QString::number(z);
    QString color2 = "rgb("+r+","+g+","+b+")";
    QString a="{ background-color : "+color2+"; color :"+color2+"; }";
    ui->label_5->setStyleSheet("QLabel "+a);
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    ajustaColor1(value);
}

void Widget::on_pushButton_2_clicked()//Rutina de calibracion
{
        QByteArray a;
        a.append(1);//PROTOCOLO (en bytes): 0. inicio(0x01), 1. tamaño (datos), 2 - (n+1).datos, n+2. checksum, n+3. final(0x02)
        uint8_t tam=a.size();
        char inicio=1, parada=2, confirma=0;
        confirma=confirma+tam+inicio;
        for(int i=0; i<tam;i++){
            confirma+=a[i];
        }
        a.prepend(tam);
        a.prepend(inicio);
        a.append(confirma);
        a.append(parada);

        if(ttl->isOpen()){
            ttl->write(a);

        }
        else{
            QMessageBox msg;
            msg.setText("El puerto esta cerrado");
            msg.exec();
        }
}

void Widget::on_pushButton_3_clicked()//motor derecho
{
    QByteArray a;
        a.append(2);//PROTOCOLO (en bytes): 0. inicio(0x01), 1. tamaño (datos), 2 - (n+1).datos, n+2. checksum, n+3. final(0x02)
        uint8_t tam=a.size();
        char inicio=1, parada=2, confirma=0;
        confirma=confirma+tam+inicio;
        for(int i=0; i<tam;i++){
            confirma+=a[i];
        }
        a.prepend(tam);
        a.prepend(inicio);
        a.append(confirma);
        a.append(parada);

        if(ttl->isOpen()){
            ttl->write(a);

        }
    else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado");
        msg.exec();
    }

}

void Widget::on_pushButton_4_clicked()//motor izquierdo
{
    QByteArray a;
        a.append(3);//PROTOCOLO (en bytes): 0. inicio(0x01), 1. tamaño (datos), 2 - (n+1).datos, n+2. checksum, n+3. final(0x02)
        uint8_t tam=a.size();
        char inicio=1, parada=2, confirma=0;
        confirma=confirma+tam+inicio;
        for(int i=0; i<tam;i++){
            confirma+=a[i];
        }
        a.prepend(tam);
        a.prepend(inicio);
        a.append(confirma);
        a.append(parada);

        if(ttl->isOpen()){
            ttl->write(a);

        }
    else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado");
        msg.exec();
    }
}

void Widget::on_pushButton_5_clicked()//seguidor de linea
{
    QByteArray a;
        a.append(4);//PROTOCOLO (en bytes): 0. inicio(0x01), 1. tamaño (datos), 2 - (n+1).datos, n+2. checksum, n+3. final(0x02)
        uint8_t tam=a.size();
        char inicio=1, parada=2, confirma=0;
        confirma=confirma+tam+inicio;
        for(int i=0; i<tam;i++){
            confirma+=a[i];
        }
        a.prepend(tam);
        a.prepend(inicio);
        a.append(confirma);
        a.append(parada);

        if(ttl->isOpen()){
            ttl->write(a);

        }
    else{
        QMessageBox msg;
        msg.setText("El puerto esta cerrado");
        msg.exec();
    }
}
