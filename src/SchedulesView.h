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
private:
	void open_schedules(QString object);
	pqxx::connection *sql_cn= 0;
	QPushButton *b_back		= 0;
	QLabel *label_date		= 0;
	QWidget *p_list_view	= 0;
	QWidget *p_tble_view	= 0;
};
#endif