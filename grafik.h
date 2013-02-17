#ifndef GRAFIK_H
#define GRAFIK_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class Grafik;
}

class Grafik : public QDialog
{
    Q_OBJECT
    
public:
    explicit Grafik(QWidget *parent = 0);
    ~Grafik();

    Ui::Grafik *ui;

    QGraphicsScene *Scene1;
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

signals:
    void plotHiSignal();
    void plotQ1Signal();
    void plotQ2Signal();
    void plotQSignal();
    void plotMu1Signal();
    void plotMu2Signal();
    void plotSigmaSignal();

    void EventPlotSignal();

    void SaveCsvSignal();
    void SaveWavesSignal();

private:

};

#endif // GRAFIK_H
