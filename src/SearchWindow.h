#ifndef H_SW
#define H_SW
#include "InputField.h"
#include "Table.h"
#include <QWidget>
#include <QCloseEvent>
#include <QList>

class SearchWindow: public QWidget{
Q_OBJECT
public:
	SearchWindow(QList<TableElement> t, QString target_name = "таблице");
	~SearchWindow();
public slots:
	void CheckAllFields();
	void StartSearch();
	void closeEvent (QCloseEvent *event);
private:
	QList<InputField*> all_fields;
};

#endif
