#include "SQL_TABLES.h"

//! Все SQL таблицы
//				column_name		human_column_name	column_type		link_to_table	primaly_key	max		is_nullable
QList<TableElement> SQLTable_Patient{
	TableElement("patient_id",			"Номер Пациента",		Int,	NULL_T,		true),
	TableElement("medcard_number",		"Номер медкарты",		Int,	NULL_T,		false, 12, true),
	TableElement("place_of_work",		"Место работы",			String, NULL_T,		false, 80, true),
	TableElement("human_id",			"Номер Человека",		Int,	Human,		false, 0, false)
};

QList<TableElement> SQLTable_Doctor{
	TableElement("doctor_id",			"Номер Доктора",		Int,	NULL_T,		true),
	TableElement("office",				"Кабинет",				String, NULL_T,		false, 4, true),
	TableElement("on_vacation",			"В отпуске",			Bool),
	TableElement("on_work",				"На работе",			Bool),
	TableElement("human_id",			"Номер Человека",		Int,	Human,		false, 0, true)
};

QList<TableElement> SQLTable_Service{
	TableElement("service_id", 			"Номер",				Int,	NULL_T,		true),
	TableElement("service_name",		"Название услуги",		String, NULL_T,		false, 64),
	TableElement("doctor_id", 			"Доктор",				Int,	Doctor,		false, 0 , true),//EditorWindow::CheckAllFields
	TableElement("price", 				"Цена",					Int,	NULL_T,		false, 0 , true),
	TableElement("time_begin", 			"Вермя начала приёма",	Time, 	NULL_T,		false, 0 , true),
	TableElement("time_end", 			"Вермя конца приёма",	Time,	NULL_T,		false, 0 , true)
};

QList<TableElement> SQLTable_Contract{
	TableElement("contract_id",			"Номер",				Int,	NULL_T,		true),
	TableElement("contract_title",		"Заголовок Контракта",	String,	NULL_T,		false, 80, true),
	TableElement("customer",			"Заказчик",				Int,	Patient),
	TableElement("executor",			"Исполнитель",			Int,	Doctor),
	TableElement("date_of_create",		"Дата создания",		Date)
};

QList<TableElement> SQLTable_MedDocs{
	TableElement("meddoc_id",			"УИ",					Int,	NULL_T,		true),
	TableElement("title",				"Заголовок мед. док.",	String,	NULL_T,		false,	64),
	TableElement("doctor_ID",			"Доктор",				Int,	Doctor),
	TableElement("receiver",			"Получатель",			Int,	Patient),
	TableElement("date_of_ready",		"Дата создания",		Date),
	TableElement("vlid_util",			"Годен до",				Date),
	TableElement("date_of_give",		"Дата выдачи",			Date,	NULL_T,		false, 0, true),
};

QList<TableElement> SQLTable_Schedules{
	TableElement("action_id",			"Номер",				Int,	NULL_T,		true),
	TableElement("service_id",			"Услуга",				Int,	Service,	false), //EditorWindow::CheckAllFields
	TableElement("patient_id",			"Пациент",				Int,	Patient,	false),
	TableElement("action_date",			"Дата и время",			Timestamp),
	TableElement("status",				"Завершённость",		Bool)
};

QList<TableElement> SQLTable_Passport{
	TableElement("passport_number",		 "Номер Паспорта",		StrInt, NULL_T,		false, 6),
	TableElement("passport_seriec",		 "Серия Паспорта",		StrInt, NULL_T,		false, 4),
	TableElement("issue_by",			 "Кем Выдан",			Text),
	TableElement("date_of_issue",		 "Дата Выдачи",			Date),
	TableElement("place_of_regestration","Место рег. ",			Text),
	TableElement("place_of_birth",		 "Место рож.",			Text),
	TableElement("date_of_birth",		 "Дата рож.",			Date)
};

QList<TableElement> SQLTable_Human{
	TableElement("human_id",			"Номер",				Int,	NULL_T, 	true),
	TableElement("human_name",			"Имя",					String, NULL_T, 	false, 20),
	TableElement("human_surname",		"Фамилия",				String, NULL_T, 	false, 24),
	TableElement("human_patronymic",	"Отчество",				String, NULL_T, 	false, 30),
	TableElement("human_gender",		"Пол",					Char),
	TableElement("posport_number",		"Номер Паспорта",		StrInt,	Passport,false, 6, true),
	TableElement("phone_number",		"Номер Телефона",		Telephone, NULL_T,	false, 16)
};


QList<TableElement>& get_sql_table(short id){
	switch(id){
	case 0:	return SQLTable_Patient;
	case 1:	return SQLTable_Doctor;
	case 2:	return SQLTable_Service;
	case 3:	return SQLTable_Contract;
	case 4:	return SQLTable_MedDocs;
	case 5:	return SQLTable_Schedules;
	case 6:	return SQLTable_Human;
	case 7:	return SQLTable_Passport;
	}
}
QList<TableElement>& get_sql_table(SQLTable t){
	switch(t){
	case Passport:	return SQLTable_Passport;
	case Human:		return SQLTable_Human;
	case Patient:	return SQLTable_Patient;
	case Doctor:	return SQLTable_Doctor;
	case Service:	return SQLTable_Service;
	case Contract:	return SQLTable_Contract;
	case MedDoc:	return SQLTable_MedDocs;
	case Schedules:	return SQLTable_Schedules;
	}
}
