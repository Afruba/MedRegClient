#ifndef H_PD
#define H_PD
#include <QWidget>
#include <QString>

class QLineEdit;
class PasswordDialog: public QWidget{
Q_OBJECT
public:
	PasswordDialog(QString un, QString up);
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
	QString un = "";
	QString up = "";
};
#endif