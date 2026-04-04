#include "SQLTableView.h"
#include "EditorWindow.h"
#include "VIEW_COL_OF_TABLE.h"
#include "SQL_Manager.h"
SQLTableView::SQLTableView(short tbl_id, pqxx::connection *sql_cn, bool editable, const QList<SQLFilter> &filters):
QTableView(), sql_tbl_id(tbl_id), sql_cn(sql_cn){
	setModel(create_model_for_table(tbl_id, filters));
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	if(editable) connect(this, &QTableView::doubleClicked, this, &SQLTableView::edit_row);
	connect(this, &QTableView::doubleClicked, this, &SQLTableView::dc_on_row);
}

SQLTableView::~SQLTableView(){
}

QStandardItemModel* SQLTableView::create_model_for_table(short tbl_id, const QList<SQLFilter> &filters){
	SQLTable tbl = static_cast<SQLTable>(tbl_id);
	QList<TableElement> tbl_obj = get_h_table(tbl);
	QStandardItemModel* model =  new QStandardItemModel(0, tbl_obj.size());
	for (short i=0; i<tbl_obj.size(); i++){
		model->setHeaderData(i, Qt::Horizontal, tbl_obj[i].human_column_name);
	}
	QList<QStringList> data{};
	switch(tbl){
	case Patient:	get_data_from_sql_tbl(data, Patient,	sql_cn, filters);	break;
	case Schedules:	get_data_from_sql_tbl(data, Schedules,	sql_cn, filters);	break;
	case MedDoc:	get_data_from_sql_tbl(data, MedDoc,		sql_cn, filters);	break;
	case Service:	get_data_from_sql_tbl(data, Service,	sql_cn, filters);	break;
	case Doctor:	get_data_from_sql_tbl(data, Doctor,		sql_cn, filters);	break;
	case Contract:	get_data_from_sql_tbl(data, Contract,	sql_cn, filters);	break;
	case Passport:	get_data_from_sql_tbl(data, Passport, 	sql_cn, filters);	break;
	case Human:		get_data_from_sql_tbl(data, Human,		sql_cn, filters);	break;
	}
	for(size_t r=0; r<data.size(); r++){
		for(size_t c=0; c<data[r].size(); c++){
			if(tbl_obj[c].column_type==Bool){
				if(data[r][c]=="t") model->setItem(r,c, new QStandardItem("True"));
				else model->setItem(r,c, new QStandardItem(""));
			}
			else model->setItem(r,c, new QStandardItem(data[r][c]));
		}
	}
	return model;
}

void SQLTableView::change_model(short tbl_id, const QList<SQLFilter> &filters){
	sql_tbl_id = tbl_id;
	setModel(create_model_for_table(sql_tbl_id, filters));
}

void SQLTableView::update(){
	setModel(create_model_for_table(sql_tbl_id));
}

void SQLTableView::edit_row(const QModelIndex &index){
	QModelIndex mIndex = model()->index(index.row(), 0);
	QString object_id = (mIndex.data()).toString();
	EditorWindow *ew = new EditorWindow(static_cast<SQLTable>(sql_tbl_id), false, sql_cn, object_id);
    //all_ew_points.push_back(ew);
	connect(ew, &EditorWindow::data_is_changed, this, &SQLTableView::update);
	//connect(ew, SIGNAL(destroyed(QObject*)), this, SLOT(remove_closed_ew(QObject*)));
}



void SQLTableView::add_row(){
	EditorWindow *ew = new EditorWindow(static_cast<SQLTable>(sql_tbl_id), true, sql_cn);
}

void SQLTableView::dc_on_row(const QModelIndex &index){
	QModelIndex mIndex = model()->index(index.row(), 0);
	QString object_id = (mIndex.data()).toString();
	emit itemIdChanged(object_id);
}