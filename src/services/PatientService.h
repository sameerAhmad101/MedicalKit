#pragma once
#pragma warning (push,0)
#include <vector>
#include <string>
#include <array>
#include <qstring.h> 
#include  <qbytearray.h>
#include <qobject.h>
#include <qstring.h>
#include <codecvt>
#include <experimental/filesystem>
#include <qlist.h>
#pragma warning (pop)
#include "SqlDB.h"
#include "Patient.h"
namespace service
{
	class  PatientService;
}

class PatientService
{
public:
    PatientService(std::shared_ptr<Database> db);
    ~PatientService();
	std::map<std::string, Patient*> getPatients();
    bool addPatient(std::string &firstName, std::string &lastName, int age, std::string &location,
                    std::string &gender, std::string &nationalNumber, std::string &disease,
                    std::string &personalImage, std::vector<std::string> &images_v, std::string &imageSeriesDir);
	bool initPatients();
private:
	Patient * m_patient;
	std::map<std::string, Patient*> m_patients_v;
	std::shared_ptr<Database> m_database;
};

