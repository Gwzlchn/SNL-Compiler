#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_InputToken_clicked();

    void on_save_clicked();

    void on_InputProductionset_clicked();

    void on_FollowTable_clicked();

    void on_TokenList_clicked();

    void on_FirstTable_clicked();

    void on_PredicctTable_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
