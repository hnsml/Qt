#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer> //піключення бібліотек для відтворення музики
#include <QAudioOutput>
#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend class Menu;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void updateTimer();

    void on_pushButton_A_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_B_clicked();

    void on_pushButton_V_clicked();

    void on_pushButton_G_clicked();

    void on_pushButton_GE_clicked();

    void on_pushButton_D_clicked();

    void on_pushButton_E_clicked();

    void on_pushButton_YE_clicked();

    void on_pushButton_ZH_clicked();

    void on_pushButton_Z_clicked();

    void on_pushButton_U_clicked();

    void on_pushButton_I_clicked();

    void on_pushButton_YI_clicked();

    void on_pushButton_Y_clicked();

    void on_pushButton_K_clicked();

    void on_pushButton_L_clicked();

    void on_pushButton_M_clicked();

    void on_pushButton_N_clicked();

    void on_pushButton_O_clicked();

    void on_pushButton_P_clicked();

    void on_pushButton_R_clicked();

    void on_pushButton_C_clicked();

    void on_pushButton_T_clicked();

    void on_pushButton_U_2_clicked();

    void on_pushButton_F_clicked();

    void on_pushButton_X_clicked();

    void on_pushButton_TS_clicked();

    void on_pushButton_CH_clicked();

    void on_pushButton_SH_clicked();

    void on_pushButton_SHCH_clicked();

    void on_pushButton_QQ_clicked();

    void on_pushButton_YU_clicked();

    void on_pushButton_YA_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    Menu *menu;
    int diamonds=100;                      //змінна, яка позначає кількість кристаликів
    int word_count=0;                      //змінна, яка позначає кількість вгаданих слів
    QString word;                          //загадане слово
    QString dispWord;                      //маска, яка виводить, які букви користувач вгадав
    bool check (QString letter);           //функція, яка перевіряє чи є буква в слові
    int found=0;                           //змінна, яка позначає кількість вгаданих букв
    int tries = 6;                         //змінна, яка позначає кількість спроб
    bool isPlaying = true;                 //змінна, яка показує, чи грає музика
    bool game=true;                        //змінна, яка погакує чи гра завершена
    QTimer* timer;                         //таймер
    int timeCount;                         //кількість секунд
    void switch_buttons(bool variant);     //функція, яка включає (виключає) всі кнопки
    void switch_background();              //функція, яка забирає фон на всіх кнопках
    void print(int tr);                    //функція, яка виводить дяпіка
    QMediaPlayer *soundPlayer;             //плеєр
    QAudioOutput *playerOutput;

    QList <QString> wordList={"лев", "вовк", "лисиця", "тигр", "слон", "носоріг", "леопард", "їжак", "борсук", "олень", "лось", "зубр", "білка", "заєць", "панда", "жираф", "кабан", "лінивець", "кіт", "собака", "тюльпан", "роза", "гіпсофіла", "гортензія", "ромашка", "кульбаба", "волошка", "півень", "нагідка", "жоржина", "троянда", "лілія", "півонія", "чорнобривці", "мак", "гвоздика", "нарцис", "айстра", "барвінок", "конвалія", "ворона", "снігур", "орел", "голуб", "горобець", "дятел", "грифон", "сова", "синиця", "лебідь", "перепілка", "гуска", "дрізд", "жайворонок", "зозуля", "куріпка", "крук", "чайка", "папуга", "пелікан", "синій", "червоний", "чорний", "фіолетовий", "помаранчевий", "коричневий", "блакитний", "білий", "рожевий", "жовтий", "зелений", "салатовий", "сірий", "бузковий", "бежевий", "багровий", "срібний", "золотий", "вишневий", "оливковий", "баклажан", "картопля", "кукурудза", "морква", "полуниця", "малина", "тиква", "банан", "диня", "авокадо", "огірок", "помідор", "капуста", "лимон", "апельсин", "лайм", "мандарин", "груша", "виноград", "яблуко"};
    //список слів
public slots:
    void category1 (QString category);     //функція, яка вибирає слово з першої категорії (Тварини)
    void category2 (QString category);     //функція, яка вибирає слово з першої категорії (Квіти)
    void category3 (QString category);     //функція, яка вибирає слово з першої категорії (Птахи)
    void category4 (QString category);     //функція, яка вибирає слово з першої категорії (Кольори)
    void category5 (QString category);     //функція, яка вибирає слово з першої категорії (Фрукти та овочі)
};
#endif // MAINWINDOW_H

