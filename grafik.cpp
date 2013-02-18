#include "grafik.h"
#include "ui_grafik.h"
#include <QDebug>

Grafik::Grafik(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Grafik)
{
    ui->setupUi(this);

    Scene1 = new QGraphicsScene(this);
    ui->graphicsView->setScene(Scene1);

//    ui->pushButton_4->setFocus();
    ui->pushButton_10->setVisible(false);
    numGraph = 4;
}

Grafik::~Grafik()
{
    delete ui;
}

void Grafik::on_pushButton_clicked()
{
    numGraph = 1;
    emit plotHiSignal();

}

void Grafik::on_pushButton_2_clicked()
{
    numGraph = 2;
    emit plotQ1Signal();

}

void Grafik::on_pushButton_3_clicked()
{
    numGraph = 3;
    emit plotQ2Signal();

}

void Grafik::on_pushButton_4_clicked()
{
    numGraph = 4;
    emit plotQSignal();

}

void Grafik::on_pushButton_5_clicked()
{
    numGraph = 5;
    emit plotMu1Signal();

}

void Grafik::on_pushButton_6_clicked()
{
    numGraph = 6;
    emit plotMu2Signal();

}

void Grafik::on_checkBox_clicked()
{
    emit EventPlotSignal();
 //   on_pushButton_clicked();
 //   ui->pushButton->setFocus();

}

void Grafik::on_pushButton_7_clicked()
{
    numGraph = 7;
    emit plotSigmaSignal();

}

void Grafik::on_pushButton_9_clicked()
{
    emit SaveWavesSignal();
}

void Grafik::on_pushButton_8_clicked()
{
    emit SaveCsvSignal();
}

void Grafik::on_pushButton_10_clicked()
{
    emit SaveULPSignal();
}

void Grafik::FocusButtons()
{
    if(numGraph == 1)
    {
        ui->pushButton->setFocus();
    }
    else if(numGraph == 2)
    {
        ui->pushButton_2->setFocus();
    }
    else if(numGraph == 3)
    {
        ui->pushButton_3->setFocus();
    }
    else if(numGraph == 4)
    {
        ui->pushButton_4->setFocus();
        qDebug() << "4 fok";
    }
    else if(numGraph == 5)
    {
        ui->pushButton_5->setFocus();
        qDebug() << "5 fok";
    }
    else if(numGraph == 6)
    {
        ui->pushButton_6->setFocus();
    }
    else
    {
        ui->pushButton_7->setFocus();
    }
}
