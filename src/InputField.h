#ifndef H_IF
#define H_IF
#include <QWidget>
#include "Table.h"
class QLineEdit;
class QCheckBox;

class InputField: public QWidget{
Q_OBJECT
public:
	InputField(QString name, DataType dt, SQLTable link_t = NULL_T,bool is_editable = true);
	~InputField();
	void SetIndicator(bool v);
	bool CheckInput();
	QString GetValue();
	QLineEdit* getLineEdit();
	QCheckBox* getCheckBox();
	DataType getDataType();
public slots:
	void telephone_is_change(QString new_text);
	void time_is_change(QString new_text);
	void date_is_change(QString new_text);
private:
	QLineEdit *le = 0;
	QCheckBox *cb = 0;
	DataType dt;

};
#endif