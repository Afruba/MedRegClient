#ifndef H_BEW
#define H_BEW
#include "EditField.h"
#include "Table.h"
#include <pqxx/pqxx>
#include <QWidget>
#include <QCloseEvent>
#include <QList>


class BigEditorWindow: public QWidget{
	Q_OBJECT
public:
	BigEditorWindow(SQLTable t, bool add_mode, pqxx::connection *sql_cn, QString id=0);
	~BigEditorWindow();
public slots:
	void CheckAllFields();
	void closeEvent (QCloseEvent *event);
	void disable_passport(bool state);
signals:
	void data_is_changed();
private:
	void add_edit_column(SQLTable t, QString id="");
	void AddToDB();
	void ApplyChange();

	pqxx::connection *sql_cn;
	QStringList object_id;
	SQLTable sql_tbl;
	QList<InputField*> all_fields;
	bool add_mode;
	bool use_passport;
};

#endif
