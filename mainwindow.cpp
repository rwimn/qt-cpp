#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) : // Конструктор
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->arraytable->setColumnCount(1);
    ui->minlabel->setText("-123");
    ui->maxlabel->setText("-");
    ui->medlabel->setText("-");
    ui->sizebox->setMaximum(1000000);
    fillState = false;
}

MainWindow::~MainWindow(){ // Деструктор
    delete ui;
}

//"Очистка" значений минимума, максимума и среднего значения массива и дубликатов, поисковыых строки и вывода
void MainWindow::clear(){
    ui->minlabel->setText("-");
    ui->maxlabel->setText("-");
    ui->medlabel->setText("-");
    ui->searchline->setText("");
    ui->includeedit->setText("");
}

//"Очистка" значений поисковыых строки и вывода
void MainWindow::clearsearch(){
    ui->searchline->setText("");
    ui->includeedit->setText("");
}

//"Реакция" на нажатую кнопку "Заполнение"
void MainWindow::on_inputbutton_clicked(){
    clear();
    int size = getRowCount();

    fillState = true;
    ui->arraytable->clear();

    for(int i = 0; i < size; i++){
        ui->arraytable->setItem(i, 0, new QTableWidgetItem(QString::number(QRandomGenerator::global()->bounded(1, size))));
    }

    fillState = false;
}

//Проверка элементов таблицы на корректные значения
bool MainWindow::problems(int *mas){
    bool retval = true;
    if (ui->sizebox->value() == 0){
        QMessageBox::warning(this, "Ошибка", "Нулевой размер массива", QMessageBox::Ok);
        retval = false;
        clear();
    } else {
        for (int i = 0; i < getRowCount(); i++){
            if (ui->arraytable->item(i, 0) == nullptr){
                clear();
                ui->arraytable->setItem(i, 0, new QTableWidgetItem(""));
                retval = false;
                QMessageBox::warning(this, "Ошибка", "Не все ячейки заполнены");
                break;
            }
            int box = ui->arraytable->item(i, 0)->text().toInt(&retval);
            if (!retval) {
                clear();
                QMessageBox::warning(this, "Ошибка", "Некорректное значение в ячейке(ах)");
                break;
            } else {
                retval = true;
                if (mas != NULL){
                    mas[i] = box;
                }
            }
        }
    }
    return retval;
}

//Перезаполнение таблицы изменённым массивом с удалёнными дубликатами
void MainWindow::sinch(int *mas){
    for (int i = 0; i < getRowCount(); i++){
        QString numberText = QString::number(mas[i]);
        ui->arraytable->setItem(i, 0, new QTableWidgetItem(numberText));
    }
}

//"Реакция" на изменённый счётчик количества элементов в массиве
void MainWindow::on_sizebox_valueChanged(int newRowCount){
    if (fillState)
    {
        fillState = false;
        return;
    }
    ui->arraytable->setRowCount(newRowCount);
    clear();
    clearLighting();
}

int MainWindow::getRowCount() {
    return ui->arraytable->rowCount();
}

//Быстрая сортировка - алгоритм
void MainWindow::quickSort(int left, int right, int *mas){
    int pivot;
    int l_hold = left;
    int r_hold = right;
    pivot = mas[left];
    while (left < right){
      while ((mas[right] >= pivot) && (left < right)){
        right--;
      }
      if (left != right){
        mas[left] = mas[right];
        left++;
      }
      while ((mas[left] <= pivot) && (left < right)){
        left++;
      }
      if (left != right){
        mas[right] = mas[left];
        right--;
      }
    }
    mas[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot){
      quickSort(left, pivot - 1, mas);
    }
    if (right > pivot){
        quickSort(pivot + 1, right, mas);
    }
}

//Проверка на повторения
bool MainWindow::correct(int size, int *mas){
    bool ok7 = false;
    for(int i = 0; i < size - 1; i++){
        if (mas[i] > mas[i+1]){
            ok7 = true;
            break;
        }
    }
    if(ok7){
        return true;
    }else{
        return false;
    }
}

//Сортировка гномом - алгоритм
void MainWindow::shuffle(int size, int *mas){
    for (int i = 0; i < size; i++){
        int a = mas[i];
        int ran = QRandomGenerator::global()->bounded(0, size-1);
        int b = mas[ran];
        mas[i] = b;
        mas[ran] = a;
    }
}

//Обезьянья сортировка - "реакция" на нажатую кнопку
void MainWindow::on_monkeybutton_clicked(){
    if(getRowCount() > 10){
        clearsearch();
        QMessageBox::warning(this,"Ошибка", "Обезьянья сортировка принимает массив размера не более 10 элементов", QMessageBox::Ok);
    }else{
        int* mas_number = new int[getRowCount()];
        if(problems(mas_number)){
            while (correct(getRowCount(), mas_number)){
                    shuffle(getRowCount(), mas_number);
            }
            sinch(mas_number);
        }
        delete [] mas_number;
        mas_number = NULL;
    }

}

