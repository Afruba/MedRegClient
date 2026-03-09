#ifndef H_MW
#define H_MW
#include <QMainWindow>
#include "Table.h"
#include <QList>

class PasswordDialog;
class QTableView;
class MainWindow: public QMainWindow{
Q_OBJECT;
public:
	MainWindow();
	~MainWindow();
	void CreateUI();
	void CreateMenu();
	QWidget* CreateTableWidget(QList <TableElement> &t);
	QList<TableElement>& get_current_table();
public slots:
	void open_main_menu();
	void open_service_list();
	void open_doctor_list();
	void search();
	void update_page();
	void add_to_cur_table();
	void add_to_oth_table(short id);
	void exit_from_acc();
	void quit_from_app();
	void get_answer_from_pd(bool status);
	void change_in_main_widget(int index);
	void edit_row_in_table(const QModelIndex &index);
	void change_data_in_cur_table(int row_id, QStringList data);
	void add_data_in_table(int tbl_id, QStringList data);
private:
	QTableView* get_current_table_view();

	QWidget *centrall_widget		= 0;
	QWidget *widget_main_menu		= 0;
	QWidget *widget_service_list	= 0;
	QWidget *widget_doctor_list		= 0;
	bool menu_is_create				= false;
	//QList<TableElement* > *CurrnetTable = 0;
	short current_table				= 0;
	PasswordDialog *password_widget	= 0;
};
#endif
