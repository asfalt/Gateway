#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QDebug>
#include <QtCore/qmath.h>

#include "grafik.h"
#include <QGraphicsTextItem>

#include <QDataStream> //для импорта в UNDALIMINAPORTAE
#include <QLocale>     //для смены десятичного разделителя на запятую

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool testValuesMain();     //проверка правильности введённых значений для расчёта
    bool testValuesSecond();   //проверка правильности введённых значений для waves
    void setValues();            //присваиваем значения переменным
    void openPar(bool isParDefault);   //открываем файл с исходными данными


    //функции для коэффициентов расхода верхнего отверстия
    float mu_25();
    float mu_15();
    float mu_07();
    float mu1();
    float mu2();
    float sigma();



    float g;   //Ускорение свободного падения

    float Lk;  //длина камеры
    float Bk;  //ширина камеры
    float Ho;  //начальный напор
    float hp;  //глубина на пороге верхней головы
    float p1;  // вел заглубления скоса порога по вертикали
    float p2;  // высота уступа порога
    float Vvor;  //скорость опускания затвора
    float hovmax;  //максимальная величина опускания ворот
    float Lkons;  // длина консоли ворот
    float av;  //возвышение ворот над УВБ в закрытом состоянии
    float tvor;  //толщина ворот по верху
    float deltat;  // шаг по времени
    float alfa;
    float mu1Const;  //постоянное значение коэфф расхода ниж отв

    int typeog;  //тип оголовка 1 - цилиндрический 2-прямоугольный
    int typekoz;  //тип стр козырька 1 - полигональный 2-криволинейный

    float hokam;  //начальная глубина в камере
    float Lsud;  //длина судна
    float Bsud;  // ширина судна
    float Hsud;  // осадка судна
    float Lpolsud;  // расст от створа поступления воды до судна

    QVector<float> htimeVec;      //напор на камеру
    QVector<float> Q1timeVec;     //расход через нижнее отверстие
    QVector<float> Q2timeVec;     //расход через верхнее отверстие
    QVector<float> mu1timeVec;    //коэффициент расхода нижнего отверстия
    QVector<float> mu2timeVec;    //коэффициент расхода верхнего отверстия
    QVector<float> sigmaVec;

    int ntime;    //номер промежутка времени
    float hovi;   //высота текущего положения ворот
    float htime;  //напор на камеру в расчётном промежутке по времени
    float htimePrew;  //напор на камеру в предыдущем промежутке по времени

    float htime_max, Q1_max, Q2_max, mu1_max, mu2_max, sigma_max;

    float timeNijPodt;    //промежуток времени подтопления нижнего отверстия
    float TimeVerhRash;   //промежуток времени появления расхода через верхнее отверстие
    float timeVerhPodt;   //промежуток времени подтопления верхнего отверстия
    float timeOstanZatv;  // промежуток времени остановки затвора
    bool showEventsFlag;




    Grafik *Gra;
    
private slots:
    void on_checkBox_2_clicked();

    void on_checkBox_1_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void PlotHi();
    void PlotQ1();
    void PlotQ2();
    void PlotQ();
    void PlotMu1();
    void PlotMu2();
    void PlotSigma();
    void trigEventPlot();
    void SaveToCsv();
    void SaveToWaves();
    void SaveToULP();

    void PlotAfterEval();

    void on_checkBoxMu1Const_clicked();

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
