#ifndef MENU_H
#define MENU_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Menu; }
QT_END_NAMESPACE

class Menu : public QMainWindow //клас, який відповідає за початкове меню користувача
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();
    friend class MainWindow;

private:
    Ui::Menu *ui;

public slots:

signals:
    void choicecategory1(QString); //сигнал, який вибирає першу категорію
    void choicecategory2(QString); //сигнал, який вибирає другу категорію
    void choicecategory3(QString); //сигнал, який вибирає третю категорію
    void choicecategory4(QString); //сигнал, який вибирає четверту категорію
    void choicecategory5(QString); //сигнал, який вибирає п'яту категорію

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

};
#endif // MENU_H
