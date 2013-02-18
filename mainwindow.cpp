#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    g = 9.81;  //Ускорение свободного падения

    Lk = 0;  //длина камеры
    Bk = 0;  //ширина камеры
    Ho = 0;  //начальный напор
    hp = 0;  //глубина на пороге верхней головы
    p1 = 0;  // вел заглубления скоса порога по вертикали
    p2 = 0;  // высота уступа порога
    Vvor = 0;  //скорость опускания затвора
    hovmax = 0;  //максимальная величина опускания ворот
    Lkons = 0;  // длина консоли ворот
    av = 0;  //возвышение ворот над УВБ в закрытом состоянии
    tvor = 0;  //толщина ворот по низу
    deltat = 0;  // шаг по времени

    typeog = 0;  //тип оголовка 1 - цилиндрический 2-прямоугольный
    typekoz = 0;  //тип стр козырька 1 - полигональный 2-криволинейный
    alfa = 0;
    ui->Editalfa->setText("0");
    mu1Const = 0.6;

    hokam = 0;  //начальная глубина в камере
    Lsud = 0;  //длина судна
    Bsud = 0;  // ширина судна
    Hsud = 0;  // осадка судна
    Lpolsud = 0;  // расст от створа поступления воды до судна





    ntime = 0;    //текущий промежуток времени
    hovi = 0;     //высота положения ворот
    htime = Ho;
    htimePrew = 0.0;

    timeNijPodt = 0;    //промежуток времени подтопления нижнего отверстия
    TimeVerhRash = 0;   //промежуток времени появления расхода через верхнее отверстие
    timeVerhPodt = 0;   //промежуток времени подтопления верхнего отверстия
    timeOstanZatv = 0;  // промежуток времени остановки затвора
    showEventsFlag = false;

    Gra = new Grafik();



    connect(Gra, SIGNAL(plotHiSignal()), this, SLOT(PlotHi()) );
    connect(Gra, SIGNAL(plotQ1Signal()), this, SLOT(PlotQ1()) );
    connect(Gra, SIGNAL(plotQ2Signal()), this, SLOT(PlotQ2()) );
    connect(Gra, SIGNAL(plotQSignal()), this, SLOT(PlotQ()) );
    connect(Gra, SIGNAL(plotMu1Signal()), this, SLOT(PlotMu1()) );
    connect(Gra, SIGNAL(plotMu2Signal()), this, SLOT(PlotMu2()) );
    connect(Gra, SIGNAL(plotSigmaSignal()), this, SLOT(PlotSigma()) );

    connect(Gra, SIGNAL(EventPlotSignal()), this, SLOT(trigEventPlot()) );
    connect(Gra, SIGNAL(SaveCsvSignal()), this, SLOT(SaveToCsv()) );
    connect(Gra, SIGNAL(SaveWavesSignal()), this, SLOT(SaveToWaves()) );




    //откроем файл параметров по умолчанию
    openPar(true);


    ui->pushButton_4->setVisible(false);
    ui->EditMu1Const->setVisible(false);
    ui->label_22->setVisible(false);

    //длина камеры
    //ширина камеры
    //начальный напор
    //глубина на пороге верхней головы
    // вел заглубления скоса порога по вертикали
    // высота уступа порога
    //скорость опускания затвора
    //максимальная величина опускания ворот
    // длина консоли ворот
    //возвышение ворот над УВБ в закрытом состоянии
    //толщина ворот по низу
    // шаг по времени

    //тип оголовка 1 - цилиндрический 2-прямоугольный
    //тип стр козырька 1 - полигональный 2-криволинейный
    //альфа

    //начальная глубина в камере
    //длина судна
    // ширина судна
    // осадка судна
    // расст от створа поступления воды до судна
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_2_clicked()
{
    //прямоугольный тип оголовка
    ui->checkBox_1->setChecked(false);
}

void MainWindow::on_checkBox_1_clicked()
{
    //цилиндрический тип оголовка
    ui->checkBox_2->setChecked(false);
}

void MainWindow::on_checkBox_3_clicked()
{
    //полигональный тип струенаправляющего козырька
    ui->checkBox_4->setChecked(false);
    ui->Editalfa->setVisible(true);
    ui->label_12->setVisible(true);
}

void MainWindow::on_checkBox_4_clicked()
{
    //криволинейный тип струенаправляющего козырька
    ui->checkBox_3->setChecked(false);
    ui->Editalfa->setVisible(false);
    ui->label_12->setVisible(false);
}

void MainWindow::on_pushButton_clicked()//кнопка расчёт
{
    htimeVec.clear();
    Q1timeVec.clear();
    Q2timeVec.clear();
    mu1timeVec.clear();
    mu2timeVec.clear();
    sigmaVec.clear();

    htime_max = 0.0;
    Q1_max = 0.0;
    Q2_max = 0.0;
    mu1_max = 0.0;
    mu2_max = 0.0;
    sigma_max = 1.0;

    timeNijPodt = 0;
    timeOstanZatv = 0;
    timeVerhPodt = 0;
    TimeVerhRash = 0;


    //сначала проверим введённые значения
    if (!testValuesMain())
    {
        return;
    }
    //присвоим введённые значения переменным
    setValues();

    ntime = 0;
    htime = Ho;
    hovi = 0.0;

    htimeVec.append(htime);
    Q1timeVec.append(0.0);
    Q2timeVec.append(0.0);
    mu1timeVec.append(0.0);
    mu2timeVec.append(0.0);
    sigmaVec.append(1.0);

    float h1, omega1, h2, omega2;
    h1 = 0.0; omega1 = 0.0;
    h2 = 0.0; omega2 =0.0;

    float muu1, muu2, siigma;
    muu1 = 0.0; muu2 = 0.0; siigma = 1.0;

    float QQ1, QQ2;
    QQ1 = 0.0; QQ2 = 0.0;

    while (htime > 0)
    {
       ntime++;
       htimePrew = htime;

    //Определяем глубину текущего положения затвора
    hovi = ntime * deltat * Vvor;
        //проверяем не опустился ли затвор ниже чем ограничен
       if(hovi > hovmax)
       {
           hovi = hovmax;
           if(timeOstanZatv == 0)
           {
           timeOstanZatv = ntime;
           }
       }


    //Определяем расход через нижнее отверстие
       if(htimePrew >= (hp + p1 + p2 + hovi/2) )  //нижнее отв не подтоплено
       {
           h1 = hp + p1 + p2 + hovi/2;
           omega1 = Bk * hovi;
 //          qDebug() << "нижнее не подтоплено   h1: " << h1;
       }
       else  //нижнее отв подтоплено
       {
           h1 = htimePrew;
           omega1 = Bk * hovi;
           if(timeNijPodt == 0)
           {
           timeNijPodt = (float) ntime;
           }
 //          qDebug() << "нижнее подтоплено   h1: " << h1;
       }

       //Для вычисления с постоянным коэффициентом расхода нижнего отверстия
       if(ui->checkBoxMu1Const->isChecked())
       {
          muu1 = mu1Const;
  //        qDebug() << "коэфф ниж отв постоянный: " << mu1Const;
       }
       else
       {
          muu1 = mu1();
       }

       QQ1 = muu1*omega1*qSqrt(2*g*h1);


    //Определяем расход через верхнее отверстие
       if(hovi <= av)
       {
           QQ2 = 0;
 //          qDebug() << "верхнее нет расхода";
       }
       else
       {
           if(TimeVerhRash == 0)
           {
           TimeVerhRash = (float) ntime;
           }




           if(htimePrew >= hovi - av ) //верх отв не подтоплено
           {
               h2 = hovi - av;
               omega2 = Bk*h2;
               muu2 = mu2();
               QQ2 = muu2*omega2*qSqrt(2*g*h2);

  //             qDebug() << "верхнее не подтоплено  h2: " << h2;
           }
           else //верх отв подтоплено
           {
               h2 = hovi - av;
               omega2 = Bk*h2;
               siigma = sigma();
               muu2 = mu2();
               QQ2 = siigma * muu2*omega2*qSqrt(2*g*h2);

               if(timeVerhPodt == 0)
               {
               timeVerhPodt = (float) ntime;
               }
  //             qDebug() << "верхнее подтоплено  h2: " << h2;
           }



       }

    //Вычисляем напор на конец данного промежутка
    htime = htimePrew - ((QQ1+QQ2)/(Lk*Bk))*deltat;

    htimeVec.append(htime);
    Q1timeVec.append(QQ1);
    Q2timeVec.append(QQ2);
    mu1timeVec.append(muu1);
    mu2timeVec.append(muu2);
    sigmaVec.append(siigma);


    htime_max = qMax(htime_max, htime);
    Q1_max = qMax(Q1_max, QQ1);
    Q2_max = qMax(Q2_max, QQ2);
    mu1_max = qMax(mu1_max, muu1);
    mu2_max = qMax(mu2_max, muu2);
    sigma_max = qMax(sigma_max, siigma);

/*    qDebug() << ntime << "   время: " <<ntime*deltat << "    hi= " << htime   << "  Q1: " << QQ1  << " mu1:" << muu1 << "  omega1: " << omega1 << "   ||"         << "  Q2: " << QQ2 << " mu2:" << muu2 << "  omega2: " << omega2  << "  hovi: " << hovi << "  av: " << av;
    qDebug() << "   sigma: " << siigma;
    qDebug() << "    ____________";
    qDebug() << "   ";
*/
    }

    //показываем форму графика
    if(!Gra->isVisible())
    {
    Gra->show();
    Gra->setWindowTitle("График");
    }
//    PlotQ();
    PlotAfterEval();
//    Gra->FocusButtons();
    Gra->activateWindow();


}

