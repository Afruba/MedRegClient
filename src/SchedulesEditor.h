#ifndef SCHEDULES_E_H
#define SCHEDULES_E_H
#include "EditField.h"
#include <pqxx/pqxx>
#include <QWidget>
#include <QCloseEvent>
#include <QList>

class QLabel; class QGridLayout; class QPushButton;
class SchedulesEditor: public QWidget{
	Q_OBJECT
public:
	SchedulesEditor(bool add_mode, pqxx::connection *sql_cn, QString default_date = "", QString id="");
	~SchedulesEditor();
public slots:
	void CheckAllFields();
	void closeEvent (QCloseEvent *event);
	void delete_data();
	void check_service(QString);
	void select_time(QString t);
	void print_ticket(bool v=false);
signals:
	void data_is_changed();
private:
	void AddToDB();
	void ApplyChange();
	pqxx::connection *sql_cn;
	QString object_id;
	QString selected_time	= "";
	QList<EditField*> all_fields;
	bool add_mode;
	bool print_late_ticket	= false;
	bool doctor_no_vac		= true;
	QLabel *l_doctor		= 0;
	QLabel *l_time			= 0;
	QLabel *l_stime			= 0;
	QGridLayout *gl_times	= 0;
	QPushButton *bt_apply	= 0;
};
#endif