#ifndef H_MW
#define H_MW
#include <QMainWindow>
#include "Table.h"
#include <QList>
#include <pqxx/pqxx>
#include <QCloseEvent>
class QStandardItemModel;
class PasswordDialog;
class SQLTableView;
class MainWindow: public QMainWindow{
Q_OBJECT;
public:
	MainWindow(pqxx::connection *sql_cn);
	~MainWindow();
	void CreateUI();
	void CreateMenu();
public slots:
	void closeEvent (QCloseEvent *event);
	void open_main_menu();
	void open_table(short i);
	void search();
	void update_page();
	void add_to_cur_table();
	void add_to_oth_table(short id);
	void add_new_patient();
	void exit_from_acc();
	void quit_from_app();
	void get_answer_from_pd(bool status);
	void change_in_main_widget(int index);
	void open_schedules();
	void remove_closed_ew(QObject *object);
	void remove_search_window();
	
private:
	QWidget* CreateTableWidget(short tbl_id);
	QStandardItemModel* create_model_for_table(short tbl_id);

	QList<TableElement>& get_human_table(short id);
	QList<TableElement>& get_current_table();
	SQLTableView* get_current_table_view();
	void delete_CW();
	QWidget *centrall_widget		= 0;
	bool menu_is_create				= false;
	//QList<TableElement* > *CurrnetTable = 0;
	short current_table				= 0;
	PasswordDialog *password_widget	= 0;
	pqxx::connection *sql_cn;
	QList<QObject*> all_ew_points;
	QWidget *search_wnd				= 0;
};
#endif