void MainWindow::on_pushButton_3_clicked() //сохранить файл параметров
{
    //сначала проверим введённые значения
    if (!testValuesMain())
    {
        return;
    }

    //запускаем диалог сохранения
    QString FileName = QFileDialog::getSaveFileName(this,
                                                        "Сохранить в текстовый файл исходные данные", ".txt",
                                                        "Текстовые файлы (*.txt);;Все файлы (*)");

    if (FileName.isEmpty())
        {
            return;
        }

    QFile filewr(FileName);
    filewr.open(QIODevice::WriteOnly);
    QTextStream out(&filewr);


    //длина камеры
    out << ui->EditLk->text();
    out << "\n";

    //ширина камеры
    out << ui->EditBk->text();
    out << "\n";

    //начальный напор
    out << ui->EditHo->text();
    out << "\n";

    //глубина на пороге верхней головы
    out << ui->Edithp->text();
    out << "\n";

    // вел заглубления скоса порога по вертикали
    out << ui->Editp1->text();
    out << "\n";

    // высота уступа порога
    out << ui->Editp2->text();
    out << "\n";

    //скорость опускания затвора
    out << ui->EditVvor->text();
    out << "\n";

    //максимальная величина опускания ворот
    out << ui->Edithovmax->text();
    out << "\n";

    // длина консоли ворот
    out << ui->EditLkons->text();
    out << "\n";

    //возвышение ворот над УВБ в закрытом состоянии
    out << ui->Editav->text();
    out << "\n";

    //толщина ворот по верху
    out << ui->Edittvor->text();
    out << "\n";

    // шаг по времени
    out << ui->Editdeltat->text();
    out << "\n";

    //тип оголовка 1 - цилиндрический 2-прямоугольный
    if(ui->checkBox_1->isChecked())
    {
        out << 1;
        out << "\n";
    }
    else
    {
        out << 2;
        out << "\n";
    }

    //тип стр козырька 1 - полигональный 2-криволинейный
    if(ui->checkBox_3->isChecked())
    {
        out << 1;
        out << "\n";
    }
    else
    {
        out << 2;
        out << "\n";
    }

    //альфа
    out << ui->Editalfa->text();
    out << "\n";

    //постоянный коэфф расхода нижнего отв.

    out << ui->EditMu1Const->text();
    out << "\n";


    //необязательные параметры проверим

    //начальная глубина в камере
    if(ui->Edithokam->text().isEmpty())
    {
        out << " ";
    }
    else
    {
    out << ui->Edithokam->text();
    }
    out << "\n";

    //длина судна
    if(ui->EditLsud->text().isEmpty())
    {
        out << " ";
    }
    else
    {
    out << ui->EditLsud->text();
    }
    out << "\n";

    // ширина судна
    if(ui->EditBsud->text().isEmpty())
    {
        out << " ";
    }
    else
    {
    out << ui->EditBsud->text();
    }
    out << "\n";

    // осадка судна
    if(ui->EditHsud->text().isEmpty())
    {
        out << " ";
    }
    else
    {
    out << ui->EditHsud->text();
    }
    out << "\n";

    // расст от створа поступления воды до судна
    if(ui->EditLpolsud->text().isEmpty())
    {
        out << " ";
    }
    else
    {
    out << ui->EditLpolsud->text();
    }
    out << "\n";

}

