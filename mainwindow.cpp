#include "banco.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtDebug>
#include <QModelIndexList>
#include <QItemSelectionModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::limpaForm()
{
    ui->txtCodigo->setText("");
    ui->txtNome->setText("");
    ui->txtEmail->setText("");
    ui->txtIdade->setText("");
    ui->txtCidade->setText("");
    ui->rbMasculino->setAutoExclusive(false);
    ui->rbMasculino->setChecked(false);
    ui->rbFeminino->setAutoExclusive(false);
    ui->rbFeminino->setChecked(false);
    return 0;
}

void MainWindow::on_btnSalvar_clicked()
{
    int codigo;
    QString nome;
    QString email;
    int  idade;
    QString cidade;
    QString sexo;

    codigo = ui->txtCodigo->text().toInt();
    nome = ui->txtNome->text();
    email = ui->txtEmail->text();
    idade = ui->txtIdade->text().toInt();
    cidade = ui->txtCidade->text();

    if (ui->rbMasculino->isChecked()) {
        sexo = 'm';
    } else if(ui->rbFeminino->isChecked()) {
        sexo = 'f';
    }

    util::Banco banco;
    banco.conecta();

    if (
        nome == ""
        || email == ""
        || idade < 0
        || cidade == ""
    ) {
        QMessageBox messageBox;
        messageBox.critical(0,"Aviso","Você deve preencher todos os campos!");
    } else if(banco.ok) {
        QSqlQuery query(banco.db);

        if (codigo > 0) {
            query.prepare(" UPDATE usuarios"
                          " SET nome = :nome"
                          " ,email = :email"
                          " ,sexo = :sexo"
                          " ,idade = :idade"
                          " ,cidade = :cidade "
                          " WHERE id = :id");
            query.bindValue(":id", QString::number(codigo));
        } else {
            query.prepare("INSERT INTO usuarios("
                          " nome"
                          " ,email"
                          " ,sexo"
                          " ,idade"
                          " ,cidade"
                          " ) VALUES ("
                          " :nome"
                          " ,:email"
                          " ,:sexo"
                          " ,:idade"
                          " ,:cidade"
                          " )");
        }

        query.bindValue(":nome", nome);
        query.bindValue(":email", email);
        query.bindValue(":sexo", sexo);
        query.bindValue(":idade", QString::number(idade));
        query.bindValue(":cidade", cidade);

        if (query.exec()) {
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Aviso"));

            if (codigo > 0) {

                messageBox.setText(tr("Pessoa atualizada sucesso"));
            } else {
                messageBox.setText(tr("Pessoa incluida com sucesso"));
                limpaForm();
            }

            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.exec();

        } else {
            QMessageBox messageBox;

            if (codigo > 0) {
                messageBox.critical(0,"Erro","Falha ao atualizar pessoa!");
            } else {
                messageBox.critical(0,"Erro","Falha ao incluir pessoa!");
            }

            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.exec();
        }
    } else {
        QMessageBox messageBox;
        messageBox.critical(0,"Erro","falha ao conetar a base de dados!");
    }

    carregaTudo();
    banco.fecha();
}

void MainWindow::on_btnLimpar_clicked()
{
     limpaForm();
}

int MainWindow::carregaTudo()
{
    util::Banco banco;
    banco.conecta();

    if(banco.ok) {
        QSqlQueryModel * model = new QSqlQueryModel();
        QSqlQuery *query = new QSqlQuery(banco.db);
        query->prepare("SELECT id as codigo,nome,email AS 'E-mail',sexo,idade,cidade FROM usuarios");
        query->exec();
        model->setQuery(*query);
        ui->tvPessoas->setModel(model);
    }

    banco.fecha();
    return 0;
}

void MainWindow::on_btnListar_clicked()
{
    carregaTudo();
}

void MainWindow::on_tvPessoas_activated(const QModelIndex &index)
{
    int row = index.row();
    ui->txtCodigo->setText(index.sibling(row, 0).data().toString());
    ui->txtNome->setText(index.sibling(row, 1).data().toString());
    ui->txtEmail->setText(index.sibling(row, 2).data().toString());
    QString sexo = index.sibling(row, 3).data().toString();

    if (sexo == "m") {
        ui->rbMasculino->setChecked(true);
        ui->rbFeminino->setChecked(false);
    } else if(sexo == "f") {
        ui->rbFeminino->setChecked(true);
        ui->rbMasculino->setChecked(false);
    }

    ui->txtIdade->setText(index.sibling(row, 4).data().toString());
    ui->txtCidade->setText(index.sibling(row, 5).data().toString());

    //qDebug() << "uma mensagem para debugar";
}

void MainWindow::on_btnExcluir_clicked()
{
    int codigo;
    codigo = ui->txtCodigo->text().toInt();

    util::Banco banco;
    banco.conecta();

    if(banco.ok) {
        QSqlQuery query(banco.db);

        if (codigo > 0) {
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Exclusão"));
            messageBox.setText(tr("Tem certeza que deseja excluir esta pessoa?"));
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            messageBox.setDefaultButton(QMessageBox::No);
            if (messageBox.exec() == QMessageBox::Yes) {
                query.prepare(" DELETE FROM usuarios"
                              " WHERE id = :id");
                query.bindValue(":id", QString::number(codigo));

                if (query.exec()) {
                    QMessageBox messageBox;
                    messageBox.setWindowTitle(tr("Aviso"));
                    messageBox.setText(tr("Pessoa excluida com sucesso"));
                    limpaForm();
                    messageBox.setStandardButtons(QMessageBox::Ok);
                    messageBox.exec();
                } else {
                    QMessageBox messageBox;
                    messageBox.setWindowTitle(tr("Erro"));
                    messageBox.setText(tr("falha ao excluir pessoa"));
                    messageBox.setStandardButtons(QMessageBox::Ok);
                    messageBox.exec();
                }
            }
       }
    }

    carregaTudo();
    banco.fecha();
}
