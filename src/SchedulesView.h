#ifndef SCHEDULES_H
#define SCHEDULES_H
#include <QWidget>
#include <pqxx/pqxx>
class QPushButton; class QLabel;
class SchedulesView : public QWidget{
	Q_OBJECT
public:
	SchedulesView(pqxx::connection *sql_cn);
	~SchedulesView();
public slots:
	void open_date_list();
	void set_check_status(bool v);
	void add_new_action();
	void update_page();
private:
	void open_schedules(QString object);
	QString currnet_date	="";
	pqxx::connection *sql_cn= 0;
	QPushButton *b_back		= 0;
	QLabel *label_date		= 0;
	QWidget *p_list_view	= 0;
	QWidget *p_tble_view	= 0;
	bool check_status		= 0;
};
#endif