#include "xo_game.h"
#include "./ui_xo_game.h"

#include <math.h>
#include "cmath"

#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>


XO_game::XO_game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XO_game)
{
    ui->setupUi(this);

    Timer=new QTimer(this);//таймер на всё время активности программы
    Timer->setInterval(100);//на цикл по 100
    Timer->start();//Стартует
    connect(Timer, SIGNAL(timeout()), this, SLOT(onTimer()));

    rnd = 13;
    index = 0;
    symbol = "O";
    symbol2 = "X";

    fullTimer = 0;
    noTimer = 0;

    for(int i=0; i<26; i++)    {        step_m[i]=0; step_ii[i]=0;    }

    ui->pb_4->setEnabled(false); ui->pb_5->setEnabled(false); ui->pb_9->setEnabled(false); ui->pb_10->setEnabled(false);
    ui->pb_14->setEnabled(false); ui->pb_15->setEnabled(false); ui->pb_16->setEnabled(false); ui->pb_17->setEnabled(false);
    ui->pb_18->setEnabled(false); ui->pb_19->setEnabled(false); ui->pb_20->setEnabled(false); ui->pb_21->setEnabled(false);
    ui->pb_22->setEnabled(false); ui->pb_23->setEnabled(false); ui->pb_24->setEnabled(false); ui->pb_25->setEnabled(false);
}

XO_game::~XO_game()
{
    delete ui;
}


//int p1 = 0;
void XO_game::onTimer()
{
    fullTimer++;
    mainlogic();
    mir();
}
void XO_game::mainlogic()
{
    switch (index)
    {

    //Ждём действий
    case 0:
        //проверяем, чем играет игрок 1
        if (ui->rb_x->isChecked()){symbol = "X";}
        else {symbol = "O";}
        fullTimer = 0;
        noTimer=fullTimer;
        if (ui->demoButton->isChecked()) index = 1;
    break;

    //Проверка на победу
    case 1:
        if (ui->demoButton->isChecked()) {rnd = 7; ii2_run();}
        winner();
    //Если нет, то ходим
    if (index != 0 && !ui->demoButton->isChecked())
    {fullTimer = 0; index = 2;}
        break;
    //Ход компьютера
    case 2:
        if (ui->rb_m->isChecked())
        {
            //проверяем, чем играет игрок 2
            if (ui->rb_x->isChecked()){symbol = "O";}
            else {symbol = "X";}
        }
        else {rnd = 13; ii_run();}
        winner2();
    break;

    default:
        break;
    }

}

//следим за настройками
void XO_game::on_rb_3_clicked(bool checked){    ui->rb_5->setChecked(false);
                                                ui->pb_4->setEnabled(false); ui->pb_5->setEnabled(false); ui->pb_9->setEnabled(false); ui->pb_10->setEnabled(false);
                                                ui->pb_14->setEnabled(false); ui->pb_15->setEnabled(false); ui->pb_16->setEnabled(false); ui->pb_17->setEnabled(false);
                                                ui->pb_18->setEnabled(false); ui->pb_19->setEnabled(false); ui->pb_20->setEnabled(false); ui->pb_21->setEnabled(false);
                                                ui->pb_22->setEnabled(false); ui->pb_23->setEnabled(false); ui->pb_24->setEnabled(false); ui->pb_25->setEnabled(false);}
void XO_game::on_rb_5_clicked(bool checked){    ui->rb_3->setChecked(false);
                                                ui->pb_4->setEnabled(true); ui->pb_5->setEnabled(true); ui->pb_9->setEnabled(true); ui->pb_10->setEnabled(true);
                                                ui->pb_14->setEnabled(true); ui->pb_15->setEnabled(true); ui->pb_16->setEnabled(true); ui->pb_17->setEnabled(true);
                                                ui->pb_18->setEnabled(true); ui->pb_19->setEnabled(true); ui->pb_20->setEnabled(true); ui->pb_21->setEnabled(true);
                                                ui->pb_22->setEnabled(true); ui->pb_23->setEnabled(true); ui->pb_24->setEnabled(true); ui->pb_25->setEnabled(true);}
void XO_game::on_rb_o_clicked(bool checked){    ui->rb_x->setChecked(false);}
void XO_game::on_rb_x_clicked(bool checked){    ui->rb_o->setChecked(false);}
void XO_game::on_rb_m_clicked(bool checked){   ui->rb_ii->setChecked(false);}
void XO_game::on_rb_ii_clicked(bool checked){   ui->rb_m->setChecked(false);}

