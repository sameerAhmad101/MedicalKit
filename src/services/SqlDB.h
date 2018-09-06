#pragma once
#pragma warning(push, 0)
#include <QtSql>
#include <QSqlDatabase>
#include <QString>
#pragma warning(pop)
#include "Patient.h"


namespace  service {
	class Database;
}

class Database {
public:
	Database();
	~Database();
	bool connectToDb(QString &dbName);
	bool addTables();

    bool addPatient(Patient *patient);
	Patient* getPatient(std::string &nationalNumber);
	std::map<std::string, Patient*> getPatients();
private:
	QSqlDatabase m_database;
	QString m_dbName;
	Patient * m_patient;
	std::map<std::string, Patient*> m_patients_v;
};
