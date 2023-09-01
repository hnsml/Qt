#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);

    //встановлення фонового зображення
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/fon.png")));
    this->setPalette(palette);
}

Menu::~Menu()
{
    delete ui;
}


void Menu::on_pushButton_clicked()
{
    emit choicecategory1("Тварини"); //вибір категорії "Тварини"
    hide();
}


void Menu::on_pushButton_2_clicked()
{
    emit choicecategory2("Квіти"); //вибір категорії "Квіти"
    hide();
}


void Menu::on_pushButton_3_clicked()
{
    emit choicecategory3("Птахи"); //вибір категорії "Птахи"
    hide();
}


void Menu::on_pushButton_4_clicked()
{
    emit choicecategory4("Кольори"); //вибір категорії "Кольори"
    hide();
}


void Menu::on_pushButton_5_clicked()
{
    emit choicecategory5("Фрукти та овочі"); //вибір категорії "Фрукти та овочі"
    hide();
}

