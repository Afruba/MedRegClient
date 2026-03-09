#ifndef H_EW
#define H_EW
#include "InputField.h"
// #include "EditField.h"
#include <QWidget>
#include <QCloseEvent>
#include <QList>
class MainWindow;

class EditField: public InputField{
public:
	EditField(QString name, DataType dt, SQLTable link_t = NULL_T, bool is_editable = true);
	~EditField();
	QString getValue();
	void setValue(QString);
}; 

class EditorWindow: public QWidget{
Q_OBJECT
public:
	EditorWindow(QList<TableElement> t ,MainWindow *mw, bool add_mode, QStringList data = {}, int row_pos=0);
	~EditorWindow();
public slots:
	void CheckAllFields();
	void ApplyChange();
	void closeEvent (QCloseEvent *event);
private:
	int row_pos;
	MainWindow *mw;
	QList<EditField*> all_fields;
	bool add_mode;
};

#endif
