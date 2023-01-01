#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    bool input = false;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readMassive(double *mas, int *size, bool *error); //функция для считывания массива
    void writeMassive(double *mas, int size);
    void values();

private slots:
    void clearsearch();

    void clear();

    bool problems(int *mas);

    int getRowCount();

    void sinch(int *mas);

    void quickSort(int left, int right, int *mas);

    void izmen();

    bool correct(int size, int *mas);

    void shuffle(int size, int *mas);

    void on_delbutton_clicked();

    void on_sizebox_valueChanged(int arg1);

    void on_inputbutton_clicked();

    void on_monkeybutton_clicked();

    void on_arraytable_itemChanged(QTableWidgetItem *item);

    void on_raschbutton_clicked();

    void on_gnombutton_clicked();

    void on_puzirbutton_clicked();

    void on_fastbutton_clicked();

    void on_searchbutton_clicked();

    void on_minbutton_clicked();

    void on_maxbutton_clicked();

    void on_medbutton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