//"Реакция" на измённое значение в таблице (не относится к сортировкам)
void MainWindow::on_arraytable_itemChanged(QTableWidgetItem *item){

    if (this->fillState)
        return;

    QString text = item -> text();

    bool ok;
    text.toInt(&ok);

    if(text.toInt()){
        item -> setBackground(Qt::white);
    }
    else{
        item->setBackground(Qt::red);
    }

    //ui->lable_searchCount->clear();
    ui->searchline->clear();
    ui->includeedit->clear();
}


//Сортировка рсчёской - "реакция" на нажатую кнопку
void MainWindow::on_raschbutton_clicked(){
    int* mas_number = new int[getRowCount()];
    if(problems(mas_number)){
        if(getRowCount() >= 400000){
            QMessageBox::information(this, "Внимание", "Этот процесс будет длиться 5 или более секунд");
        }
        int shag = getRowCount();
        bool flagUA = true;
        int vrem;
        while(shag > 1 or flagUA){
            if(shag > 1){
                shag = floor(shag / 1.247);
            }
                flagUA = false;
                int i = 0;
                while(i + shag < getRowCount()){
                    if(mas_number[i] > mas_number[i+shag]){
                        vrem = mas_number[i];
                        mas_number[i] = mas_number[i+shag];
                        mas_number[i+shag] = vrem;
                        flagUA = true;
                    }
                    i++;
                }
            }
       sinch(mas_number);
    }

    delete [] mas_number;
    mas_number = NULL;

}

//Сортировка гномом - "реакция" на нажатую кнопку
void MainWindow::on_gnombutton_clicked(){
    int* mas_number = new int[getRowCount()];
    if(getRowCount() > 100000){
        QMessageBox::warning(this,"Ошибка", "Слишком большое число");
    } else {
        if(problems(mas_number)){
            if(getRowCount() > 50000){
                QMessageBox::information(this, "Внимание", "Этот процесс будет длиться 5 или более секунд");
            }
            int i = 0;
            int j = 0;
            int vrem;
            int vernis = 0;
            bool na_meste = true;
            while (i < getRowCount()){
                if(i==0 or mas_number[i-1] <= mas_number[i]){
                    if(!na_meste){
                        na_meste = true;
                        i = vernis;
                    }
                    i++;
                }else{
                    if(na_meste){
                        vernis = i;
                    }
                    na_meste = false;
                    j = i - 1;
                    vrem = mas_number[i];
                    mas_number[i] = mas_number[j];
                    mas_number[j] = vrem;
                    i = j;
                }
            }
            sinch(mas_number);
        }
    }

    delete [] mas_number;
    mas_number = NULL;

}

//Сортировка пузырьком - "реакция" на нажатую кнопку
void MainWindow::on_puzirbutton_clicked(){
    int* mas_number = new int[getRowCount()];
    if(getRowCount() > 50000){
        QMessageBox::warning(this,"Ошибка", "Слишком большие числа");
    }else{
        if(getRowCount() > 25000){
            QMessageBox::information(this, "Внимание", "Этот процесс будет длиться 5 или более секунд");
        }
        if(problems(mas_number)){
            int i = 0;
            int j;
            int vrem;
            while(i < getRowCount()-1){
                j = i+1;
                while(j < getRowCount()){
                    if(mas_number[i] > mas_number[j]){
                        vrem = mas_number[i];
                        mas_number[i] = mas_number[j];
                        mas_number[j] = vrem;
                    }
                j = j+1;
                }
            i = i+1;
            }
            sinch(mas_number);
        }
    }

    delete [] mas_number;
    mas_number = NULL;

}

//Быстрая сортировка - "реакция" на нажатую кнопку
void MainWindow::on_fastbutton_clicked(){
    int* mas_number = new int[getRowCount()];

    if(problems(mas_number)){
        if(getRowCount() >= 400000){
            QMessageBox::information(this, "Внимание", "Этот процесс будет длиться 5 или более секунд");
        }
        quickSort(0, getRowCount()-1, mas_number);
        sinch(mas_number);
    }

    delete [] mas_number;
    mas_number = NULL;

}

// Очистка подсветки ранее выделенных строк
void MainWindow::clearLighting()
{
    fillState = true;

    for(int i = 0; i < getRowCount(); i++) {
        auto item = ui->arraytable->item(i,0);
        if (item != nullptr) {
            QString text = item->text();

            bool ok;
            text.toInt(&ok);

            if(text.toInt()){
                item->setBackground(Qt::white);
            } else {
                item->setBackground(Qt::red);
            }
        }
    }

    fillState = false;
}

