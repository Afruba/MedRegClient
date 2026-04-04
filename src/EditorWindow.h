#ifndef H_EW
#define H_EW
#include "EditField.h"
#include <pqxx/pqxx>
#include <QWidget>
#include <QCloseEvent>
#include <QList>



class EditorWindow: public QWidget{
Q_OBJECT
public:
	EditorWindow(SQLTable t, bool add_mode, pqxx::connection *sql_cn, QString id="");
	~EditorWindow();
public slots:
	void CheckAllFields();
	void closeEvent (QCloseEvent *event);
	void delete_data();
signals:
	void data_is_changed();
private:
	void AddToDB();
	void ApplyChange();
	
	pqxx::connection *sql_cn;
	QString object_id;
	SQLTable sql_tbl;
	QList<EditField*> all_fields;
	bool add_mode;
};

#endif
