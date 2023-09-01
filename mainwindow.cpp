#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_menu.h"
#include "menu.h"
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QMediaPlayer>
#include <QAudioOutput>

/*Особистий вклад кожного учасника: */
/*Тетяна                                       Артем*/
/*встановлення іконок                          пошук всіх іконок*/
/*створення таймера                            створення та налаштування зовнішнього вигляду mainwindow.ui*/
/*підключення меню                             створення та налаштування зовнішнього вигляду menu.ui*/
/*створення підказки                           пошук всіх звукових файлів*/
/*створення функцій для вибору категорії       підключення звукових файлів*/
/*створення функції для виводу чоловічка       малювання кожної стадії чоловічка*/
/*створення функції перевірки букви            створення функцій switch_buttons і switch_background*/
/*створення функції для звуку (вкл/викл)       запис всіх слів в wordList*/
/*створення функції on_pushButton_A_clicked    створення функцій для всіх інших букв по шаблону on_pushButton_A_clicked*/
/*створення міні документації                  моральна підтримка*/
/*Разом розробили концепт і придумали доповнення для гри*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    soundPlayer = new QMediaPlayer(this); //встановлення плеєра
    playerOutput = new QAudioOutput(soundPlayer);
    QUrl audioUrl("qrc:/images/melody.wav"); // встановлення мелодії, яка буде на фоні грати
    soundPlayer->setSource(audioUrl);
    playerOutput->setVolume(0.2);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->setLoops(-1); //встановлення зациклення музики
    soundPlayer->play();

    switch_buttons(true); //включення всіх кнопок

    //встановлення іконки для кристаликів
    QPixmap diamondixmap(":/images/diamond.png");
    QPixmap resizeddiamondPixmap = diamondixmap.scaled(50, 50, Qt::KeepAspectRatio);
    ui->pushButton->setIcon(QIcon(resizeddiamondPixmap));
    ui->pushButton->setIconSize(resizeddiamondPixmap.size());

    //встановлення іконки для підказки
    QPixmap hintixmap(":/images/hint.png");
    QPixmap resizedhintPixmap = hintixmap.scaled(50, 50, Qt::KeepAspectRatio);
    ui->pushButton_3->setIcon(QIcon(resizedhintPixmap));
    ui->pushButton_3->setIconSize(resizedhintPixmap.size());

    //встановлення іконки для жетонів
    QPixmap counterpixmap(":/images/counter.png");
    QPixmap resizedcounterPixmap = counterpixmap.scaled(50, 50, Qt::KeepAspectRatio);
    ui->pushButton_2->setIcon(QIcon(resizedcounterPixmap));
    ui->pushButton_2->setIconSize(resizedcounterPixmap.size());

    //встановлення іконки для включеного динаміка
    QPixmap onpixmap(":/images/on.png");
    QPixmap resizedonPixmap = onpixmap.scaled(80, 80, Qt::KeepAspectRatio);
    ui->pushButton_5->setIcon(QIcon(resizedonPixmap));
    ui->pushButton_5->setIconSize(resizedonPixmap.size());

    //встановлення таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timeCount = 0;

    //встановлення зображення на фон програми
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/fon.png")));
    this->setPalette(palette);

    ui->pushButton_4->hide(); //ховаємо кнопку "Грати знову"

    //створення початкового меню і підключення функцій
    menu = new Menu(this);
    menu->show();
    hide();
    connect(menu, &Menu::choicecategory1, this, &MainWindow::category1);
    connect(menu, &Menu::choicecategory2, this, &MainWindow::category2);
    connect(menu, &Menu::choicecategory3, this, &MainWindow::category3);
    connect(menu, &Menu::choicecategory4, this, &MainWindow::category4);
    connect(menu, &Menu::choicecategory5, this, &MainWindow::category5);

    ui->label_2->setText("");
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QMessageBox::about(this, "Про кристали", "Це ваша кількість кристалів. Вони необхідні для придбання підказок. Після вгаданого слова вам нараховується 30 кристаликів."); //виводить інформацію про кристали
}


void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::about(this, "Про жетони", "Це ваша кількість жетонів. Жетон дається після вгадування слова."); //виводить інформацію про жетони
}


void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Купити підказку", "Ви хочете купити підказку за 20 кристалів?", QMessageBox::Yes | QMessageBox::No); //викликає QMessageBox, який запитує чи користувач хоче купити підказку
    if (reply == QMessageBox::Yes)
    {
        if(diamonds<20)
        {
            QMessageBox::about(this, "Недостатньо кристаликів", "На вашому рахунку недостатньо кристалів!"); //виводить інформацію про недостатню кількість кристалів
        }
        else
        {
            diamonds-=20; //віднімається вартість підказки
            ui->pushButton->setText(QString::number(diamonds));
            int position=0;

            //вибираємо рандомну позицію, яка ще не відкрита
            while(dispWord[position] != '_')
            {
                position=rand()%word.length();
            }
            check(word[position]); //передаємо цю літеру як параметр для функції check, щоб вона відкрила її в слові
        }
    }
}

void MainWindow::updateTimer()
{
    //обновлення таймеру і виведення у потрібному форматі
    timeCount++;
    int minutes = (timeCount % 3600) / 60;
    int seconds = timeCount % 60;
    QString timeString = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    ui->timerLabel->setText(timeString);
}

void MainWindow::switch_buttons(bool variant)
{
    //функція, робить всі кнопки, що є на MainWindow доступними (параметр true), або недоступними (false)
    ui->pushButton->setEnabled(variant);
    ui->pushButton_2->setEnabled(variant);
    ui->pushButton_3->setEnabled(variant);
    ui->pushButton_A->setEnabled(variant);
    ui->pushButton_B->setEnabled(variant);
    ui->pushButton_V->setEnabled(variant);
    ui->pushButton_G->setEnabled(variant);
    ui->pushButton_GE->setEnabled(variant);
    ui->pushButton_D->setEnabled(variant);
    ui->pushButton_E->setEnabled(variant);
    ui->pushButton_YE->setEnabled(variant);
    ui->pushButton_ZH->setEnabled(variant);
    ui->pushButton_Z->setEnabled(variant);
    ui->pushButton_U->setEnabled(variant);
    ui->pushButton_I->setEnabled(variant);
    ui->pushButton_YI->setEnabled(variant);
    ui->pushButton_Y->setEnabled(variant);
    ui->pushButton_K->setEnabled(variant);
    ui->pushButton_L->setEnabled(variant);
    ui->pushButton_M->setEnabled(variant);
    ui->pushButton_N->setEnabled(variant);
    ui->pushButton_O->setEnabled(variant);
    ui->pushButton_P->setEnabled(variant);
    ui->pushButton_R->setEnabled(variant);
    ui->pushButton_C->setEnabled(variant);
    ui->pushButton_T->setEnabled(variant);
    ui->pushButton_U_2->setEnabled(variant);
    ui->pushButton_F->setEnabled(variant);
    ui->pushButton_X->setEnabled(variant);
    ui->pushButton_TS->setEnabled(variant);
    ui->pushButton_CH->setEnabled(variant);
    ui->pushButton_SH->setEnabled(variant);
    ui->pushButton_SHCH->setEnabled(variant);
    ui->pushButton_QQ->setEnabled(variant);
    ui->pushButton_YU->setEnabled(variant);
    ui->pushButton_YA->setEnabled(variant);
}
void MainWindow::switch_background()
{
    //функція забирає всі фонові зображення, які могли залишитися після попередньої гри
    QString styleSheet = "QPushButton {" "background-image: none;" "}";
    ui->pushButton_A->setStyleSheet(styleSheet);
    ui->pushButton_B->setStyleSheet(styleSheet);
    ui->pushButton_V->setStyleSheet(styleSheet);
    ui->pushButton_G->setStyleSheet(styleSheet);
    ui->pushButton_GE->setStyleSheet(styleSheet);
    ui->pushButton_D->setStyleSheet(styleSheet);
    ui->pushButton_E->setStyleSheet(styleSheet);
    ui->pushButton_YE->setStyleSheet(styleSheet);
    ui->pushButton_ZH->setStyleSheet(styleSheet);
    ui->pushButton_Z->setStyleSheet(styleSheet);
    ui->pushButton_U->setStyleSheet(styleSheet);
    ui->pushButton_I->setStyleSheet(styleSheet);
    ui->pushButton_YI->setStyleSheet(styleSheet);
    ui->pushButton_Y->setStyleSheet(styleSheet);
    ui->pushButton_K->setStyleSheet(styleSheet);
    ui->pushButton_L->setStyleSheet(styleSheet);
    ui->pushButton_M->setStyleSheet(styleSheet);
    ui->pushButton_N->setStyleSheet(styleSheet);
    ui->pushButton_O->setStyleSheet(styleSheet);
    ui->pushButton_P->setStyleSheet(styleSheet);
    ui->pushButton_R->setStyleSheet(styleSheet);
    ui->pushButton_C->setStyleSheet(styleSheet);
    ui->pushButton_T->setStyleSheet(styleSheet);
    ui->pushButton_U_2->setStyleSheet(styleSheet);
    ui->pushButton_F->setStyleSheet(styleSheet);
    ui->pushButton_X->setStyleSheet(styleSheet);
    ui->pushButton_TS->setStyleSheet(styleSheet);
    ui->pushButton_CH->setStyleSheet(styleSheet);
    ui->pushButton_SH->setStyleSheet(styleSheet);
    ui->pushButton_SHCH->setStyleSheet(styleSheet);
    ui->pushButton_QQ->setStyleSheet(styleSheet);
    ui->pushButton_YU->setStyleSheet(styleSheet);
    ui->pushButton_YA->setStyleSheet(styleSheet);
}
void MainWindow::on_pushButton_4_clicked()
{
    //функція викликає заново вікно з вибором категорії і підключає функції
    menu->show();
    connect(menu, &Menu::choicecategory1, this, &MainWindow::category1);
    connect(menu, &Menu::choicecategory2, this, &MainWindow::category2);
    connect(menu, &Menu::choicecategory3, this, &MainWindow::category3);
    connect(menu, &Menu::choicecategory4, this, &MainWindow::category4);
    connect(menu, &Menu::choicecategory5, this, &MainWindow::category5);
}

void MainWindow::category1(QString category)
{
    found=0;
    tries = 6;
    print(tries);
    switch_buttons(true);
    game=true;
    timeCount=0;
    ui->timerLabel->setText("00:00");
    ui->label->setText("Тварини");
    word = wordList[rand() % 20]; //вибирає слово з перших 20 елементів списка, що відповідає категорії "Тварини"
    timer->start(1000); //запускаємо таймер, який обновлюється кожну секунду
    dispWord= QString(word.length(), QChar('_')); //створюємо маску
    ui->label_2->setText("");
    ui->pushButton_4->hide(); //ховаємо кнопку "Грати знову"
    switch_background(); //забираємо фони кнопок

    //виводимо маску через пробіли
    for(int i=0;i<word.length();i++)
    {
        ui->label_2->setText(ui->label_2->text()+dispWord[i]);
        ui->label_2->setText(ui->label_2->text()+" ");
    }
}
void MainWindow::category2(QString category)
{
    found=0;
    tries = 6;
    print(tries);
    switch_buttons(true);
    game=true;
    timeCount=0;
    ui->timerLabel->setText("00:00");
    ui->label->setText("Квіти");
    word = wordList[rand() % 20 + 20]; //вибирає слово з 20 по 40 елемент списка, що відповідає категорії "Квіти"
    timer->start(1000); //запускаємо таймер, який обновлюється кожну секунду
    dispWord= QString(word.length(), QChar('_')); //створюємо маску
    ui->label_2->setText("");
    ui->pushButton_4->hide(); //ховаємо кнопку "Грати знову"
    switch_background(); //забираємо фони кнопок

    //виводимо маску через пробіли
    for(int i=0;i<word.length();i++)
    {
        ui->label_2->setText(ui->label_2->text()+dispWord[i]);
        ui->label_2->setText(ui->label_2->text()+" ");
    }
}
void MainWindow::category3(QString category)
{
    found=0;
    tries = 6;
    print(tries);
    switch_buttons(true);
    game=true;
    timeCount=0;
    ui->timerLabel->setText("00:00");
    ui->label->setText("Птахи");
    word = wordList[rand() % 20 + 40]; //вибирає слово з 40 по 60 елемент списка, що відповідає категорії "Птахи"
    timer->start(1000); //запускаємо таймер, який обновлюється кожну секунду
    dispWord= QString(word.length(), QChar('_')); //створюємо маску
    ui->label_2->setText("");
    ui->pushButton_4->hide(); //ховаємо кнопку "Грати знову"
    switch_background(); //забираємо фони кнопок

    //виводимо маску через пробіли
    for(int i=0;i<word.length();i++)
    {
        ui->label_2->setText(ui->label_2->text()+dispWord[i]);
        ui->label_2->setText(ui->label_2->text()+" ");
    }
}
void MainWindow::category4(QString category)
{
    found=0;
    tries = 6;
    print(tries);
    game=true;
    timeCount=0;
    ui->timerLabel->setText("00:00");
    switch_buttons(true);
    ui->label->setText("Кольори");
    word = wordList[rand() % 20 + 60]; //вибирає слово з 60 по 80 елемент списка, що відповідає категорії "Кольори"
    timer->start(1000); //запускаємо таймер, який обновлюється кожну секунду
    dispWord= QString(word.length(), QChar('_')); //створюємо маску
    ui->label_2->setText("");
    ui->pushButton_4->hide(); //ховаємо кнопку "Грати знову"
    switch_background(); //забираємо фони кнопок

    //виводимо маску через пробіли
    for(int i=0;i<word.length();i++)
    {
        ui->label_2->setText(ui->label_2->text()+dispWord[i]);
        ui->label_2->setText(ui->label_2->text()+" ");
    }

}
void MainWindow::category5(QString category)
{
    switch_buttons(true);
    found=0;
    tries =6;
    game=true;
    timeCount=0;
    ui->timerLabel->setText("00:00");
    switch_buttons(true);
    print(tries);
    ui->label->setText("Фрукти-овочі");
    word = wordList[rand() % 20 + 80]; //вибирає слово з 80 по 100 елемент списка, що відповідає категорії "Фрукти-овочі"
    timer->start(1000); //запускаємо таймер, який обновлюється кожну секунду
    dispWord= QString(word.length(), QChar('_')); //створюємо маску
    ui->label_2->setText("");
    ui->pushButton_4->hide(); //ховаємо кнопку "Грати знову"
    switch_background(); //забираємо фони кнопок

    //виводимо маску через пробіли
    for(int i=0;i<word.length();i++)
    {
        ui->label_2->setText(ui->label_2->text()+dispWord[i]);
        ui->label_2->setText(ui->label_2->text()+" ");
    }
}


void MainWindow::print(int tr)
{
    //функція виводить картинку з дяпіком, залежно від кількості спроб, які залишилися
    switch (tr) {
    case 6:
    {
       ui->label_3->setPixmap(QPixmap(":/images/01.png"));
       break;
    }
    case 5:
    {
       ui->label_3->setPixmap(QPixmap(":/images/02.png"));
       break;
    }
    case 4:
    {
       ui->label_3->setPixmap(QPixmap(":/images/03.png"));
       break;
    }
    case 3:
    {
       ui->label_3->setPixmap(QPixmap(":/images/04.png"));
       break;
    }
    case 2:
    {
       ui->label_3->setPixmap(QPixmap(":/images/05.png"));
       break;
    }
    case 1:
    {
       ui->label_3->setPixmap(QPixmap(":/images/06.png"));
       break;
    }
    case 0:
    {
       ui->label_3->setPixmap(QPixmap(":/images/07.png"));
       break;
    }
    default:
        break;
    }
}
   QUrl audioUrlwin("qrc:/images/win.mp3"); //створюємо QUrl з перемогою
   QUrl audioUrlend("qrc:/images/end.wav"); //створюємо QUrl з поразкою

bool MainWindow::check(QString letter)
{
    bool mark=false; //змінна, яка показує чи є буква, яка прийшла в параметрі, в слові
    QChar charLetter = letter.at(0);  // Отримуємо перший символ з рядка letter
    for (int i = 0; i < word.length(); i++)
    {
        if (word[i] == charLetter && dispWord[i] == '_') //перевіряємо чи є і чи закрита
        {
            dispWord[i] = charLetter; //заміняємо в масці _ на букву, яку вже знайшли
            found++; //кількість знайдених букв збільшуємо на 1
            mark=true;
        }
    }
    //якщо букву не знайшли, зменшуємо кількість спроб і виводимо дяпіка
    if(mark==false)
    {
    tries--;
    print(tries);
    }

    //виводимо маску через пробіли
    ui->label_2->setText("");
    for(int i=0;i<word.length();i++)
    {
        ui->label_2->setText(ui->label_2->text()+dispWord[i]);
        ui->label_2->setText(ui->label_2->text()+" ");
    }

    //якщо кількість знайдених букв дорівнює довжині слова, то оголошуємо пермогу
    if(found==word.length())
    {
        switch_buttons(false); //виключаємо всі кнопки
        ui->pushButton_4->show(); //показуємо кнопку "Грати знову"
        word_count++; //добавляємо кількість жетонів
        ui->pushButton_2->setText(QString::number(word_count));
        diamonds+=30; //добавляємо кількість кристаликів
        ui->pushButton->setText(QString::number(diamonds));
        game=false;
        timer->stop(); //зупиняємо таймер

        //виводимо звук перемоги
        QMediaPlayer *soundPlayer = new QMediaPlayer(this);
        QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);
        soundPlayer->setSource(audioUrlwin);
        playerOutput->setVolume(2);
        soundPlayer->setAudioOutput(playerOutput);
        soundPlayer->play();

        //виводимо повідомлення про перемогу
        QMessageBox::about(this, "Перемога", "Вітаю з перегою. Кількість спроб, яких залишилося: "+QString::number(tries)+" спроб. Ви вгадали слово за "+ui->timerLabel->text());
    }

    //якщо спроби закінчилися, то оголошуємо поразку
    if(tries<=0)
    {
        switch_buttons(false); //виключаємо всі кнопки
        ui->pushButton_4->show(); //показуємо кнопку "Грати знову"
        timer->stop(); //зупиняємо таймер
        game=false;
        ui->label_2->setText(word); //виводимо загадане слово

        //виводимо звук поразки
        QMediaPlayer *soundPlayer = new QMediaPlayer(this);
        QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);
        soundPlayer->setSource(audioUrlend);
        playerOutput->setVolume(2);
        soundPlayer->setAudioOutput(playerOutput);
        soundPlayer->play();

        //виводимо повідомлення про поразку
        QMessageBox::about(this, "Поразка", "Поразка – це лише один з варіантів розвитку подій, який потрібно відкинути через непотрібність.\nКількість вгаданих букв: "+QString::number(found)+". Час: "+ui->timerLabel->text());
    }

    return mark;
}

void MainWindow::on_pushButton_5_clicked()
{
    if (isPlaying)
    {
        // вимкнути звук, якщо музика вже грає
        soundPlayer->stop();
        isPlaying = false;

        //замінити іконку на виключений динамік
        QPixmap pixmap(":/images/off.png");
        QPixmap resizedPixmap = pixmap.scaled(80, 80, Qt::KeepAspectRatio);
        ui->pushButton_5->setIcon(QIcon(resizedPixmap));
        ui->pushButton_5->setIconSize(resizedPixmap.size());
    }
    else
    {
        // включити звук, якщо музика не грає
        soundPlayer->play();
        isPlaying = true;

        //замінити іконку на включений динамік
        QPixmap pixmap(":/images/on.png");
        QPixmap resizedPixmap = pixmap.scaled(80, 80, Qt::KeepAspectRatio);
        ui->pushButton_5->setIcon(QIcon(resizedPixmap));
        ui->pushButton_5->setIconSize(resizedPixmap.size());
    }
}

QUrl audioUrlright("qrc:/images/right.wav"); //створюємо QUrl для вгаданої букви
QUrl audioUrlwrong("qrc:/images/wrong.wav"); //створюємо QUrl для невгаданої букви

void MainWindow::on_pushButton_A_clicked()
{
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("а")) //передаємо в функцію check букву а
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}"; //якщо буква є в слові, то встановлюється фон галочки
        ui->pushButton_A->setStyleSheet(styleSheet);
        //якщо гра ще не закінчена, виводимо звук для вгаданої букви
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}"; //якщо букви немає в слові, то встановлюється фон хрестика
        ui->pushButton_A->setStyleSheet(styleSheet);
        //якщо гра ще не закінчена, виводимо звук для невгаданої букви
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_A->setEnabled(false); //робимо кнопку недоступною
}

void MainWindow::on_pushButton_B_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("б"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_B->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_B->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_B->setEnabled(false);
}


void MainWindow::on_pushButton_V_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("в"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_V->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_V->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_V->setEnabled(false);
}


void MainWindow::on_pushButton_G_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("г"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_G->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_G->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_G->setEnabled(false);
}


void MainWindow::on_pushButton_GE_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ґ"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_GE->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_GE->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_GE->setEnabled(false);
}


void MainWindow::on_pushButton_D_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("д"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_D->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_D->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_D->setEnabled(false);
}


void MainWindow::on_pushButton_E_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("е"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_E->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_E->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_E->setEnabled(false);
}


void MainWindow::on_pushButton_YE_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("є"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_YE->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_YE->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_YE->setEnabled(false);
}


void MainWindow::on_pushButton_ZH_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ж"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_ZH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_ZH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_ZH->setEnabled(false);
}


void MainWindow::on_pushButton_Z_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("з"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_Z->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_Z->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_Z->setEnabled(false);
}


void MainWindow::on_pushButton_U_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("и"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_U->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_U->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_U->setEnabled(false);
}


void MainWindow::on_pushButton_I_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("і"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_I->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_I->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_I->setEnabled(false);
}


void MainWindow::on_pushButton_YI_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ї"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_YI->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_YI->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_YI->setEnabled(false);
}


void MainWindow::on_pushButton_Y_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("й"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_Y->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_Y->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_Y->setEnabled(false);
}

void MainWindow::on_pushButton_K_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("к"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_K->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_K->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_K->setEnabled(false);
}


void MainWindow::on_pushButton_L_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("л"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_L->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_L->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_L->setEnabled(false);
}


void MainWindow::on_pushButton_M_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("м"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_M->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_M->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_M->setEnabled(false);
}


void MainWindow::on_pushButton_N_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("н"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_N->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_N->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_N->setEnabled(false);
}


void MainWindow::on_pushButton_O_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("о"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_O->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_O->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_O->setEnabled(false);
}


void MainWindow::on_pushButton_P_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("п"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_P->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_P->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_P->setEnabled(false);
}


void MainWindow::on_pushButton_R_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("р"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_R->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_R->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_R->setEnabled(false);
}


void MainWindow::on_pushButton_C_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("с"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_C->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_C->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_C->setEnabled(false);
}


void MainWindow::on_pushButton_T_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("т"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_T->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_T->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_T->setEnabled(false);
}


void MainWindow::on_pushButton_U_2_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("у"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_U_2->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_U_2->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_U_2->setEnabled(false);
}


void MainWindow::on_pushButton_F_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ф"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_F->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_F->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_F->setEnabled(false);
}


void MainWindow::on_pushButton_X_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("х"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_X->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_X->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_X->setEnabled(false);
}


void MainWindow::on_pushButton_TS_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ц"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_TS->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_TS->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_TS->setEnabled(false);
}


void MainWindow::on_pushButton_CH_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ч"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_CH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_CH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_CH->setEnabled(false);
}


void MainWindow::on_pushButton_SH_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ш"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_SH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_SH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_SH->setEnabled(false);
}


void MainWindow::on_pushButton_SHCH_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("щ"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_SHCH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_SHCH->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_SHCH->setEnabled(false);
}


void MainWindow::on_pushButton_QQ_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ь"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_QQ->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_QQ->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_QQ->setEnabled(false);
}


void MainWindow::on_pushButton_YU_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("ю"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_YU->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_YU->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_YU->setEnabled(false);
}


void MainWindow::on_pushButton_YA_clicked()
{
    //аналогічно до on_pushButton_A_clicked()
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *playerOutput = new QAudioOutput(soundPlayer);

    if(check("я"))
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/right.png);""}";
        ui->pushButton_YA->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlright);
    }
    else
    {
        QString styleSheet = "QPushButton {" "background-image: url(:/images/wrong.png);""}";
        ui->pushButton_YA->setStyleSheet(styleSheet);
        if(game)
        soundPlayer->setSource(audioUrlwrong);
    }
    playerOutput->setVolume(0.4);
    soundPlayer->setAudioOutput(playerOutput);
    soundPlayer->play();
    ui->pushButton_YA->setEnabled(false);
}
