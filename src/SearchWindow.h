#ifndef H_SearchW
#define H_SearchW
#include "InputField.h"
#include "Table.h"
#include <QWidget>
#include <QCloseEvent>
#include <pqxx/pqxx>
class QComboBox;
class QCheckBox;
class QListView;
class QLineEdit;
class SQLTableView;

class SearchWindow: public QWidget{
Q_OBJECT
public:
	SearchWindow(pqxx::connection *sql_cn, QLineEdit *le=nullptr, short tbl_id=0, short cln_id=0);
	~SearchWindow();
public slots:
	void CheckAllFields();
	void StartSearch();
	void closeEvent (QCloseEvent *event);
	void change_table(int index);
	void change_column(int index);
	void set_value_to_le(QString);
private:
	pqxx::connection *sql_cn;
	QComboBox *cb_table = 0;
	QComboBox *cb_column = 0;
	QCheckBox *chb_inverted = 0;
	QListView *lv_result = 0;
	InputField* input_field = 0;
	QLineEdit *le = nullptr;
	SQLTableView *tv;
};

#endif
