#ifndef H_IF
#define H_IF
#include <QWidget>
#include "Table.h"
class QLineEdit;
class QCheckBox;
class QDateTimeEdit;
class QDate; class QButtonGroup;
class InputField: public QWidget{
Q_OBJECT
public:
	InputField(TableElement te, bool full_prevel=false);
	~InputField();
	void SetIndicator(bool v);
	void SetEnabled(bool v);
	bool CheckInput();

	QString get_value(bool full=true);
	QDate get_date();
	void set_value(QString);
	void set_value(QDate);
	void change_table_element(TableElement te, bool full_prevel=false);
	QLineEdit *get_line_edit();
public slots:
	void emit_signal_finished();
	void telephone_is_change(QString new_text);
signals:
	void edit_finished(QString);
private:
	void create_ui(TableElement te, bool full_prevel=false);
	QLineEdit		*le = 0;
	QCheckBox		*cb = 0;
	QDateTimeEdit	*dte= 0;
	QButtonGroup	*btg= 0;
	DataType dt;
	bool is_nullable;

};
#endif
