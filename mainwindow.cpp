#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>

bool founder = false;
bool clasd = false;
QString temp1 = "-1";
int temp2 = -1;
int temp3 = -1;
bool ter = false;

MainWindow::MainWindow(QWidget *parent) : // Конструктор
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->arraytable->setColumnCount(1);
    ui->minlabel->setText("-");
    ui->maxlabel->setText("-");
    ui->medlabel->setText("-");
    ui->sizebox->setMaximum(1000000);
}

MainWindow::~MainWindow(){ // Деструктор
    delete ui;
}

//"Очистка" значений минимума, максимума и среднего значения массива и дубликатов, поисковыых строки и вывода
void MainWindow::clear(){
    ui->minlabel->setText("-");
    ui->maxlabel->setText("-");
    ui->medlabel->setText("-");
    QString temp1 = "-1";
    int temp2 = -1;
    int temp3 = -1;
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
    int size = ui->sizebox->value();
    for(int i = 0; i < getRowCount(); i++){
        this->input = true;
        ui->arraytable->setItem(i,0,new QTableWidgetItem(QString::number(QRandomGenerator::global()->bounded(1, size))));
    }
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
    ui->arraytable->setRowCount(newRowCount);
    clear();
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

    if(ter){
        ui->medlabel->setText(temp1);
        ui->maxlabel->setNum(temp2);
        ui->minlabel->setNum(temp3);
        ter = false;
    }else{
        ui->medlabel->setText("-");
        ui->maxlabel->setText("-");
        ui->minlabel->setText("-");
    }
}

//"Реакция" на измённое значение в таблице (не относится к сортировкам)
void MainWindow::on_arraytable_itemChanged(QTableWidgetItem *item){

    if (this->input)
    {
        this->input = false;
        return;
    }
    if (item == nullptr)
    {
        return;
    }

    QString text = item -> text();

    bool ok;
    text.toInt(&ok);

    if(text.toInt()){

        if(!founder)
            item -> setBackground(Qt:: white);
            clasd = true;
    }
    else{
        item->setBackground(Qt:: red);
    }

    //ui->lable_searchCount->clear();
    ui->searchline->clear();
    ui->includeedit->clear();
}


//Сортировка рсчёской - "реакция" на нажатую кнопку
void MainWindow::on_raschbutton_clicked(){
    int* mas_number = new int[getRowCount()];
    clasd = true;
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

    if(temp2 != -1){
        ui->medlabel->setText(temp1);
        ui->maxlabel->setNum(temp2);
        ui->minlabel->setNum(temp3);
    }
}

//Сортировка гномом - "реакция" на нажатую кнопку
void MainWindow::on_gnombutton_clicked(){
    int* mas_number = new int[getRowCount()];
    clasd = true;
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

    if(ter){
        ui->medlabel->setText(temp1);
        ui->maxlabel->setNum(temp2);
        ui->minlabel->setNum(temp3);
        ter = false;
    }else{
        ui->medlabel->setText("-");
        ui->maxlabel->setText("-");
        ui->minlabel->setText("-");
    }
}

//Сортировка пузырьком - "реакция" на нажатую кнопку
void MainWindow::on_puzirbutton_clicked(){
    int* mas_number = new int[getRowCount()];
    clasd = true;
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

    if(ter){
        ui->medlabel->setText(temp1);
        ui->maxlabel->setNum(temp2);
        ui->minlabel->setNum(temp3);
        ter = false;
    }else{
        ui->medlabel->setText("-");
        ui->maxlabel->setText("-");
        ui->minlabel->setText("-");
    }
}

//Быстрая сортировка - "реакция" на нажатую кнопку
void MainWindow::on_fastbutton_clicked(){
    int* mas_number = new int[getRowCount()];
    clasd = true;

    if(problems(mas_number)){
        if(getRowCount() >= 400000){
            QMessageBox::information(this, "Внимание", "Этот процесс будет длиться 5 или более секунд");
        }
        quickSort(0, getRowCount()-1, mas_number);
        sinch(mas_number);
    }

    delete [] mas_number;
    mas_number = NULL;

    if(ter){
        ui->medlabel->setText(temp1);
        ui->maxlabel->setNum(temp2);
        ui->minlabel->setNum(temp3);
        ter = false;
    }else{
        ui->medlabel->setText("-");
        ui->maxlabel->setText("-");
        ui->minlabel->setText("-");
    }
}