//Отключаем натройки после начала игры
void XO_game::seting_disable()
{    ui->rb_3->setEnabled(false); ui->rb_5->setEnabled(false); ui->rb_o->setEnabled(false); ui->rb_x->setEnabled(false);}
void XO_game::seting_enable()
{    ui->rb_3->setEnabled(true); ui->rb_5->setEnabled(true); ui->rb_o->setEnabled(true); ui->rb_x->setEnabled(true);}

//Проверка на победителя
void XO_game::winner()
{
  //Если игра 3х3
  if (ui->rb_3->isChecked())
  {
    //проверяем горизонтали
     if((step_m[1]>0 && step_m[2]>0 && step_m[3]>0)||(step_m[6]>0 && step_m[7]>0 && step_m[8]>0)||(step_m[11]>0 && step_m[12]>0 && step_m[13]>0))
    {
    ui->l_win->setText("горизонталь"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Вы победили по горизонтали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
     }
     //проверяем вертикали
     if((step_m[1]>0 && step_m[6]>0 && step_m[11]>0)||(step_m[2]>0 && step_m[7]>0 && step_m[12]>0)||(step_m[3]>0 && step_m[8]>0 && step_m[13]>0))
    {
    ui->l_win->setText("вертикаль"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Вы победили по вертикали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
     }
     //проверяем диагонали
     if((step_m[1]>0 && step_m[7]>0 && step_m[13]>0)||(step_m[3]>0 && step_m[7]>0 && step_m[11]>0))
    {
    ui->l_win->setText("диагональ"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Вы победили по диагонали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
     }
   }
  //Если игра 5х5
  if (ui->rb_5->isChecked())
  {
    //проверяем горизонтали
     if((step_m[1]>0 && step_m[2]>0 && step_m[3]>0 && step_m[4]>0 && step_m[5]>0)||
        (step_m[6]>0 && step_m[7]>0 && step_m[8]>0 && step_m[9]>0 && step_m[10]>0)||
        (step_m[11]>0 && step_m[12]>0 && step_m[13]>0 && step_m[14]>0 && step_m[15]>0)||
        (step_m[16]>0 && step_m[17]>0 && step_m[18]>0 && step_m[19]>0 && step_m[20]>0)||
        (step_m[21]>0 && step_m[22]>0 && step_m[23]>0 && step_m[24]>0 && step_m[25]>0))
    {
    ui->l_win->setText("горизонталь"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Вы победили по горизонтали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
     }
     //проверяем вертикали
     if((step_m[1]>0 && step_m[6]>0 && step_m[11]>0 && step_m[16]>0 && step_m[21]>0)||
        (step_m[2]>0 && step_m[7]>0 && step_m[12]>0 && step_m[17]>0 && step_m[22]>0)||
        (step_m[3]>0 && step_m[8]>0 && step_m[13]>0 && step_m[18]>0 && step_m[23]>0)||
        (step_m[4]>0 && step_m[9]>0 && step_m[14]>0 && step_m[19]>0 && step_m[24]>0)||
        (step_m[5]>0 && step_m[10]>0 && step_m[15]>0 && step_m[20]>0 && step_m[25]>0))
    {
    ui->l_win->setText("вертикаль"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Вы победили по вертикали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
     }
     //проверяем диагонали
     if((step_m[1]>0 && step_m[7]>0 && step_m[13]>0 && step_m[19]>0 && step_m[25]>0)||
        (step_m[5]>0 && step_m[9]>0 && step_m[13]>0 && step_m[17]>0 && step_m[21]>0))
    {
    ui->l_win->setText("диагональ"); ui->demoButton->setChecked(false); index = 0;int n = QMessageBox::warning(0,"ПОБЕДА","Вы победили по диагонали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
     }
   }
}
void XO_game::winner2()
{
    //Если игра 3х3
    if (ui->rb_3->isChecked())
    {
      //проверяем горизонтали
       if((step_ii[1]>0 && step_ii[2]>0 && step_ii[3]>0)||(step_ii[6]>0 && step_ii[7]>0 && step_ii[8]>0)||(step_ii[11]>0 && step_ii[12]>0 && step_ii[13]>0))
      {
      ui->l_win->setText("горизонталь"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Игрок 2 победил по горизонтали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем вертикали
       if((step_ii[1]>0 && step_ii[6]>0 && step_ii[11]>0)||(step_ii[2]>0 && step_ii[7]>0 && step_ii[12]>0)||(step_ii[3]>0 && step_ii[8]>0 && step_ii[13]>0))
      {
      ui->l_win->setText("вертикаль"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Игрок 2 победил по вертикали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем диагонали
       if((step_ii[1]>0 && step_ii[7]>0 && step_ii[13]>0)||(step_ii[3]>0 && step_ii[7]>0 && step_ii[11]>0))
      {
      ui->l_win->setText("диагональ"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Игрок 2 победил по диагонали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
     }
    //Если игра 5х5
    if (ui->rb_5->isChecked())
    {
      //проверяем горизонтали
       if((step_ii[1]>0 && step_ii[2]>0 && step_ii[3]>0 && step_ii[4]>0 && step_ii[5]>0)||
          (step_ii[6]>0 && step_ii[7]>0 && step_ii[8]>0 && step_ii[9]>0 && step_ii[10]>0)||
          (step_ii[11]>0 && step_ii[12]>0 && step_ii[13]>0 && step_ii[14]>0 && step_ii[15]>0)||
          (step_ii[16]>0 && step_ii[17]>0 && step_ii[18]>0 && step_ii[19]>0 && step_ii[20]>0)||
          (step_ii[21]>0 && step_ii[22]>0 && step_ii[23]>0 && step_ii[24]>0 && step_ii[25]>0))
      {
      ui->l_win->setText("горизонталь"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Игрок 2 победил по горизонтали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем вертикали
       if((step_ii[1]>0 && step_ii[6]>0 && step_ii[11]>0 && step_ii[16]>0 && step_ii[21]>0)||
          (step_ii[2]>0 && step_ii[7]>0 && step_ii[12]>0 && step_ii[17]>0 && step_ii[22]>0)||
          (step_ii[3]>0 && step_ii[8]>0 && step_ii[13]>0 && step_ii[18]>0 && step_ii[23]>0)||
          (step_ii[4]>0 && step_ii[9]>0 && step_ii[14]>0 && step_ii[19]>0 && step_ii[24]>0)||
          (step_ii[5]>0 && step_ii[10]>0 && step_ii[15]>0 && step_ii[20]>0 && step_ii[25]>0))
      {
      ui->l_win->setText("вертикаль"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Игрок 2 победил по вертикали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем диагонали
       if((step_ii[1]>0 && step_ii[7]>0 && step_ii[13]>0 && step_ii[19]>0 && step_ii[25]>0)||
          (step_ii[5]>0 && step_ii[9]>0 && step_ii[13]>0 && step_ii[17]>0 && step_ii[21]>0))
      {
      ui->l_win->setText("диагональ"); ui->demoButton->setChecked(false); index = 0;int n = QMessageBox::warning(0,"ПОБЕДА","Игрок 2 победил по диагонали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
     }
}


//ход компьютера
void XO_game::ii_run()
{
    //таймер на ничью

    if (rnd > 0 && index == 2){
        ui->rb_5->isChecked()? rnd = (rand() % 26):rnd = (rand() % 14);
        ui->spinBox->setValue(rnd);
        if (step_m[rnd]<1 && step_ii[rnd]<1){
            if (rnd == 1){ui->pb_1->setChecked(true); ui->pb_1->setEnabled(false); ui->pb_1->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[1]=1;}
            if (rnd == 2){ui->pb_2->setChecked(true); ui->pb_2->setEnabled(false); ui->pb_2->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[2]=1;}
            if (rnd == 3){ui->pb_3->setChecked(true); ui->pb_3->setEnabled(false); ui->pb_3->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[3]=1;}
            if (rnd == 4){if (ui->pb_4->isEnabled()){ui->pb_4->setChecked(true); ui->pb_4->setEnabled(false); ui->pb_4->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[4]=1;} else rnd = 30;}
            if (rnd == 5){if (ui->pb_5->isEnabled()){ui->pb_5->setChecked(true); ui->pb_5->setEnabled(false); ui->pb_5->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[5]=1;} else rnd = 30;}
            if (rnd == 6){ui->pb_6->setChecked(true); ui->pb_6->setEnabled(false); ui->pb_6->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[6]=1;}
            if (rnd == 7){ui->pb_7->setChecked(true); ui->pb_7->setEnabled(false); ui->pb_7->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[7]=1;}
            if (rnd == 8){ui->pb_8->setChecked(true); ui->pb_8->setEnabled(false); ui->pb_8->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[8]=1;}
            if (rnd == 9){if (ui->pb_9->isEnabled()){ ui->pb_9->setChecked(true); ui->pb_9->setEnabled(false); ui->pb_9->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[9]=1;} else rnd = 30;}
            if (rnd == 10){if (ui->pb_10->isEnabled()){ ui->pb_10->setChecked(true); ui->pb_10->setEnabled(false); ui->pb_10->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[10]=1;} else rnd = 30;}
            if (rnd == 11){ui->pb_11->setChecked(true); ui->pb_11->setEnabled(false); ui->pb_11->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[11]=1;}
            if (rnd == 12){ui->pb_12->setChecked(true); ui->pb_12->setEnabled(false); ui->pb_12->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[12]=1;}
            if (rnd == 13){ui->pb_13->setChecked(true); ui->pb_13->setEnabled(false); ui->pb_13->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[13]=1;}
            if (rnd == 14){ui->pb_14->setChecked(true); ui->pb_14->setEnabled(false); ui->pb_14->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[14]=1;}
            if (rnd == 15){ui->pb_15->setChecked(true); ui->pb_15->setEnabled(false); ui->pb_15->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[15]=1;}
            if (rnd == 16){ui->pb_16->setChecked(true); ui->pb_16->setEnabled(false); ui->pb_16->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[16]=1;}
            if (rnd == 17){ui->pb_17->setChecked(true); ui->pb_17->setEnabled(false); ui->pb_17->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[17]=1;}
            if (rnd == 18){ui->pb_18->setChecked(true); ui->pb_18->setEnabled(false); ui->pb_18->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[18]=1;}
            if (rnd == 19){ui->pb_19->setChecked(true); ui->pb_19->setEnabled(false); ui->pb_19->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[19]=1;}
            if (rnd == 20){ui->pb_20->setChecked(true); ui->pb_20->setEnabled(false); ui->pb_20->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[20]=1;}
            if (rnd == 21){ui->pb_21->setChecked(true); ui->pb_21->setEnabled(false); ui->pb_21->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[21]=1;}
            if (rnd == 22){ui->pb_22->setChecked(true); ui->pb_22->setEnabled(false); ui->pb_22->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[22]=1;}
            if (rnd == 23){ui->pb_23->setChecked(true); ui->pb_23->setEnabled(false); ui->pb_23->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[23]=1;}
            if (rnd == 24){ui->pb_24->setChecked(true); ui->pb_24->setEnabled(false); ui->pb_24->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[24]=1;}
            if (rnd == 25){ui->pb_25->setChecked(true); ui->pb_25->setEnabled(false); ui->pb_25->setText(symbol == "O"?"X":"O"); rnd = 0; index = 0; step_ii[25]=1;}
        }
    }
}

void XO_game::ii2_run()
{
    if (rnd > 0 && index == 1){
        ui->rb_5->isChecked()? rnd = (rand() % 26):rnd = (rand() % 14);
        ui->spinBox->setValue(rnd);
        if (step_m[rnd]<1 && step_ii[rnd]<1){
            if (rnd == 1){ui->pb_1->setChecked(true); ui->pb_1->setEnabled(false); ui->pb_1->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[1]=1;}
            if (rnd == 2){ui->pb_2->setChecked(true); ui->pb_2->setEnabled(false); ui->pb_2->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[2]=1;}
            if (rnd == 3){ui->pb_3->setChecked(true); ui->pb_3->setEnabled(false); ui->pb_3->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[3]=1;}
            if (rnd == 4){if (ui->pb_4->isEnabled()){ui->pb_4->setChecked(true); ui->pb_4->setEnabled(false); ui->pb_4->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[4]=1;} else rnd = 30;}
            if (rnd == 5){if (ui->pb_5->isEnabled()){ui->pb_5->setChecked(true); ui->pb_5->setEnabled(false); ui->pb_5->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[5]=1;} else rnd = 30;}
            if (rnd == 6){ui->pb_6->setChecked(true); ui->pb_6->setEnabled(false); ui->pb_6->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[6]=1;}
            if (rnd == 7){ui->pb_7->setChecked(true); ui->pb_7->setEnabled(false); ui->pb_7->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[7]=1;}
            if (rnd == 8){ui->pb_8->setChecked(true); ui->pb_8->setEnabled(false); ui->pb_8->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[8]=1;}
            if (rnd == 9){if (ui->pb_9->isEnabled()){ ui->pb_9->setChecked(true); ui->pb_9->setEnabled(false); ui->pb_9->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[9]=1;} else rnd = 30;}
            if (rnd == 10){if (ui->pb_10->isEnabled()){ ui->pb_10->setChecked(true); ui->pb_10->setEnabled(false); ui->pb_10->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[10]=1;} else rnd = 30;}
            if (rnd == 11){ui->pb_11->setChecked(true); ui->pb_11->setEnabled(false); ui->pb_11->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[11]=1;}
            if (rnd == 12){ui->pb_12->setChecked(true); ui->pb_12->setEnabled(false); ui->pb_12->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[12]=1;}
            if (rnd == 13){ui->pb_13->setChecked(true); ui->pb_13->setEnabled(false); ui->pb_13->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[13]=1;}
            if (rnd == 14){ui->pb_14->setChecked(true); ui->pb_14->setEnabled(false); ui->pb_14->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[14]=1;}
            if (rnd == 15){ui->pb_15->setChecked(true); ui->pb_15->setEnabled(false); ui->pb_15->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[15]=1;}
            if (rnd == 16){ui->pb_16->setChecked(true); ui->pb_16->setEnabled(false); ui->pb_16->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[16]=1;}
            if (rnd == 17){ui->pb_17->setChecked(true); ui->pb_17->setEnabled(false); ui->pb_17->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[17]=1;}
            if (rnd == 18){ui->pb_18->setChecked(true); ui->pb_18->setEnabled(false); ui->pb_18->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[18]=1;}
            if (rnd == 19){ui->pb_19->setChecked(true); ui->pb_19->setEnabled(false); ui->pb_19->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[19]=1;}
            if (rnd == 20){ui->pb_20->setChecked(true); ui->pb_20->setEnabled(false); ui->pb_20->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[20]=1;}
            if (rnd == 21){ui->pb_21->setChecked(true); ui->pb_21->setEnabled(false); ui->pb_21->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[21]=1;}
            if (rnd == 22){ui->pb_22->setChecked(true); ui->pb_22->setEnabled(false); ui->pb_22->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[22]=1;}
            if (rnd == 23){ui->pb_23->setChecked(true); ui->pb_23->setEnabled(false); ui->pb_23->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[23]=1;}
            if (rnd == 24){ui->pb_24->setChecked(true); ui->pb_24->setEnabled(false); ui->pb_24->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[24]=1;}
            if (rnd == 25){ui->pb_25->setChecked(true); ui->pb_25->setEnabled(false); ui->pb_25->setText(symbol == "O"?"O":"X"); rnd = 0; index = 2; step_m[25]=1;}
        }
    }
    //Если игра 3х3
    if (ui->rb_3->isChecked())
    {
      //проверяем горизонтали
       if((step_ii[1]>0 && step_ii[2]>0 && step_ii[3]>0)||(step_ii[6]>0 && step_ii[7]>0 && step_ii[8]>0)||(step_ii[11]>0 && step_ii[12]>0 && step_ii[13]>0))
      {
      ui->l_win->setText("горизонталь"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Компьютер победил по горизонтали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем вертикали
       if((step_ii[1]>0 && step_ii[6]>0 && step_ii[11]>0)||(step_ii[2]>0 && step_ii[7]>0 && step_ii[12]>0)||(step_ii[3]>0 && step_ii[8]>0 && step_ii[13]>0))
      {
      ui->l_win->setText("вертикаль"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Компьютер победил по вертикали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем диагонали
       if((step_ii[1]>0 && step_ii[7]>0 && step_ii[13]>0)||(step_ii[3]>0 && step_ii[7]>0 && step_ii[11]>0))
      {
      ui->l_win->setText("диагональ"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Компьютер победил по диагонали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
     }
    //Если игра 5х5
    if (ui->rb_5->isChecked())
    {
      //проверяем горизонтали
       if((step_ii[1]>0 && step_ii[2]>0 && step_ii[3]>0 && step_ii[4]>0 && step_ii[5]>0)||
          (step_ii[6]>0 && step_ii[7]>0 && step_ii[8]>0 && step_ii[9]>0 && step_ii[10]>0)||
          (step_ii[11]>0 && step_ii[12]>0 && step_ii[13]>0 && step_ii[14]>0 && step_ii[15]>0)||
          (step_ii[16]>0 && step_ii[17]>0 && step_ii[18]>0 && step_ii[19]>0 && step_ii[20]>0)||
          (step_ii[21]>0 && step_ii[22]>0 && step_ii[23]>0 && step_ii[24]>0 && step_ii[25]>0))
      {
      ui->l_win->setText("горизонталь"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Компьютер победил по горизонтали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем вертикали
       if((step_ii[1]>0 && step_ii[6]>0 && step_ii[11]>0 && step_ii[16]>0 && step_ii[21]>0)||
          (step_ii[2]>0 && step_ii[7]>0 && step_ii[12]>0 && step_ii[17]>0 && step_ii[22]>0)||
          (step_ii[3]>0 && step_ii[8]>0 && step_ii[13]>0 && step_ii[18]>0 && step_ii[23]>0)||
          (step_ii[4]>0 && step_ii[9]>0 && step_ii[14]>0 && step_ii[19]>0 && step_ii[24]>0)||
          (step_ii[5]>0 && step_ii[10]>0 && step_ii[15]>0 && step_ii[20]>0 && step_ii[25]>0))
      {
      ui->l_win->setText("вертикаль"); ui->demoButton->setChecked(false); index = 0; int n = QMessageBox::warning(0,"ПОБЕДА","Компьютер победил по вертикали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
       //проверяем диагонали
       if((step_ii[1]>0 && step_ii[7]>0 && step_ii[13]>0 && step_ii[19]>0 && step_ii[25]>0)||
          (step_ii[5]>0 && step_ii[9]>0 && step_ii[13]>0 && step_ii[17]>0 && step_ii[21]>0))
      {
      ui->l_win->setText("диагональ"); ui->demoButton->setChecked(false); index = 0;int n = QMessageBox::warning(0,"ПОБЕДА","Компьютер победил по диагонали","Продолжить", "Заново", QString (), 0, 1); if (n) {reset();}
       }
     }
}


void XO_game::on_resetButton_clicked()
{
    reset();
}

void XO_game::mir()
{
    if (!ui->pb_1->isEnabled() && !ui->pb_2->isEnabled() && !ui->pb_3->isEnabled() && !ui->pb_4->isEnabled() && !ui->pb_5->isEnabled() &&
        !ui->pb_6->isEnabled() && !ui->pb_7->isEnabled() && !ui->pb_8->isEnabled() && !ui->pb_9->isEnabled() && !ui->pb_10->isEnabled() &&
        !ui->pb_11->isEnabled() && !ui->pb_12->isEnabled() && !ui->pb_13->isEnabled() && !ui->pb_14->isEnabled() && !ui->pb_15->isEnabled() &&
        !ui->pb_16->isEnabled() && !ui->pb_17->isEnabled() && !ui->pb_18->isEnabled() && !ui->pb_19->isEnabled() && !ui->pb_20->isEnabled() &&
        !ui->pb_21->isEnabled() && !ui->pb_22->isEnabled() && !ui->pb_23->isEnabled() && !ui->pb_24->isEnabled() && !ui->pb_25->isEnabled() )
    {int n = QMessageBox::warning(0,"Ничья","Не вижу ходов.","Продолжить", "Заново", QString (), 0, 1); if (n) {ui->demoButton->setChecked(false); index = 0; reset();}}
}
void XO_game::reset()
{
    seting_enable();
    ui->l_win->setText("играем");
    for(int i=0; i<26; i++)    {        step_m[i]=0; step_ii[i]=0;    }
    ui->pb_1->setChecked(false); ui->pb_1->setText("");    ui->pb_2->setChecked(false); ui->pb_2->setText("");    ui->pb_3->setChecked(false); ui->pb_3->setText("");
    ui->pb_4->setChecked(false); ui->pb_4->setText("");    ui->pb_5->setChecked(false); ui->pb_5->setText("");    ui->pb_6->setChecked(false); ui->pb_6->setText("");
    ui->pb_7->setChecked(false); ui->pb_7->setText("");    ui->pb_8->setChecked(false); ui->pb_8->setText("");    ui->pb_9->setChecked(false); ui->pb_9->setText("");
    ui->pb_10->setChecked(false); ui->pb_10->setText("");    ui->pb_11->setChecked(false); ui->pb_11->setText("");    ui->pb_12->setChecked(false); ui->pb_12->setText("");
    ui->pb_13->setChecked(false); ui->pb_13->setText("");    ui->pb_14->setChecked(false); ui->pb_14->setText("");    ui->pb_15->setChecked(false); ui->pb_15->setText("");
    ui->pb_16->setChecked(false); ui->pb_16->setText("");    ui->pb_17->setChecked(false); ui->pb_17->setText("");    ui->pb_18->setChecked(false); ui->pb_18->setText("");
    ui->pb_19->setChecked(false); ui->pb_19->setText("");    ui->pb_20->setChecked(false); ui->pb_20->setText("");    ui->pb_21->setChecked(false); ui->pb_21->setText("");
    ui->pb_22->setChecked(false); ui->pb_22->setText("");    ui->pb_23->setChecked(false); ui->pb_23->setText("");    ui->pb_24->setChecked(false); ui->pb_24->setText("");
    ui->pb_25->setChecked(false); ui->pb_25->setText("");
    ui->pb_1->setEnabled(true); ui->pb_2->setEnabled(true); ui->pb_3->setEnabled(true); ui->pb_4->setEnabled(true); ui->pb_5->setEnabled(true);
    ui->pb_6->setEnabled(true); ui->pb_7->setEnabled(true); ui->pb_8->setEnabled(true); ui->pb_9->setEnabled(true); ui->pb_10->setEnabled(true);
    ui->pb_11->setEnabled(true); ui->pb_12->setEnabled(true); ui->pb_13->setEnabled(true); ui->pb_14->setEnabled(true); ui->pb_15->setEnabled(true);
    ui->pb_16->setEnabled(true); ui->pb_17->setEnabled(true); ui->pb_18->setEnabled(true); ui->pb_19->setEnabled(true); ui->pb_20->setEnabled(true);
    ui->pb_21->setEnabled(true); ui->pb_22->setEnabled(true); ui->pb_23->setEnabled(true); ui->pb_24->setEnabled(true); ui->pb_25->setEnabled(true);
    if (ui->rb_3->isChecked()){
        ui->pb_4->setEnabled(false); ui->pb_5->setEnabled(false); ui->pb_9->setEnabled(false); ui->pb_10->setEnabled(false);
        ui->pb_14->setEnabled(false); ui->pb_15->setEnabled(false); ui->pb_16->setEnabled(false); ui->pb_17->setEnabled(false);
        ui->pb_18->setEnabled(false); ui->pb_19->setEnabled(false); ui->pb_20->setEnabled(false); ui->pb_21->setEnabled(false);
        ui->pb_22->setEnabled(false); ui->pb_23->setEnabled(false); ui->pb_24->setEnabled(false); ui->pb_25->setEnabled(false);}
}

//Жмём кнопки!
void XO_game::on_pb_1_clicked(bool checked){     symbol=="X"?ui->pb_1->setText("X"):ui->pb_1->setText("O"); ui->pb_1->setEnabled(false); seting_disable(); index == 2?step_ii[1]=1:step_m[1]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_2_clicked(bool checked){     symbol=="X"?ui->pb_2->setText("X"):ui->pb_2->setText("O"); ui->pb_2->setEnabled(false); seting_disable(); index == 2?step_ii[2]=1:step_m[2]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_3_clicked(bool checked){     symbol=="X"?ui->pb_3->setText("X"):ui->pb_3->setText("O"); ui->pb_3->setEnabled(false); seting_disable(); index == 2?step_ii[3]=1:step_m[3]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_4_clicked(bool checked){     symbol=="X"?ui->pb_4->setText("X"):ui->pb_4->setText("O"); ui->pb_4->setEnabled(false); seting_disable(); index == 2?step_ii[4]=1:step_m[4]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_5_clicked(bool checked){     symbol=="X"?ui->pb_5->setText("X"):ui->pb_5->setText("O"); ui->pb_5->setEnabled(false); seting_disable(); index == 2?step_ii[5]=1:step_m[5]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_6_clicked(bool checked){     symbol=="X"?ui->pb_6->setText("X"):ui->pb_6->setText("O"); ui->pb_6->setEnabled(false); seting_disable(); index == 2?step_ii[6]=1:step_m[6]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_7_clicked(bool checked){     symbol=="X"?ui->pb_7->setText("X"):ui->pb_7->setText("O"); ui->pb_7->setEnabled(false); seting_disable(); index == 2?step_ii[7]=1:step_m[7]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_8_clicked(bool checked){     symbol=="X"?ui->pb_8->setText("X"):ui->pb_8->setText("O"); ui->pb_8->setEnabled(false); seting_disable(); index == 2?step_ii[8]=1:step_m[8]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_9_clicked(bool checked){     symbol=="X"?ui->pb_9->setText("X"):ui->pb_9->setText("O"); ui->pb_9->setEnabled(false); seting_disable(); index == 2?step_ii[9]=1:step_m[9]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_10_clicked(bool checked){    symbol=="X"?ui->pb_10->setText("X"):ui->pb_10->setText("O"); ui->pb_10->setEnabled(false); seting_disable(); index == 2?step_ii[10]=1:step_m[10]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_11_clicked(bool checked){    symbol=="X"?ui->pb_11->setText("X"):ui->pb_11->setText("O"); ui->pb_11->setEnabled(false); seting_disable(); index == 2?step_ii[11]=1:step_m[11]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_12_clicked(bool checked){    symbol=="X"?ui->pb_12->setText("X"):ui->pb_12->setText("O"); ui->pb_12->setEnabled(false); seting_disable(); index == 2?step_ii[12]=1:step_m[12]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_13_clicked(bool checked){    symbol=="X"?ui->pb_13->setText("X"):ui->pb_13->setText("O"); ui->pb_13->setEnabled(false); seting_disable(); index == 2?step_ii[13]=1:step_m[13]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_14_clicked(bool checked){    symbol=="X"?ui->pb_14->setText("X"):ui->pb_14->setText("O"); ui->pb_14->setEnabled(false); seting_disable(); index == 2?step_ii[14]=1:step_m[14]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_15_clicked(bool checked){    symbol=="X"?ui->pb_15->setText("X"):ui->pb_15->setText("O"); ui->pb_15->setEnabled(false); seting_disable(); index == 2?step_ii[15]=1:step_m[15]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_16_clicked(bool checked){    symbol=="X"?ui->pb_16->setText("X"):ui->pb_16->setText("O"); ui->pb_16->setEnabled(false); seting_disable(); index == 2?step_ii[16]=1:step_m[16]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_17_clicked(bool checked){    symbol=="X"?ui->pb_17->setText("X"):ui->pb_17->setText("O"); ui->pb_17->setEnabled(false); seting_disable(); index == 2?step_ii[17]=1:step_m[17]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_18_clicked(bool checked){    symbol=="X"?ui->pb_18->setText("X"):ui->pb_18->setText("O"); ui->pb_18->setEnabled(false); seting_disable(); index == 2?step_ii[18]=1:step_m[18]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_19_clicked(bool checked){    symbol=="X"?ui->pb_19->setText("X"):ui->pb_19->setText("O"); ui->pb_19->setEnabled(false); seting_disable(); index == 2?step_ii[19]=1:step_m[19]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_20_clicked(bool checked){    symbol=="X"?ui->pb_20->setText("X"):ui->pb_20->setText("O"); ui->pb_20->setEnabled(false); seting_disable(); index == 2?step_ii[20]=1:step_m[20]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_21_clicked(bool checked){    symbol=="X"?ui->pb_21->setText("X"):ui->pb_21->setText("O"); ui->pb_21->setEnabled(false); seting_disable(); index == 2?step_ii[21]=1:step_m[21]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_22_clicked(bool checked){    symbol=="X"?ui->pb_22->setText("X"):ui->pb_22->setText("O"); ui->pb_22->setEnabled(false); seting_disable(); index == 2?step_ii[22]=1:step_m[22]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_23_clicked(bool checked){    symbol=="X"?ui->pb_23->setText("X"):ui->pb_23->setText("O"); ui->pb_23->setEnabled(false); seting_disable(); index == 2?step_ii[23]=1:step_m[23]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_24_clicked(bool checked){    symbol=="X"?ui->pb_24->setText("X"):ui->pb_24->setText("O"); ui->pb_24->setEnabled(false); seting_disable(); index == 2?step_ii[24]=1:step_m[24]=1; index == 2?index = 0:index = 1;}
void XO_game::on_pb_25_clicked(bool checked){    symbol=="X"?ui->pb_25->setText("X"):ui->pb_25->setText("O"); ui->pb_25->setEnabled(false); seting_disable(); index == 2?step_ii[25]=1:step_m[25]=1; index == 2?index = 0:index = 1;}

