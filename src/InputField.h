#ifndef H_IF
#define H_IF
#include <QWidget>
#include "Table.h"
class QLineEdit;
class QCheckBox;
class QDateTimeEdit;
class InputField: public QWidget{
Q_OBJECT
public:
	InputField(TableElement te, bool full_prevel=false);
	~InputField();
	void SetIndicator(bool v);
	void SetEnabled(bool v);
	bool CheckInput();

	QString get_value();
	void set_value(QString);
	void change_table_element(TableElement te, bool full_prevel=false);
	QLineEdit *get_line_edit();
public slots:
	void telephone_is_change(QString new_text);
private:
	void create_ui(TableElement te, bool full_prevel=false);
	QLineEdit		*le = 0;
	QCheckBox		*cb = 0;
	QDateTimeEdit	*dte = 0;
	DataType dt;
	bool is_nullable;

};
#endif
