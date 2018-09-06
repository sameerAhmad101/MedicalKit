#pragma once
#pragma warning (push, 0)

#pragma warning (pop)
#include <string>
#include <array>
#include <vector>

namespace service {
	class Patient;
}

class Patient {
public:
    Patient(std::string &firstName, std::string &lastName, int age, std::string &location,  std::string &gender, std::string &nationalNumber, std::string &disease, std::string &personalImage, std::vector<std::string> &images_v, std::string &imageSeriesDir);
	~Patient();
	std::string getFirstName();
	std::string getLastName();
	std::string getLocation();
	std::string getprofilePath();
	std::string getGender();
	std::string getNationalNumber();
    std::string getPersonalImage();
    std::string getDisease();

	int getAge();
	void setIamgeSeriesDir(std::string &dir);
	std::vector<std::string>  getImages();
	void  setIamges(std::vector<std::string> &images);
	std::string getImagesPath();

private:
	std::string m_id;
	std::string m_firstName;
	std::string m_lastName;
	std::string m_location;
	std::string m_profilePath;
	int m_age;
	std::string m_gender;
	std::string m_nationalNumber;
    std::vector<std::string> m_images_v;
	std::string m_imageSeriesDir;
    std::string m_personalImage;
    std::string m_disease;
};