bool MainWindow::testValuesMain()
{
    QString sa;
    bool ok;

    //длина камеры
    sa = ui->EditLk->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditLk->setFocus();
        return ok;
    }

    //ширина камеры
    sa = ui->EditBk->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditBk->setFocus();
        return  ok;
    }

    //начальный напор
    sa = ui->EditHo->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditHo->setFocus();
        return  ok;
    }

    //глубина на пороге верхней головы
    sa = ui->Edithp->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Edithp->setFocus();
        return ok;
    }

    // вел заглубления скоса порога по вертикали
    sa = ui->Editp1->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Editp1->setFocus();
        return ok;
    }


    // высота уступа порога
    sa = ui->Editp2->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Editp2->setFocus();
        return ok;
    }


    //скорость опускания затвора
    sa = ui->EditVvor->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditVvor->setFocus();
        return ok;
    }


    //максимальная величина опускания ворот
    sa = ui->Edithovmax->text();
    float hovmaxPr = sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Edithovmax->setFocus();
        return ok;
    }
    else if(hovmaxPr > 3.5)
    {
        QMessageBox::about(this, "Некорректное значение", "Максимальная величина опускания ворот должна находиться \nв интервале значений от 0.0 до 3.5, включительно.");
        ui->EditLkons->setFocus();
        return false;
    }


    // длина консоли ворот
    sa = ui->EditLkons->text();
    float lkonsPr = sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditLkons->setFocus();
        return ok;
    }
    else if( !( (lkonsPr > 0.6999) && lkonsPr <= 2.5) )
    {
        QMessageBox::about(this, "Некорректное значение", "Величина длины консоли должна находиться \nв интервале значений от 0.7 до 2.5, включительно.");
        ui->EditLkons->setFocus();
 //       qDebug() << "111111 " << lkonsPr;
        return false;

    }


    //возвышение ворот над УВБ в закрытом состоянии
    sa = ui->Editav->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Editav->setFocus();
        return ok;
    }


    //толщина ворот по низу
    sa = ui->Edittvor->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Edittvor->setFocus();
        return ok;
    }


    // шаг по времени
    sa = ui->Editdeltat->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Editdeltat->setFocus();
        return ok;
    }

    //тип оголовка 1 - цилиндрический 2-прямоугольный
    //тип стр козырька 1 - полигональный 2-криволинейный

    //альфа
    sa = ui->Editalfa->text();
    //Может быть пустым
    if( ui->checkBox_3->isChecked())
    {

       float alfaPr = sa.toFloat(&ok);
       if(!ok)
       {
           QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
           ui->Editalfa->setFocus();
           return ok;
       }
       else if(alfaPr < 30 || alfaPr > 70 )
       {
           QMessageBox::about(this, "Некорректное значение", "Величина alfa должна находиться \nв интервале значений от 30 до 70, включительно.");
           ui->Editalfa->setFocus();
           return false;
       }

    }

    //Постоянное значение коэффициента расхода ниж отв.
    if (ui->checkBoxMu1Const->isChecked())
    {
       sa = ui->EditMu1Const->text();
       sa.toFloat(&ok);
       if(!ok)
       {
           QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
           ui->EditMu1Const->setFocus();
           return ok;
       }
    }


    return true;
}

bool MainWindow::testValuesSecond()
{

    QString sa;
    bool ok;

    //начальная глубина в камере
    sa = ui->Edithokam->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->Edithokam->setFocus();
        return ok;
    }


    //длина судна
    sa = ui->EditLsud->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditLsud->setFocus();
        return ok;
    }

    // ширина судна
    sa = ui->EditBsud->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditBsud->setFocus();
        return ok;
    }

    // осадка судна
    sa = ui->EditHsud->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditHsud->setFocus();
        return ok;
    }

    // расст от створа поступления воды до судна
    sa = ui->EditLpolsud->text();
    sa.toFloat(&ok);
    if(!ok)
    {
        QMessageBox::about(this, "Некорректное значение", "Введите числовое значение, \nиспользуя в качестве десятичного разделителя точку");
        ui->EditLpolsud->setFocus();
        return ok;
    }

    return true;
}

void MainWindow::setValues()
{
    QString sa;
    //длина камеры
    sa = ui->EditLk->text();
    Lk = sa.toFloat();

    //ширина камеры
    sa = ui->EditBk->text();
    Bk = sa.toFloat();

    //начальный напор
    sa = ui->EditHo->text();
    Ho = sa.toFloat();

    //глубина на пороге верхней головы
    sa = ui->Edithp->text();
    hp = sa.toFloat();

    // вел заглубления скоса порога по вертикали
    sa = ui->Editp1->text();
    p1 = sa.toFloat();

    // высота уступа порога
    sa = ui->Editp2->text();
    p2 = sa.toFloat();

    //скорость опускания затвора
    sa = ui->EditVvor->text();
    Vvor = sa.toFloat();

    //максимальная величина опускания ворот
    sa = ui->Edithovmax->text();
    hovmax = sa.toFloat();

    // длина консоли ворот
    sa = ui->EditLkons->text();
    Lkons = sa.toFloat();

    //возвышение ворот над УВБ в закрытом состоянии
    sa = ui->Editav->text();
    av = sa.toFloat();

    //толщина ворот по верху
    sa = ui->Edittvor->text();
    tvor = sa.toFloat();

    // шаг по времени
    sa = ui->Editdeltat->text();
    deltat = sa.toFloat();

    // альфа
    sa = ui->Editalfa->text();
    alfa = sa.toFloat();

    //тип оголовка 1 - цилиндрический 2-прямоугольный
    if(ui->checkBox_1->isChecked())
    {
        typeog = 1;
    }
    else if(ui->checkBox_2->isChecked())
    {
        typeog = 2;
    }
    //тип стр козырька 1 - полигональный 2-криволинейный
    if(ui->checkBox_3->isChecked())
    {
        typekoz = 1;
    }
    else if(ui->checkBox_4->isChecked())
    {
        typekoz = 2;
    }

    sa = ui->EditMu1Const->text();
    mu1Const = sa.toFloat();
}

void MainWindow::on_pushButton_2_clicked() //открыть файл с данными
{   
    openPar(false);
}

void MainWindow::openPar(bool isParDefault) //функция открыть файл с данными
{
    QString FileName;
    if (isParDefault)
    {
        FileName = "defaultParam.txt";
    }
    else
    {
       FileName = QFileDialog::getOpenFileName(this,
                                               "Открыть текстовый файл с исходными данными", "",
                                               "Текстовые файлы (*.txt);;Все файлы (*)");


       if(FileName.isEmpty())
       {
           return;
       }
    }

       //Прочитаем файл
       QFile filere(FileName);
       filere.open(QIODevice::ReadOnly);
       QTextStream in(&filere);

       QString sa;

       //длина камеры
       in >> sa;
       ui->EditLk->setText(sa);

       //ширина камеры
       in >> sa;
       ui->EditBk->setText(sa);

       //начальный напор
       in >> sa;
       ui->EditHo->setText(sa);

       //глубина на пороге верхней головы
       in >> sa;
       ui->Edithp->setText(sa);

       // вел заглубления скоса порога по вертикали
       in >> sa;
       ui->Editp1->setText(sa);

       // высота уступа порога
       in >> sa;
       ui->Editp2->setText(sa);


       //скорость опускания затвора
       in >> sa;
       ui->EditVvor->setText(sa);

       //максимальная величина опускания ворот
       in >> sa;
       ui->Edithovmax->setText(sa);

       // длина консоли ворот
       in >> sa;
       ui->EditLkons->setText(sa);

       //возвышение ворот над УВБ в закрытом состоянии
       in >> sa;
       ui->Editav->setText(sa);

       //толщина ворот по верху
       in >> sa;
       ui->Edittvor->setText(sa);

       // шаг по времени
       in >> sa;
       ui->Editdeltat->setText(sa);

       //тип оголовка 1 - цилиндрический 2-прямоугольный
       in >> sa;
       if( sa.toInt() == 1 )
       {
           //цилиндрический тип оголовка
           ui->checkBox_1->setChecked(true);
           ui->checkBox_2->setChecked(false);
       }
       else
       {
           //прямоугольный тип оголовка
           ui->checkBox_2->setChecked(true);
           ui->checkBox_1->setChecked(false);
       }

       //тип стр козырька 1 - полигональный 2-криволинейный
       in >> sa;
       if( sa.toInt() == 1 )
       {
           //полигональный тип струенаправляющего козырька
           ui->checkBox_3->setChecked(true);
           ui->checkBox_4->setChecked(false);
           ui->Editalfa->setVisible(true);
           ui->label_12->setVisible(true);
       }
       else
       {
           //криволинейный тип струенаправляющего козырька
           ui->checkBox_4->setChecked(true);
           ui->checkBox_3->setChecked(false);
           ui->Editalfa->setVisible(false);
           ui->label_12->setVisible(false);
       }

       //альфа
       in >> sa;
       ui->Editalfa->setText(sa);

       in >> sa;
       ui->EditMu1Const->setText(sa);





       //начальная глубина в камере
       in >> sa;
       ui->Edithokam->setText(sa);

       //длина судна
       in >> sa;
       ui->EditLsud->setText(sa);

       // ширина судна
       in >> sa;
       ui->EditBsud->setText(sa);

       // осадка судна
       in >> sa;
       ui->EditHsud->setText(sa);

       // расст от створа поступления воды до судна
       in >> sa;
       ui->EditLpolsud->setText(sa);
}

