#ifndef H_STV
#define H_STV
#include <QTableView>
#include "Table.h"
#include <QStandardItemModel>
#include <QList>
#include <QModelIndex>
#include <pqxx/pqxx>

class SQLTableView: public QTableView{
	Q_OBJECT
public:
	SQLTableView(short tbl_id, pqxx::connection *sql_cn, bool editable = true, const QList<SQLFilter> &filters = {});
	~SQLTableView();
	QStandardItemModel* create_model_for_table(short tbl_id, const QList<SQLFilter> &filters = {});
	void update_f( const QList<SQLFilter> &filters);
	unsigned short get_sql_tbl_id();
public slots:
	void edit_row(const QModelIndex &index);
	void change_model(short tbl_id, const QList<SQLFilter> &filters = {});
	void update();
	void add_row();
	void dc_on_row(const QModelIndex &index);
	void remove_closed_ew();
signals:
void itemIdChanged(QString);
private:
	pqxx::connection *sql_cn;
	QWidget *editor_wnd	= 0;
	QList<SQLFilter> filters = {};
	short sql_tbl_id;
};
#endif