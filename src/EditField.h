#ifndef EF_H
#define EF_H
#include "InputField.h"
#include <QWidget>
#include <pqxx/pqxx>
class EditField: public InputField{
	Q_OBJECT
public:
	EditField(TableElement te, pqxx::connection *sql_cn);
	~EditField();
public slots:
	void open_search_wnd();
	void remove_search_wnd();
private:
	pqxx::connection *sql_cn;
	QWidget *search_wnd_p = nullptr;
	short tbl;
};
#endif
