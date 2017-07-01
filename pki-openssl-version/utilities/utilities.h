/*
Télécom SudParis, Télécom Saint-Étienne
2015, February
Pierre-Olivier Rocher
*/

#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <map>
#include <vector>
#include <regex>

#include <stonepine/logger/logger.h>

//#ifdef _WIN32
//#include <sysinfoapi.h>
//#endif

#if _WIN32
#ifdef UTILITIES_EXPORTS
#define UTILITIES_API __declspec(dllexport)
#else
#define UTILITIES_API __declspec(dllimport)
#endif
#else
#define UTILITIES_API
#endif

namespace Stonepine
{
	class utilities final
	{

	private:

		template <class Function>
		static void iosBaseFailure(Function&& f_fonctionToProtect);

		static inline void xToString(std::ostream&) noexcept {}

		template<class T, class... R>
		static void xToString(std::ostream& fm_ostream, const T& fm_currentValue, const R&... fm_otherValuesToProcess) noexcept;

	public:

		template <class... T>
		static const std::string concatenate(T... fm_valuesToConcatenate) noexcept;

		UTILITIES_API static const bool isAsciiCompliant(const std::string& fm_data) noexcept;
		UTILITIES_API static const bool isWideAsciiCompliant(const std::string& fm_data) noexcept;

		UTILITIES_API static const std::string removeNonAsciiCharacters(const std::string& fm_data) noexcept;

		UTILITIES_API static const void convertToLowerCase(std::string& fm_data) noexcept;
		UTILITIES_API static const void convertToUpperCase(std::string& fm_data) noexcept;

		UTILITIES_API static const std::string convertToLowerCase(const std::string& fm_data) noexcept;
		UTILITIES_API static const std::string convertToUpperCase(const std::string& fm_data) noexcept;

		UTILITIES_API static const bool isAValidIpV4Address(const std::string& fm_data) noexcept;
		UTILITIES_API static const bool isAValidIpV6Address(const std::string& fm_data) noexcept;

		UTILITIES_API static const bool isAValidIpAddress(const std::string& fm_data) noexcept;

		// Intractive concole password prompt
		UTILITIES_API static const std::string interactiveConsolePasswordPrompt(const std::string& fm_messageToAskForPassword) noexcept;

		UTILITIES_API static const std::string removeAllSubstrings(const std::string& fm_inputString, const std::string& fm_substring) noexcept;
		UTILITIES_API static const void removeAllSubstrings(std::string& fm_inputString, const std::string& fm_substring) noexcept;

		template<typename T>
		const void removeAllSubstrings(std::basic_string<T>& fm_inputString, const std::basic_string<T>& fm_substring) noexcept;

		template<typename T, typename U>
		static const std::vector<T> extractFirstElements(const std::map<T, U>& fm_data) noexcept;


		UTILITIES_API static const void removeLastEndLine(std::string& fm_string) noexcept;

	};

	template<class T, class... R>
	inline void utilities::xToString(std::ostream& fm_ostream, const T& fm_currentValue, const R&... fm_otherValuesToProcess) noexcept
	{
		fm_ostream << fm_currentValue;

		utilities::xToString(fm_ostream, fm_otherValuesToProcess...);
	}

	template <class... T>
	inline const std::string utilities::concatenate(T... fm_valuesToConcatenate) noexcept
	{
		std::ostringstream oSS;

		utilities::xToString(oSS, fm_valuesToConcatenate...);

		return oSS.str();
	}

	template<typename T>
	inline const void utilities::removeAllSubstrings(std::basic_string<T>& fm_inputString, const std::basic_string<T>& fm_substring) noexcept
	{
		auto i = fm_inputString.find(fm_substring);

		while (i != std::basic_string<T>::npos)
		{
			fm_inputString.erase(i, fm_substring.length());

			i = fm_inputString.find(fm_substring, i);
		}
	}

	template<typename T, typename U>
	inline const std::vector<T> utilities::extractFirstElements(const std::map<T, U>& fm_data) noexcept
	{
		auto r = std::vector<T>();

		for (auto &iTD : fm_data)
		{
			r.emplace_back(iTD.first);
		}

		return r;
	}

	UTILITIES_API std::vector<char> stringToVectorChar(const std::string& fm_data);

	

	UTILITIES_API void sleep(const unsigned int f_milliSeconds);
	UTILITIES_API void sleepThisThread(const unsigned int f_milliSeconds);

	UTILITIES_API std::unique_ptr<char> string2CharUniqueSmartPointer(const std::string& f_aString);
	UTILITIES_API std::shared_ptr<char> string2CharSharedSmartPointer(const std::string& f_aString);

	// Time retaled functions
	/*UTILITIES_API std::ostream& operator<<(std::ostream& f_stream, const std::tm* f_tm);
	UTILITIES_API std::ostream& operator<<(std::ostream& f_stream, std::unique_ptr<std::tm> f_tm);
	UTILITIES_API const std::tm getLocalTime(void);
	UTILITIES_API const std::shared_ptr<time_t> localTimeAsTimeT(void);
	UTILITIES_API const std::shared_ptr<time_t> utcTimeAsTimeT(void);
	UTILITIES_API const std::tm getUtcTime(void);
	UTILITIES_API const time_t getUtcTime2(void);
	UTILITIES_API const std::string getUtcTime3(void);
	UTILITIES_API std::unique_ptr<std::tm> getLocalTimeSp(void);
	UTILITIES_API time_t tmToTime(std::tm& f_tm);
	UTILITIES_API std::unique_ptr<time_t> tmToTime(std::unique_ptr<std::tm> f_tm);
	UTILITIES_API std::string timeToString(const time_t& fm_time);
	UTILITIES_API std::unique_ptr<std::tm> stringToTm(const std::string& f_time, const std::string& f_pattern);
	UTILITIES_API std::unique_ptr<time_t> stringToTime(const std::string& f_time, const std::string& f_pattern);*/

	UTILITIES_API std::string generateAUuid(void);
	UTILITIES_API std::unique_ptr<std::string> generateAUuidSp(void);

	UTILITIES_API std::string generate16BitsUuid(void);

	// Folder/file related
	UTILITIES_API std::unique_ptr<std::vector<std::string>> getFolderListOfFiles(const std::string& f_folderPath, const std::string& f_regularExpression);

	// String stuff
	UTILITIES_API std::vector<std::string> getSplitedString(const std::string& f_string, const char f_delimiter);
}