float MainWindow::mu_25() //коэфф расхода при lk=2.5
{
    float mu30, mu45, mu60;
    mu30 = 0; mu45 = 0; mu60 = 0;
    float lk = 2.5;
    float mu_25 = 0;

    float A=hovi/lk;

    //Если козырёк криволинейной формы
    if(typekoz == 2)
    {
        if (A < 4.2)
        {
            mu_25 = 0.027*(A*A) - 0.298*A + 0.657;
        }
        else if(A >= 4.2)
        {
            mu_25 = 0.12;
        }
    }

    //Если козырёк полигональный
    else
    {

       if(A < 1.4)
       {
           mu30 = 0.237*(A*A) - 0.84*A + 1.003;
           mu45 = 0.117*(A*A) - 0.506*A + 0.773;
           mu60 = -0.074*(A*A) -0.06*A+ 0.523;
       }

       else if (A >= 1.4 && A <=4.2)
       {
           mu30 = -0.009*(A*A*A) + 0.105*(A*A) - 0.417*A + 0.695;
           mu45 = -0.009*(A*A*A) + 0.105*(A*A) - 0.417*A + 0.695;
           mu60 = -0.009*(A*A*A) + 0.105*(A*A) - 0.417*A + 0.695;
       }

       else if(A > 4.2)
       {
           mu30 = 0.12;
           mu45 = 0.12;
           mu60 = 0.12;
       }


       if(alfa >= 30 && alfa < 45)
       {
           mu_25 = ((mu45 - mu30)/(45.0-30.0)) * (alfa-30.0) + mu30;
       }
       else if(alfa >=45 && alfa<60)
       {
           mu_25 = ((mu60 - mu45)/(60.0-45.0)) * (alfa-45.0) + mu45;
       }
       else if(alfa >= 60)
       {
           mu_25 = ((mu60 - mu45)/(60.0-45.0)) * (alfa-60.0) + mu60;
       }
    }

    return mu_25;
}

float MainWindow::mu_15()
{
    float mu30, mu55, mu70;
    mu30 = 0; mu55 = 0; mu70 = 0;
    float lk = 1.5;
    float mu_15 = 0;

    float A=hovi/lk;

    //Если козырёк криволинейной формы
    if(typekoz == 2)
    {
        if (A < 4.2)
        {
            mu_15 = -0.002*(A*A*A*A) + 0.017*(A*A*A) + 0.001*(A*A) - 0.286*A + 0.716;
        }
        else if(A >= 4.2)
        {
            mu_15 = 0.12;
        }
    }

    //Если козырёк полигональный
    else
    {

       if(A < 2.26)
       {
           mu30 = 0.038*(A*A) - 0.313*A + 0.777;
           mu55 = 0.018*(A*A*A)- 0.081*(A*A) - 0.027*A + 0.533;
           mu70 = -0.036*(A*A) + 0.077*A + 0.281;
       }

       else if (A >= 2.26 && A <= 4.2)
       {
           mu30 = 0.025*(A*A) - 0.236*A + 0.672;
           mu55 = 0.025*(A*A) - 0.236*A + 0.672;
           mu70 = 0.025*(A*A) - 0.236*A + 0.672;
       }

       else if(A > 4.2)
       {
           mu30 = 0.12;
           mu55 = 0.12;
           mu70 = 0.12;
       }


       if(alfa >= 30 && alfa < 55)
       {
           mu_15 = ((mu55 - mu30)/(55.0-30.0)) * (alfa-30.0) + mu30;
       }
       else if(alfa >=55 && alfa<=70)
       {
           mu_15 = ((mu70 - mu55)/(70.0-55.0)) * (alfa-55.0) + mu55;
       }
    }


    return mu_15;
}

float MainWindow::mu_07()
{
    float mu30, mu60, mu75;
    mu30 = 0; mu60 = 0; mu75 = 0;
    float lk = 0.7;
    float mu_07 = 0;

    float A=hovi/lk;

    //Если козырёк криволинейной формы
    if(typekoz == 2)
    {
        if (A < 4.2)
        {
            mu_07 = -0.001*(A*A*A*A) + 0.007*(A*A*A) + 0.022*(A*A) - 0.316*A + 0.811;
        }
        else if(A >= 4.2)
        {
            mu_07 = 0.12;
        }
    }

    //Если козырёк полигональный
    else
    {

       if(A < 4.2)
       {
           mu30 = -0.001*(A*A*A*A) + 0.006*(A*A*A) + 0.025*(A*A) - 0.337*A + 0.879;
           mu60 = -0.001*(A*A*A) + 0.039*(A*A) - 0.268*A + 0.68;
           mu75 = 0.008*(A*A) - 0.083*A + 0.325;
       }

       else if(A >= 4.2)
       {
           mu30 = 0.12;
           mu60 = 0.12;
           mu75 = 0.12;
       }


       if(alfa >= 30 && alfa < 60)
       {
           mu_07 = ((mu60 - mu30)/(60.0-30.0)) * (alfa-30.0) + mu30;
       }
       else if(alfa >=60 && alfa<=70)
       {
           mu_07 = ((mu75 - mu60)/(75.0-60.0)) * (alfa-55.0) + mu60;
       }
    }


    return mu_07;
}

float MainWindow::mu1()
{
    float mu07, mu15, mu25, mu1returned;
    mu07=0; mu15=0; mu25=0; mu1returned = 0;


    if(Lkons > 0.7 && Lkons < 1.5)
    {
        mu07 = mu_07();
        mu15 = mu_15();
        mu1returned = ((mu15-mu07)/(1.5 - 0.7)) * (Lkons-0.7) + mu07;
 //       qDebug() << "oooooooooooop 1 ";

    }

    else if(Lkons >= 1.5 && Lkons <= 2.5)
    {
        mu15 = mu_15();
        mu25 = mu_25();
        mu1returned = ((mu25-mu15)/(2.5-1.5)) * (Lkons-1.5) + mu15;
//        qDebug() << "oooooooooooop 3 ";
    }
    else //(Lkons == 0.7)
    {
        mu07 = mu_07();
        mu1returned = mu07;
 //       qDebug() << "oooooooooooop 07 " << mu07;
    }

 //   qDebug() << "oooooooooooop ______ " << mu1returned << "   mu07" << mu07;
    return mu1returned;
}

float MainWindow::mu2() //Коэфф расх верхнего отв.
{
    float mu2returned;
    mu2returned = 0.0;

    float Hoti = hovi-av;

    if(typeog == 1) //цилиндрический оголовок
    {
        mu2returned = 0.453*(qPow(Hoti/tvor, 0.193));
    }
    else if(typeog == 2) //Прямоугольный оголовок
    {
        mu2returned = 0.336*(qPow(Hoti/tvor, 0.321));
    }
    return mu2returned;
}