//"Реакция" на нажатую кнопку "Поиск"
void MainWindow::on_searchbutton_clicked(){

    ui->includeedit->setText("");
    //ui->lable_searchCount->setText("");
    for(int i = 0; i <getRowCount(); i++){
        input = true;
        ui->arraytable->item(i,0)->setBackground(Qt::white);
    }

    int* mas_number = new int[getRowCount()];
    if(problems(mas_number)){
        bool ok89;
        int kolvo = 0;
        bool sovpad = true;
        int found = ui->searchline->text().toInt(&ok89);
        founder = true;
        QString otvet ="";
        int index[200];
        for(int i = 0; i < getRowCount()-1; i++){
            if(mas_number[i] > mas_number[i+1]){
                sovpad = false;
                break;
            }
        }
        if(sovpad){
            if(ok89){
                bool da = false;
                int count = 0;
                int del = floor(getRowCount()/2);
                int kray = del;
                while(!da){
                    if(mas_number[del] == found){
                        da = true;
                        for(int i = del; i >-1; i--){
                            if(mas_number[i] == found){
                                kray = i;
                            }else{
                                break;
                            }
                        }
                        for(int i = kray; i < del; i++){
                            index[kolvo] = i+1;
                            kolvo++;
                            ui->arraytable->item(i,0)->setBackground(Qt::green);
                            ui->medlabel->show();
                            ui->maxlabel->show();
                            ui->minlabel->show();
                        }

                        for(int i = del; i < getRowCount(); i++){
                            if(mas_number[i] == found){
                                index[kolvo] = i+1;
                                kolvo++;
                                ui->arraytable->item(i,0)->setBackground(Qt::green);
                                ui->medlabel->show();
                                ui->maxlabel->show();
                                ui->minlabel->show();
                            }else{
                                break;
                            }
                        }
                    }else if(mas_number[del] > found){
                        del = floor(del/2);
                    }else if(mas_number[del] < found){
                        if(count < 2){
                            del ++;
                        }else{
                            del += floor((getRowCount()-del)/2);
                            count++;
                        }
                    }
                }
            }
            else{
                QMessageBox::warning(this, "Ошибка", "Некорректный запрос значения при поиске");
                founder = false;
            }
        }
        else{
            if(ok89){
                for(int i = 0; i < getRowCount(); i++){
                    if(mas_number[i] == found){
                        index[kolvo] = i+1;
                        kolvo++;
                        ui->arraytable->item(i,0)->setBackground(Qt::green);
                        ui->medlabel->show();
                        ui->maxlabel->show();
                        ui->minlabel->show();
                    }
                }
            }
            else{
                QMessageBox::warning(this, "Ошибка", "Некорректный запрос значения при поиске");
                founder = false;
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
        founder = false;
    }

    delete [] mas_number;
    mas_number = NULL;
}

//"Реакция" на нажатую кнопку "Удааление дубликатов"
void MainWindow::on_delbutton_clicked(){
    int* mas_number = new int[getRowCount()];

    if(problems(mas_number)){
        bool sovpad = true;
        for(int i = 0; i < getRowCount()-1; i++){
            if(mas_number[i] > mas_number[i+1]){
                sovpad = false;
                break;
            }
        }
        if(sovpad){
            int rows = 1;
            int point = mas_number[0];
            mas_number[0] = point;
            for(int i = 1; i < getRowCount(); i++){
                if(point == mas_number[i]){
                    continue;
                }else{
                    point = mas_number[i];
                    mas_number[rows] = point;
                    rows++;
                }
            }
            ui->sizebox->setValue(rows);
            for (int i = 0; i < rows; i++){
                QString numberText = QString::number(mas_number[i]);
                ui->arraytable->setItem(i, 0, new QTableWidgetItem(numberText));
            }
        }else{
            QMessageBox::warning(this, "Ошибка", "Необходимо отсортировать массив");
        }

    }

    delete [] mas_number;
    mas_number = NULL;
}

//Расчёт минимума, максимума и среднего значения массива - алгоритм
void MainWindow::values(){
    int* mas_number = new int[getRowCount()];

    if(problems(mas_number)){
        int minNumber = 1e9;
        int maxNumber = -1;
        qint64 avrNumber = 0;

        for(int i = 0; i < getRowCount(); i ++){
            if(mas_number[i] < minNumber){
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
        temp1 = srAvg;
        temp2 = maxNumber;
        temp3 = minNumber;
        ter = true;
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

