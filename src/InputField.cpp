#include "InputField.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QValidator>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
InputField::InputField(TableElement te, bool full_prevel):
dt(te.column_type), is_nullable(te.is_nullable){
	QHBoxLayout *layout = new QHBoxLayout(this);
	create_ui(te, full_prevel);
}

InputField::~InputField(){
	QLayoutItem *li = 0;
	while ((li = layout()->takeAt(0)) != nullptr){
		delete li->widget();
		delete li;
	}
	delete layout();
}

void InputField::change_table_element(TableElement te, bool full_prevel){
	dt = te.column_type;
	is_nullable = te.is_nullable;

	QLayoutItem *li = 0;
	while ((li = layout()->takeAt(0)) != nullptr){
		delete li->widget();
		delete li;
	}

	create_ui(te, full_prevel);
}

void InputField::create_ui(TableElement te, bool full_prevel){
	le = 0; cb = 0; dte = 0;
	layout() -> addWidget(new QLabel(te.human_column_name));

	if(dt==Bool){
		cb = new QCheckBox();
		layout() -> addWidget(cb);
	}
	else if(dt==Time){
		dte = new QTimeEdit();
		layout() -> addWidget(dte);
	}
	else if(dt==Date){
		dte = new QDateEdit();
		layout() -> addWidget(dte);
	}
	else if(dt==Timestamp){
		dte = new QDateTimeEdit();
		layout() -> addWidget(dte);
	}
	else{
		le = new QLineEdit();
		le->setEnabled(!te.primaly_key || full_prevel);
		if(dt==String) {
			le -> setMaxLength(te.max_char_count);
			layout() -> addWidget(le);
		}
		else if(dt==Char){
			le -> setMaxLength(1);
			layout() -> addWidget(le);
		}
		else if(dt==Text) layout() -> addWidget(le);
		else{
			QValidator *validator = 0;
			if (dt==Int){
				validator = new QIntValidator(0, 2147483647, this);
				le -> setValidator(validator);
				layout() -> addWidget(le);
			}
			else if (dt==StrInt){
				QRegularExpression rx(QString("^\\d{0,%1}$").arg(te.max_char_count));
				validator = new QRegularExpressionValidator(rx, this);
				le -> setValidator(validator);
				layout() -> addWidget(le);
			}
			else if (dt==Telephone){// \\+? - символ + может быть, а может не быть
				QRegularExpression rx("^\\+?7?\\(?\\d{0,3}?\\)?\\d{0,3}?\\-?\\d{0,2}\\-?\\d{0,2}$");
				validator = new QRegularExpressionValidator(rx, this);
				le -> setValidator(validator);
				layout() -> addWidget(le);
				connect(le, &QLineEdit::textEdited, this, &InputField::telephone_is_change);
			}
			
		}
	}
}

void InputField::telephone_is_change(QString new_text){
	bool is_change = false;
	int old_curs_pos = le->cursorPosition();
	QRegularExpression re1("\\+7\\(\\d{3}");
	QRegularExpression re2("\\+7\\(\\d{3}\\)\\d{3}");
	QRegularExpression re3("\\+7\\(\\d{3}\\)\\d{3}\\-\\d{2}");
	// QRegularExpressionMatch match = re.match("abc123 def");
	// bool hasMatch = match.hasMatch(); // true
	if(new_text=="+" || new_text=="("){
		new_text = "+7(";
		is_change = true;
	}
	else if (new_text=="7"){
		new_text = "+7(7";
		is_change = true;
	}
	else if (new_text!="7" && new_text.size()==1){
		new_text = "+7(" + new_text;
		is_change = true;
	}
	else{
		if(re1.match(new_text).hasMatch() && new_text.size() > 6 && new_text[6]!=')'){
			new_text.insert(6,")");
			is_change = true;
		}
		else if(re2.match(new_text).hasMatch() && new_text.size() > 10 && new_text[10]!='-'){
			new_text.insert(10,"-");
			is_change = true;
		}
		else if(re3.match(new_text).hasMatch() && new_text.size() > 13 && new_text[13]!='-'){
			new_text.insert(13,"-");
			is_change = true;
		}
	}
	// else if(re.match(new_text).hasMatch() && new_text.size() == 6) new_text += ")";
	
	
	if (is_change) le->setText(new_text);
}


bool InputField::CheckInput(){

	if(le==0) return true; 
	QString my_text = le->text();
	
	if (dt==Telephone){
		QRegularExpression re("^\\+7\\(\\d{3}\\)\\d{3}\\-\\d{2}\\-\\d{2}$");
		return re.match(my_text).hasMatch();
	}
	else if(dt==StrInt){
		return (my_text.size() == le ->maxLength());
	}
	return is_nullable || !my_text.isEmpty() || !(le->isEnabled());
}

void InputField::SetIndicator(bool v){
	QWidget *w = layout() -> itemAt(0) -> widget();
	if(v)	w->setStyleSheet("color: black");
	else	w->setStyleSheet("color: red");
}
void InputField::SetEnabled(bool v){
	layout() -> itemAt(1) -> widget() -> setEnabled(v);
}

QString InputField::get_value(){
	if(le!=0){
		if(!(le->isEnabled())) return "default";
		else{
			QString text = le->text();

			return (!text.isEmpty())? '\''+(le->text())+'\'' : "";
		}
	}
	else if (cb!=0){
		if(cb->isChecked()) return "'true'";
		else return "'false'";
	}
	else if(dte!=0){
		QString text = "";
		if(dt==Date) text = dte->date().toString("yyyy/MM/dd");

		else if(dt==Time) text = dte->time().toString("hh:mm:ss");

		else if(dt==Timestamp) text = dte->dateTime().toString("yyyy/MM/dd hh:mm:ss");

		return '\''+text+'\'';
	}
	return "";
}
void InputField::set_value(QString v){
	if(le!=0){
	    le->setText(v);
	}
	else if (cb!=0){
		if(v=="t") cb->setChecked(true);
		else cb->setChecked(false);
	}
	else if(dte!=0){
		qDebug()<<v;
		if(dt==Date){
			QRegularExpression re("^(?<year>\\d{4})\\-(?<month>\\d{2})\\-(?<date>\\d{2})$");
			QRegularExpressionMatch m = re.match(v);
			qDebug() << m.captured("year") << m.captured("month") << m.captured("date");
			QDate d(m.captured("year").toInt(), m.captured("month").toInt(), m.captured("date").toInt());
			dte->setDate(d);
		} 
		else if(dt==Time){
			QRegularExpression re("^(\\d{2})\\:(\\d{2})\\:(\\d{2})$");
			QRegularExpressionMatch m = re.match(v);
			QTime t(m.captured(1).toInt(), m.captured(2).toInt(), m.captured(3).toInt());
			dte->setTime(t);
		} 
		else if(dt==Timestamp){
			QRegularExpression re("^(?<year>\\d{4})\\-(?<month>\\d{2})\\-(?<date>\\d{2}) (\\d{2})\\:(\\d{2})\\:(\\d{2})$");
			QRegularExpressionMatch m = re.match(v);
			QDate d(m.captured("year").toInt(), m.captured("month").toInt(), m.captured("date").toInt());
			QTime t(m.captured(4).toInt(), m.captured(5).toInt(), m.captured(6).toInt());
			dte->setDateTime(QDateTime(d,t));
		} 
	}
}

QLineEdit *InputField::get_line_edit(){
	return le;
}
