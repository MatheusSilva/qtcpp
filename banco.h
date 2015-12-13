#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

namespace util
{
    class Banco: public QSqlDatabase
    {

    public:
        QString tipoBanco = "QMYSQL";
        QString host      = "localhost";
        int     porta     = 3306;
        QString banco     = "qtcpp";
        QString usuario   = "root";
        QString senha     = "";
        bool    ok        = false;
        QSqlDatabase db;

        QSqlDatabase conecta()
        {
            if (ok) {
                return db;
            } else {
                QSqlDatabase con = QSqlDatabase::addDatabase(tipoBanco);
                con.setHostName(host);
                con.setPort(porta);
                con.setDatabaseName(banco);
                con.setUserName(usuario);
                con.setPassword(senha);
                ok = con.open();
                db = con;
                return db;
           }
        }

        void fecha()
        {
            if (ok) {
                db.close();
                ok = false;
            }
        }
    };
}

