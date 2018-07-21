#pragma once

#pragma warning (push,0)
#include <array>
#include <map>
#pragma warning (pop,0)

namespace settings {
	class RawFileSettings;
	class MedicalKitSettings;
}

class MedicalKitSettings{
public:
	MedicalKitSettings();
	~MedicalKitSettings();
	class RawFileSettings {

	public:
		RawFileSettings();
		~RawFileSettings();
		static std::map<std::int16_t, std::string> m_RawFileTypes;
	private:
	protected:
	};

};
