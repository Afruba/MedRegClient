#ifndef TABLE_HOLDER_H
#define TABLE_HOLDER_H
#include <QWidget>
#include <pqxx/pqxx>
#include "Table.h"
class InputField; class QComboBox; class SQLTableView;
class SQLTableHolder: public QWidget{
	Q_OBJECT
public:
	SQLTableHolder(short tbl_id, pqxx::connection *sql_cn, bool editable = true, const QList<SQLFilter> &filters = {});
	~SQLTableHolder();
public slots:
void update();
void set_exactly(bool v);
void set_inverted(bool v);
void setup_filter();
void change_column(int index);
private:
	SQLTableView *tv;
	InputField *input_field = 0;
	QComboBox *cb_column	= 0;
	bool exaclty			= true;
	bool inverted			= false;
};
#endif