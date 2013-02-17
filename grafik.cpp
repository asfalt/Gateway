#include "grafik.h"
#include "ui_grafik.h"

Grafik::Grafik(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Grafik)
{
    ui->setupUi(this);

    Scene1 = new QGraphicsScene(this);
    ui->graphicsView->setScene(Scene1);

    ui->pushButton_4->setFocus();
}

Grafik::~Grafik()
{
    delete ui;
}

void Grafik::on_pushButton_clicked()
{
    emit plotHiSignal();

}

void Grafik::on_pushButton_2_clicked()
{
    emit plotQ1Signal();

}

void Grafik::on_pushButton_3_clicked()
{
    emit plotQ2Signal();

}

void Grafik::on_pushButton_4_clicked()
{
    emit plotQSignal();
}

void Grafik::on_pushButton_5_clicked()
{
    emit plotMu1Signal();
}

void Grafik::on_pushButton_6_clicked()
{
    emit plotMu2Signal();
}

void Grafik::on_checkBox_clicked()
{
    emit EventPlotSignal();
    on_pushButton_clicked();
    ui->pushButton->setFocus();

}

void Grafik::on_pushButton_7_clicked()
{
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