//"Реакция" на нажатую кнопку "Поиск"
void MainWindow::on_searchbutton_clicked(){

    ui->includeedit->setText("");
    clearLighting();

    int* mas_number = new int[getRowCount()];
    if(problems(mas_number)){
        bool ok89;
        int kolvo = 0;
        bool sovpad = true;
        int found = ui->searchline->text().toInt(&ok89);
        QString otvet ="";
        int index[20000];
        for(int i = 0; i < getRowCount()-1; i++)
        {
            if(mas_number[i] > mas_number[i+1])
            {
                sovpad = false;
                break;
            }
        }
        if(sovpad){
            if(ok89){
                int left = 0;
                int right = getRowCount() - 1;
                int med = (left + right) / 2;

                while (left <= right && found != mas_number[med])
                {
                    med = (left + right) / 2;
                    if (found < mas_number[med])
                    {
                        right = med - 1;
                    }
                    else if (found > mas_number[med])
                    {
                        left = med + 1;
                    }
                    else
                    {
                        break;
                    }

                }
                //med = (left + right) / 2;
                //qDebug () << med << " " << left << "-" << right;
                if (found == mas_number[med])
                {
                    for (int i = 0; i < getRowCount(); i++)
                    {
                        if (mas_number[i] == found)
                        {
                            fillState = true;
                            ui->arraytable->item(i, 0)->setBackground(Qt::green);
                            fillState = false;
                            index[kolvo] = i + 1;
                            kolvo ++;

                        }
                    }
                }
                else
                {
                    ok89 = false;
                }

            }
            else{
                QMessageBox::warning(this, "Ошибка", "Некорректный запрос значения при поиске");
            }
        }
        else
        {
            if(ok89)
            {
                for(int i = 0; i < getRowCount(); i++)
                {
                    if(mas_number[i] == found)
                    {
                        index[kolvo] = i+1;
                        kolvo++;
                        fillState = true;
                        ui->arraytable->item(i,0)->setBackground(Qt::green);
                        fillState = false;
                        ui->medlabel->show();
                        ui->maxlabel->show();
                        ui->minlabel->show();
                    }
                }
            }
            else{
                QMessageBox::warning(this, "Ошибка", "Некорректный запрос значения при поиске");
            }
         }
        if(ok89){
            if(kolvo > 0){
                for(int i = 0; i < kolvo; i ++){
                    otvet += QString::number(index[i]) + " ";
                    //ui->lable_searchCount->setNum(kolvo);
                    QString res = "Число вхождений: " + QString::number(kolvo) + "\n" + "Номера вхождений: " + otvet;
                    ui->includeedit->setText(res);
                }
            }
            else{
                clearsearch();
                QMessageBox::warning(this, "", "Нет чисел");
            }
        }
        else
        {
            clearsearch();
            QMessageBox::warning(this, "", "Нет чисел");
        }
    }

    delete [] mas_number;
    mas_number = NULL;
}

//"Реакция" на нажатую кнопку "Удааление дубликатов"
void MainWindow::on_delbutton_clicked(){
    int* mas_number = new int[getRowCount()];

    if(problems(mas_number)){
        bool sovpad = true;
        for(int i = 0; i < getRowCount()-1; i++)
        {
            if(mas_number[i] > mas_number[i+1])
            {
                sovpad = false;
                break;
            }
        }
        if(sovpad){
            int rows = 1;
            int point = mas_number[0];
            mas_number[0] = point;
            for(int i = 1; i < getRowCount(); i++)
            {
                if(point == mas_number[i])
                {
                    continue;
                }
                else
                {
                    point = mas_number[i];
                    mas_number[rows] = point;
                    rows++;
                }
            }
            fillState = true;
            ui->sizebox->setValue(rows);
            ui->arraytable->setRowCount(rows);
            fillState = false;
            for (int i = 0; i < rows; i++)
            {
                QString numberText = QString::number(mas_number[i]);
                fillState = true;
                ui->arraytable->setItem(i, 0, new QTableWidgetItem(numberText));
                fillState = false;
            }
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Необходимо отсортировать массив");
        }

    }
    clearsearch();
    ui->medlabel->setText("-");
    delete [] mas_number;
    mas_number = NULL;
}

//Расчёт минимума, максимума и среднего значения массива - алгоритм
void MainWindow::values(){
    int* mas_number = new int[getRowCount()];

    if(problems(mas_number))
    {
        int minNumber = 1e9;
        int maxNumber = -1;
        qint64 avrNumber = 0;

        for(int i = 0; i < getRowCount(); i ++)
        {
            if(mas_number[i] < minNumber)
            {
                minNumber = mas_number[i];
            }
            if(mas_number[i] > maxNumber){
                maxNumber = mas_number[i];
            }
        avrNumber += mas_number[i];

        }

        ui->minlabel->show();
        ui->maxlabel->show();
        ui->medlabel->show();

        QString srAvg = QString::number(avrNumber/getRowCount());

        ui->medlabel->setText(srAvg);
        ui->maxlabel->setNum(maxNumber);
        ui->minlabel->setNum(minNumber);
    }

    delete [] mas_number;
    mas_number = NULL;
}

//"Реакция" на нажание кнопки "Минимум"
void MainWindow::on_minbutton_clicked()
{
    values();
}

//"Реакция" на нажание кнопки "Среднее значение"
void MainWindow::on_maxbutton_clicked()
{
    values();
}

//"Реакция" на нажание кнопки "Максимум"
void MainWindow::on_medbutton_clicked()
{
    values();
}

