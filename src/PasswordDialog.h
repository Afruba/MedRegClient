#ifndef H_PD
#define H_PD
#include <QWidget>
#include <QString>
#include <pqxx/pqxx>
class QLineEdit;
class PasswordDialog: public QWidget{
Q_OBJECT
public:
	PasswordDialog(pqxx::connection* sql_cn);
	~PasswordDialog();
signals:
	void user_is_enter(bool status);
public slots:
	void check_input();
	void closeEvent(QCloseEvent *event);
private:
	bool ret_status = false;
	QLineEdit *le_n;
	QLineEdit *le_p;
	pqxx::connection* sql_cn;
	bool label_is_added = false;
};
#endif