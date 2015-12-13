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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    limpaForm();
    carregaTudo();
    validaSalvar();

private slots:
    void on_btnSalvar_clicked();

    void on_btnLimpar_clicked();

    void on_btnListar_clicked();

    void on_tvPessoas_activated(const QModelIndex &index);

    void on_btnExcluir_clicked();

    void on_rbMasculino_clicked();

    void on_rbFeminino_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