float MainWindow::sigma()
{
    float sigmaReturned;
    sigmaReturned = 0.0;

    float hppp = hovi - av - htimePrew;
    float AA;

    AA = hppp/(hovi - av);

    sigmaReturned = -0.546 * (qPow(AA, 18)) - 0.07*(qPow(AA, 5)) - 0.384*(AA*AA) + 1.0;

    return sigmaReturned;
}





void MainWindow::on_pushButton_4_clicked()
{
   /* Gra->showNormal();
    Gra->setWindowTitle("График");
    PlotQ2();
    */
    testValuesMain();
}

void MainWindow::PlotHi()
{
    Gra->setWindowTitle("График зависимости напора от времени");

    float tim = 0.0;
    float timPrew = 0.0;
    float tim_max;
    float hh, hhPrew;
    hhPrew = 0.0;

    float Mavert, Mahor;
    Mavert = 1.0;
    Mahor = 1.0;

    Gra->Scene1->clear();


     //Масштабируем
         float h, w;
     h = 400;
     w = 600;

     Mahor =  w/(ntime*1.2);
     Mavert =  h/(htime_max*1.5);

     QRect rec(-50,-h/2,w+50,50);    //left, top, width, height
     Gra->Scene1->setSceneRect(rec);

     tim_max = ntime*Mahor;
     float Y_max = htime_max*Mavert;

     if(showEventsFlag)
     {
     QPen pen1;
     pen1.setColor(Qt::green);

     Gra->Scene1->addLine(timeOstanZatv*Mahor, 0, timeOstanZatv*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex1 = Gra->Scene1->addText("стоп затвор");
     tex1->setTransform(tex1->transform().rotate(-90));
     tex1->setPos(timeOstanZatv*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeNijPodt*Mahor, 0, timeNijPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex2 = Gra->Scene1->addText("подтопл. ниж.");
     tex2->setTransform(tex2->transform().rotate(-90));
     tex2->setPos(timeNijPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeVerhPodt*Mahor, 0, timeVerhPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex3 = Gra->Scene1->addText("подтопл. верх.");
     tex3->setTransform(tex3->transform().rotate(-90));
     tex3->setPos(timeVerhPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(TimeVerhRash*Mahor, 0, TimeVerhRash*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex4 = Gra->Scene1->addText("Q2 > 0");
     tex4->setTransform(tex4->transform().rotate(-90));
     tex4->setPos(TimeVerhRash*Mahor-14,-Y_max-60);
     }


     Gra->Scene1->addLine(0,0,0,-Y_max-150);
     Gra->Scene1->addLine(0,-Y_max-150,3,-Y_max-143);
     Gra->Scene1->addLine(0,-Y_max-150,-3,-Y_max-143);


     float i = 0.0;
     float co = 0.0;
     while(i < Y_max+120)
     {
         Gra->Scene1->addLine(0, -i, -4, -i);

         QPen Pen1;
         Pen1.setColor(Qt::gray);
         Pen1.setStyle(Qt::DashLine);
         Gra->Scene1->addLine(0, -i, tim_max, -i, Pen1);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(-30, -i-12);

         i = i + 2.0*Mavert;
         co = co + 2.0;
     }


     Gra->Scene1->addLine(0,0,tim_max+100,0);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93,-3);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93, 3);


     i = 0.0;
     co = 0.0;

     float horticksVa;
     horticksVa = 50/deltat;

     while(i < tim_max +80)
     {
         Gra->Scene1->addLine(i, 0, i, 4);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(i-7, 10);

         i = i + horticksVa*Mahor;
         co = co + horticksVa*deltat;
     }


     for(int i = 0; i<=ntime; i++)
     {
         hh = htimeVec[i]*Mavert;
//         Gra->Scene1->addEllipse(tim-2,-hh-2,4,4);
         if(i!=0)
         {
         Gra->Scene1->addLine(timPrew, -hhPrew, tim, -hh);
         }

         timPrew = tim;
         tim = tim + 1*Mahor;
         hhPrew = hh;
     }

     qDebug() << "zat " << timeOstanZatv*deltat << "   npodt " << timeNijPodt*deltat << "    verPodt " << timeVerhPodt*deltat << "   Verrash " << TimeVerhRash*deltat;

     Gra->FocusButtons();

}

void MainWindow::PlotQ1()
{
    Gra->setWindowTitle("График зависимости расхода через нижнее отверстие от времени");

    float tim = 0.0;
    float timPrew = 0.0;
    float tim_max;
    float hh, hhPrew;
    hhPrew = 0.0;

    float Mavert, Mahor;
    Mavert = 1.0;
    Mahor = 1.0;

    Gra->Scene1->clear();

     //Масштабируем
         float h, w;
     h = 400;
     w = 600;

     Mahor =  w/(ntime*1.2);
     Mavert =  h/(Q1_max*1.5);

     QRect rec(-50,-h/2,w+50,50);    //left, top, width, height
     Gra->Scene1->setSceneRect(rec);

     tim_max = ntime*Mahor;
     float Y_max = Q1_max*Mavert;

     if(showEventsFlag)
     {
     QPen pen1;
     pen1.setColor(Qt::green);

     Gra->Scene1->addLine(timeOstanZatv*Mahor, 0, timeOstanZatv*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex1 = Gra->Scene1->addText("стоп затвор");
     tex1->setTransform(tex1->transform().rotate(-90));
     tex1->setPos(timeOstanZatv*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeNijPodt*Mahor, 0, timeNijPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex2 = Gra->Scene1->addText("подтопл. ниж.");
     tex2->setTransform(tex2->transform().rotate(-90));
     tex2->setPos(timeNijPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeVerhPodt*Mahor, 0, timeVerhPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex3 = Gra->Scene1->addText("подтопл. верх.");
     tex3->setTransform(tex3->transform().rotate(-90));
     tex3->setPos(timeVerhPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(TimeVerhRash*Mahor, 0, TimeVerhRash*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex4 = Gra->Scene1->addText("Q2 > 0");
     tex4->setTransform(tex4->transform().rotate(-90));
     tex4->setPos(TimeVerhRash*Mahor-14,-Y_max-60);
     }


     Gra->Scene1->addLine(0,0,0,-Y_max-150);
     Gra->Scene1->addLine(0,-Y_max-150,3,-Y_max-143);
     Gra->Scene1->addLine(0,-Y_max-150,-3,-Y_max-143);


     float i = 0.0;
     float co = 0.0;
     while(i < Y_max+120)
     {
         Gra->Scene1->addLine(0, -i, -4, -i);

         QPen Pen1;
         Pen1.setColor(Qt::gray);
         Pen1.setStyle(Qt::DashLine);
         Gra->Scene1->addLine(0, -i, tim_max, -i, Pen1);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(-30, -i-12);

         i = i + 20.0*Mavert;
         co = co + 20.0;
     }


     Gra->Scene1->addLine(0,0,tim_max+100,0);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93,-3);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93, 3);


     i = 0.0;
     co = 0.0;

     float horticksVa;
     horticksVa = 50/deltat;

     while(i < tim_max +80)
     {
         Gra->Scene1->addLine(i, 0, i, 4);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(i-7, 10);

         i = i + horticksVa*Mahor;
         co = co + horticksVa*deltat;
     }

     for(int i = 0; i<=ntime; i++)
     {
         hh = Q1timeVec[i]*Mavert;
//         Gra->Scene1->addEllipse(tim-2,-hh-2,4,4);
         if(i!=0)
         {
         Gra->Scene1->addLine(timPrew, -hhPrew, tim, -hh);
         }

         timPrew = tim;
         tim = tim + 1*Mahor;
         hhPrew = hh;
     }

     Gra->FocusButtons();

}

void MainWindow::PlotQ2()
{
    Gra->setWindowTitle("График зависимости расхода через верхнее отверстие от времени");

    float tim = 0.0;
    float timPrew = 0.0;
    float tim_max;
    float hh, hhPrew;
    hhPrew = 0.0;

    float Mavert, Mahor;
    Mavert = 1.0;
    Mahor = 1.0;

    Gra->Scene1->clear();

     //Масштабируем
         float h, w;
     h = 400;
     w = 600;

     Mahor =  w/(ntime*1.2);
     Mavert =  h/(Q1_max*1.5);

     QRect rec(-50,-h/2,w+50,50);    //left, top, width, height
     Gra->Scene1->setSceneRect(rec);

     tim_max = ntime*Mahor;
     float Y_max = Q1_max*Mavert;

     if(showEventsFlag)
     {
     QPen pen1;
     pen1.setColor(Qt::green);

     Gra->Scene1->addLine(timeOstanZatv*Mahor, 0, timeOstanZatv*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex1 = Gra->Scene1->addText("стоп затвор");
     tex1->setTransform(tex1->transform().rotate(-90));
     tex1->setPos(timeOstanZatv*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeNijPodt*Mahor, 0, timeNijPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex2 = Gra->Scene1->addText("подтопл. ниж.");
     tex2->setTransform(tex2->transform().rotate(-90));
     tex2->setPos(timeNijPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeVerhPodt*Mahor, 0, timeVerhPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex3 = Gra->Scene1->addText("подтопл. верх.");
     tex3->setTransform(tex3->transform().rotate(-90));
     tex3->setPos(timeVerhPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(TimeVerhRash*Mahor, 0, TimeVerhRash*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex4 = Gra->Scene1->addText("Q2 > 0");
     tex4->setTransform(tex4->transform().rotate(-90));
     tex4->setPos(TimeVerhRash*Mahor-14,-Y_max-60);
     }


     Gra->Scene1->addLine(0,0,0,-Y_max-150);
     Gra->Scene1->addLine(0,-Y_max-150,3,-Y_max-143);
     Gra->Scene1->addLine(0,-Y_max-150,-3,-Y_max-143);


     float i = 0.0;
     float co = 0.0;
     while(i < Y_max+120)
     {
         Gra->Scene1->addLine(0, -i, -4, -i);

         QPen Pen1;
         Pen1.setColor(Qt::gray);
         Pen1.setStyle(Qt::DashLine);
         Gra->Scene1->addLine(0, -i, tim_max, -i, Pen1);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(-30, -i-12);

         i = i + 20.0*Mavert;
         co = co + 20.0;
     }


     Gra->Scene1->addLine(0,0,tim_max+100,0);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93,-3);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93, 3);


     i = 0.0;
     co = 0.0;

     float horticksVa;
     horticksVa = 50/deltat;

     while(i < tim_max +80)
     {
         Gra->Scene1->addLine(i, 0, i, 4);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(i-7, 10);

         i = i + horticksVa*Mahor;
         co = co + horticksVa*deltat;
     }

     for(int i = 0; i<=ntime; i++)
     {
         hh = Q2timeVec[i]*Mavert;
//         Gra->Scene1->addEllipse(tim-2,-hh-2,4,4);
         if(i!=0)
         {
         Gra->Scene1->addLine(timPrew, -hhPrew, tim, -hh);
         }

         timPrew = tim;
         tim = tim + 1*Mahor;
         hhPrew = hh;
     }

     Gra->FocusButtons();
}


void MainWindow::PlotQ()
{
    Gra->setWindowTitle("График зависимости суммарного расхода от времени");

    float tim = 0.0;
    float timPrew = 0.0;
    float tim_max;
    float hh, hhPrew;
    hhPrew = 0.0;

    float Mavert, Mahor;
    Mavert = 1.0;
    Mahor = 1.0;

    Gra->Scene1->clear();

     //Масштабируем
         float h, w;
     h = 400;
     w = 600;

     Mahor =  w/(ntime*1.2);
     float Q_max = Q1_max + Q2_max;

     Mavert =  h/(Q_max*1.5);

     QRect rec(-50,-h/2,w+50,50);    //left, top, width, height
     Gra->Scene1->setSceneRect(rec);

     tim_max = ntime*Mahor;
     float Y_max = Q_max*Mavert;

     if(showEventsFlag)
     {
     QPen pen1;
     pen1.setColor(Qt::green);

     Gra->Scene1->addLine(timeOstanZatv*Mahor, 0, timeOstanZatv*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex1 = Gra->Scene1->addText("стоп затвор");
     tex1->setTransform(tex1->transform().rotate(-90));
     tex1->setPos(timeOstanZatv*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeNijPodt*Mahor, 0, timeNijPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex2 = Gra->Scene1->addText("подтопл. ниж.");
     tex2->setTransform(tex2->transform().rotate(-90));
     tex2->setPos(timeNijPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeVerhPodt*Mahor, 0, timeVerhPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex3 = Gra->Scene1->addText("подтопл. верх.");
     tex3->setTransform(tex3->transform().rotate(-90));
     tex3->setPos(timeVerhPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(TimeVerhRash*Mahor, 0, TimeVerhRash*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex4 = Gra->Scene1->addText("Q2 > 0");
     tex4->setTransform(tex4->transform().rotate(-90));
     tex4->setPos(TimeVerhRash*Mahor-14,-Y_max-60);
     }



     Gra->Scene1->addLine(0,0,0,-Y_max-150);
     Gra->Scene1->addLine(0,-Y_max-150,3,-Y_max-143);
     Gra->Scene1->addLine(0,-Y_max-150,-3,-Y_max-143);


     float i = 0.0;
     float co = 0.0;
     while(i < Y_max+120)
     {
         Gra->Scene1->addLine(0, -i, -4, -i);

         QPen Pen1;
         Pen1.setColor(Qt::gray);
         Pen1.setStyle(Qt::DashLine);
         Gra->Scene1->addLine(0, -i, tim_max, -i, Pen1);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(-30, -i-12);

         i = i + 20.0*Mavert;
         co = co + 20.0;
     }


     Gra->Scene1->addLine(0,0,tim_max+100,0);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93,-3);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93, 3);


     i = 0.0;
     co = 0.0;

     float horticksVa;
     horticksVa = 50/deltat;

     while(i < tim_max +80)
     {
         Gra->Scene1->addLine(i, 0, i, 4);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(i-7, 10);

         i = i + horticksVa*Mahor;
         co = co + horticksVa*deltat;
     }

     for(int i = 0; i<=ntime; i++)
     {
         hh = (Q1timeVec[i] + Q2timeVec[i])*Mavert;
//         Gra->Scene1->addEllipse(tim-2,-hh-2,4,4);
         if(i!=0)
         {
         Gra->Scene1->addLine(timPrew, -hhPrew, tim, -hh);
         }

         timPrew = tim;
         tim = tim + 1*Mahor;
         hhPrew = hh;
     }

     Gra->FocusButtons();

}

void MainWindow::PlotMu1()
{
    Gra->setWindowTitle("График зависимости коэффициента расхода нижнего отверстия от времени");


    float tim = 0.0;
    float timPrew = 0.0;
    float tim_max;
    float hh, hhPrew;
    hhPrew = 0.0;

    float Mavert, Mahor;
    Mavert = 1.0;
    Mahor = 1.0;

    Gra->Scene1->clear();

     //Масштабируем
         float h, w;
     h = 400;
     w = 600;

     Mahor =  w/(ntime*1.2);
     Mavert =  h/(mu1_max*1.5);

     QRect rec(-50,-h/2,w+50,50);    //left, top, width, height
     Gra->Scene1->setSceneRect(rec);

     tim_max = ntime*Mahor;
     float Y_max = mu1_max*Mavert;

     if(showEventsFlag)
     {
     QPen pen1;
     pen1.setColor(Qt::green);

     Gra->Scene1->addLine(timeOstanZatv*Mahor, 0, timeOstanZatv*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex1 = Gra->Scene1->addText("стоп затвор");
     tex1->setTransform(tex1->transform().rotate(-90));
     tex1->setPos(timeOstanZatv*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeNijPodt*Mahor, 0, timeNijPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex2 = Gra->Scene1->addText("подтопл. ниж.");
     tex2->setTransform(tex2->transform().rotate(-90));
     tex2->setPos(timeNijPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeVerhPodt*Mahor, 0, timeVerhPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex3 = Gra->Scene1->addText("подтопл. верх.");
     tex3->setTransform(tex3->transform().rotate(-90));
     tex3->setPos(timeVerhPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(TimeVerhRash*Mahor, 0, TimeVerhRash*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex4 = Gra->Scene1->addText("Q2 > 0");
     tex4->setTransform(tex4->transform().rotate(-90));
     tex4->setPos(TimeVerhRash*Mahor-14,-Y_max-60);
     }



     Gra->Scene1->addLine(0,0,0,-Y_max-150);
     Gra->Scene1->addLine(0,-Y_max-150,3,-Y_max-143);
     Gra->Scene1->addLine(0,-Y_max-150,-3,-Y_max-143);



     float i = 0.0;
     float co = 0.0;
     while(i < Y_max+120)
     {
         Gra->Scene1->addLine(0, -i, -4, -i);

         QPen Pen1;
         Pen1.setColor(Qt::gray);
         Pen1.setStyle(Qt::DashLine);
         Gra->Scene1->addLine(0, -i, tim_max, -i, Pen1);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(-30, -i-12);

         i = i + 0.2*Mavert;
         co = co + 0.2;
     }


     Gra->Scene1->addLine(0,0,tim_max+100,0);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93,-3);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93, 3);


     i = 0.0;
     co = 0.0;

     float horticksVa;
     horticksVa = 50/deltat;

     while(i < tim_max +80)
     {
         Gra->Scene1->addLine(i, 0, i, 4);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(i-7, 10);

         i = i + horticksVa*Mahor;
         co = co + horticksVa*deltat;
     }


     for(int i = 0; i<=ntime; i++)
     {
         hh = mu1timeVec[i] *Mavert;
//         Gra->Scene1->addEllipse(tim-2,-hh-2,4,4);
         if(i > 1)
         {
         Gra->Scene1->addLine(timPrew, -hhPrew, tim, -hh);
         }

         timPrew = tim;
         tim = tim + 1*Mahor;
         hhPrew = hh;
     }

     Gra->FocusButtons();

}

void MainWindow::PlotMu2()
{
    Gra->setWindowTitle("График зависимости коэффициента расхода верхнего отверстия от времени");

    float tim = 0.0;
    float timPrew = 0.0;
    float tim_max;
    float hh, hhPrew;
    hhPrew = 0.0;

    float Mavert, Mahor;
    Mavert = 1.0;
    Mahor = 1.0;

    Gra->Scene1->clear();

     //Масштабируем
         float h, w;
     h = 400;
     w = 600;

     Mahor =  w/(ntime*1.2);
     Mavert =  h/(mu2_max*1.5);

     QRect rec(-50,-h/2,w+50,50);    //left, top, width, height
     Gra->Scene1->setSceneRect(rec);

     tim_max = ntime*Mahor;
     float Y_max = mu2_max*Mavert;

     Gra->Scene1->addLine(0,0,0,-Y_max-150);
     Gra->Scene1->addLine(0,-Y_max-150,3,-Y_max-143);
     Gra->Scene1->addLine(0,-Y_max-150,-3,-Y_max-143);

     if(showEventsFlag)
     {
     QPen pen1;
     pen1.setColor(Qt::green);

     Gra->Scene1->addLine(timeOstanZatv*Mahor, 0, timeOstanZatv*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex1 = Gra->Scene1->addText("стоп затвор");
     tex1->setTransform(tex1->transform().rotate(-90));
     tex1->setPos(timeOstanZatv*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeNijPodt*Mahor, 0, timeNijPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex2 = Gra->Scene1->addText("подтопл. ниж.");
     tex2->setTransform(tex2->transform().rotate(-90));
     tex2->setPos(timeNijPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeVerhPodt*Mahor, 0, timeVerhPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex3 = Gra->Scene1->addText("подтопл. верх.");
     tex3->setTransform(tex3->transform().rotate(-90));
     tex3->setPos(timeVerhPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(TimeVerhRash*Mahor, 0, TimeVerhRash*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex4 = Gra->Scene1->addText("Q2 > 0");
     tex4->setTransform(tex4->transform().rotate(-90));
     tex4->setPos(TimeVerhRash*Mahor-14,-Y_max-60);
     }


     float i = 0.0;
     float co = 0.0;
     while(i < Y_max+120)
     {
         Gra->Scene1->addLine(0, -i, -4, -i);

         QPen Pen1;
         Pen1.setColor(Qt::gray);
         Pen1.setStyle(Qt::DashLine);
         Gra->Scene1->addLine(0, -i, tim_max, -i, Pen1);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(-30, -i-12);

         i = i + 0.1*Mavert;
         co = co + 0.1;
     }


     Gra->Scene1->addLine(0,0,tim_max+100,0);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93,-3);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93, 3);


     i = 0.0;
     co = 0.0;

     float horticksVa;
     horticksVa = 50/deltat;

     while(i < tim_max +80)
     {
         Gra->Scene1->addLine(i, 0, i, 4);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(i-7, 10);

         i = i + horticksVa*Mahor;
         co = co + horticksVa*deltat;
     }

     for(int i = 0; i<=ntime; i++)
     {
         hh = mu2timeVec[i] *Mavert;
//         Gra->Scene1->addEllipse(tim-2,-hh-2,4,4);
         if(i!=0)
         {
         Gra->Scene1->addLine(timPrew, -hhPrew, tim, -hh);
         }

         timPrew = tim;
         tim = tim + 1*Mahor;
         hhPrew = hh;
     }


     Gra->FocusButtons();
}

void MainWindow::PlotSigma()
{
    Gra->setWindowTitle("График зависимости коэффициента подтопления от времени");

    float tim = 0.0;
    float timPrew = 0.0;
    float tim_max;
    float hh, hhPrew;
    hhPrew = 0.0;

    float Mavert, Mahor;
    Mavert = 1.0;
    Mahor = 1.0;

    Gra->Scene1->clear();

     //Масштабируем
         float h, w;
     h = 400;
     w = 600;

     Mahor =  w/(ntime*1.2);
     Mavert =  h/(sigma_max*1.5);

     QRect rec(-50,-h/2,w+50,50);    //left, top, width, height
     Gra->Scene1->setSceneRect(rec);

     tim_max = ntime*Mahor;
     float Y_max = sigma_max*Mavert;

     Gra->Scene1->addLine(0,0,0,-Y_max-150);
     Gra->Scene1->addLine(0,-Y_max-150,3,-Y_max-143);
     Gra->Scene1->addLine(0,-Y_max-150,-3,-Y_max-143);

     if(showEventsFlag)
     {
     QPen pen1;
     pen1.setColor(Qt::green);

     Gra->Scene1->addLine(timeOstanZatv*Mahor, 0, timeOstanZatv*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex1 = Gra->Scene1->addText("стоп затвор");
     tex1->setTransform(tex1->transform().rotate(-90));
     tex1->setPos(timeOstanZatv*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeNijPodt*Mahor, 0, timeNijPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex2 = Gra->Scene1->addText("подтопл. ниж.");
     tex2->setTransform(tex2->transform().rotate(-90));
     tex2->setPos(timeNijPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(timeVerhPodt*Mahor, 0, timeVerhPodt*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex3 = Gra->Scene1->addText("подтопл. верх.");
     tex3->setTransform(tex3->transform().rotate(-90));
     tex3->setPos(timeVerhPodt*Mahor-14,-Y_max-60);

     Gra->Scene1->addLine(TimeVerhRash*Mahor, 0, TimeVerhRash*Mahor, -Y_max-40, pen1);
     QGraphicsTextItem *tex4 = Gra->Scene1->addText("Q2 > 0");
     tex4->setTransform(tex4->transform().rotate(-90));
     tex4->setPos(TimeVerhRash*Mahor-14,-Y_max-60);
     }


     float i = 0.0;
     float co = 0.0;
     while(i < Y_max+120)
     {
         Gra->Scene1->addLine(0, -i, -4, -i);

         QPen Pen1;
         Pen1.setColor(Qt::gray);
         Pen1.setStyle(Qt::DashLine);
         Gra->Scene1->addLine(0, -i, tim_max, -i, Pen1);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(-30, -i-12);

         i = i + 0.2*Mavert;
         co = co + 0.2;
     }


     Gra->Scene1->addLine(0,0,tim_max+100,0);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93,-3);
     Gra->Scene1->addLine(tim_max+100,0,tim_max+93, 3);


     i = 0.0;
     co = 0.0;

     float horticksVa;
     horticksVa = 50/deltat;

     while(i < tim_max +80)
     {
         Gra->Scene1->addLine(i, 0, i, 4);

         QString ss;
         ss.setNum(co);
         QGraphicsTextItem *texcoo = Gra->Scene1->addText(ss);
         texcoo->setPos(i-7, 10);

         i = i + horticksVa*Mahor;
         co = co + horticksVa*deltat;
     }

     for(int i = 0; i<=ntime; i++)
     {
         hh = sigmaVec[i] *Mavert;
//         Gra->Scene1->addEllipse(tim-2,-hh-2,4,4);
         if(i!=0)
         {
         Gra->Scene1->addLine(timPrew, -hhPrew, tim, -hh);
         }

         timPrew = tim;
         tim = tim + 1*Mahor;
         hhPrew = hh;
     }

     qDebug() << "sigm_max: " << sigma_max;

     Gra->FocusButtons();

}


void MainWindow::trigEventPlot()
{
    showEventsFlag = !showEventsFlag;
    PlotAfterEval();
}


void MainWindow::SaveToCsv()  //Сохраняет результаты расчёта в csv
{
    //Для смены десятичного разделителя на запятую
    QLocale Loc;
   // Loc.decimalPoint();

    QString FileName = QFileDialog::getSaveFileName(this,
                                                        "Сохранить текстовый файл с результатами расчёта", "res.csv",
                                                        "данные, разделённые точкой с запятой (*.csv);;Все файлы (*)");
    if (FileName.isEmpty())
        {
            return;
        }

    QFile filewr(FileName);
    filewr.open(QIODevice::WriteOnly);
    QTextStream out(&filewr);

    out.setLocale(Loc);

    out << "time" << ";" << "Q" << ";" <<  "Q1" << ";" << "Q2" << ";" << "mu1" << ";" << "mu2" << ";" << "sigma" << "\n";

    for(int i = 0; i<=ntime; i++)
    {

        out << i*deltat << ";" << Q1timeVec[i] + Q2timeVec[i] << ";" << Q1timeVec[i] << ";" << Q2timeVec[i] << ";" << mu1timeVec[i] << ";" << mu2timeVec[i] << ";" << sigmaVec[i] << "\n";
    }

   // qDebug() << " ___: " << Loc.decimalPoint();
}


void MainWindow::SaveToWaves() //Сохраняет результаты для программы Waves
{
    //сначала проверим введённые значения
    if (!testValuesSecond())
    {
        return;
    }

    QString FileName = QFileDialog::getSaveFileName(this,
                                                        "Сохранить файл с результатами расчёта для программы Waves", "res.wdt",
                                                        "файлы программы Waves (*.wdt);;Все файлы (*)");


    if (FileName.isEmpty())
        {
            return;
        }

    QFile filewr(FileName);
    filewr.open(QIODevice::WriteOnly);
    QTextStream out(&filewr);

    out << Lk << "\n";        //Длина камеры шлюза
    out << Bk << "\n";        //Ширина камеры шлюза
    out << ui->Edithokam->text() << "\n";
    out << ui->EditLsud->text() << "\n";
    out << ui->EditBsud->text() << "\n";
    out << ui->EditHsud->text() << "\n";
    out << ui->EditLpolsud->text() << "\n";

    for(int i = 0; i<=ntime; i++)
    {
        out << i*deltat << "\n";
        out << Q1timeVec[i] + Q2timeVec[i] << "\n";
    }

}

void MainWindow::SaveToULP()
{
    ;
}




void MainWindow::closeEvent(QCloseEvent *)
{
    Gra->close();
}


void MainWindow::on_checkBoxMu1Const_clicked()
{
    ui->EditMu1Const->setVisible(!ui->EditMu1Const->isVisible());
    ui->label_22->setVisible(!ui->label_22->isVisible());
}

void MainWindow::PlotAfterEval()
{
    if(Gra->numGraph == 1)
    {
        PlotHi();
    }
    else if(Gra->numGraph == 2)
    {
        PlotQ1();
    }
    else if(Gra->numGraph == 3)
    {
        PlotQ2();
    }
    else if(Gra->numGraph == 4)
    {
        PlotQ();
    }
    else if(Gra->numGraph == 5)
    {
        PlotMu1();
    }
    else if(Gra->numGraph == 6)
    {
        PlotMu2();
    }
    else
    {
        PlotSigma();
    }
}
