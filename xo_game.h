#ifndef XO_GAME_H
#define XO_GAME_H

#include <QMainWindow>
#include <QTimer>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class XO_game; }
QT_END_NAMESPACE

class XO_game : public QMainWindow
{
    Q_OBJECT

public:
    XO_game(QWidget *parent = nullptr);
    ~XO_game();

    int index;
    int rnd;
    int noTimer;
    int fullTimer;
    long step_m[26];
    long step_ii[26];
    std::string symbol, symbol2;

private slots:

    void onTimer();//Таймер работающий всё время
    void mainlogic();//Основная логика тут
    void winner();//Смотрим, кто выиграл
    void winner2();
    void mir();
    void ii_run();//Ход компьютера
    void ii2_run();//Ход компьютера2
    void reset();//Сброс игры

    void on_rb_5_clicked(bool checked);

    void on_rb_3_clicked(bool checked);

    void on_rb_o_clicked(bool checked);

    void on_rb_x_clicked(bool checked);

    void on_rb_m_clicked(bool checked);

    void on_rb_ii_clicked(bool checked);

    void on_resetButton_clicked();

    void seting_disable();
    void seting_enable();

    void on_pb_1_clicked(bool checked);
    void on_pb_2_clicked(bool checked);
    void on_pb_3_clicked(bool checked);
    void on_pb_4_clicked(bool checked);
    void on_pb_5_clicked(bool checked);
    void on_pb_6_clicked(bool checked);
    void on_pb_7_clicked(bool checked);
    void on_pb_8_clicked(bool checked);
    void on_pb_9_clicked(bool checked);
    void on_pb_10_clicked(bool checked);
    void on_pb_11_clicked(bool checked);
    void on_pb_12_clicked(bool checked);
    void on_pb_13_clicked(bool checked);
    void on_pb_14_clicked(bool checked);
    void on_pb_15_clicked(bool checked);
    void on_pb_16_clicked(bool checked);
    void on_pb_17_clicked(bool checked);
    void on_pb_18_clicked(bool checked);
    void on_pb_19_clicked(bool checked);
    void on_pb_20_clicked(bool checked);
    void on_pb_21_clicked(bool checked);
    void on_pb_22_clicked(bool checked);
    void on_pb_23_clicked(bool checked);
    void on_pb_24_clicked(bool checked);
    void on_pb_25_clicked(bool checked);


private:
    Ui::XO_game *ui;
    QTimer *Timer;
};
#endif // XO_GAME_H
