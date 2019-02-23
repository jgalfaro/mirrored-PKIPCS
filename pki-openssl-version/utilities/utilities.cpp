/*
Télécom SudParis, Télécom Saint-Étienne
2015, February
Pierre-Olivier Rocher
*/

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#include <Rpc.h>
#include <filesystem>
#include <cctype>
#else
#include <dirent.h>
#include <uuid/uuid.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#define iswascii(c) ((unsigned)(c) < 0x80)
#endif

#include "stonepine.utilities.regex.h"

#include "utilities.h"

namespace Stonepine
{
	template<class Function>
	void utilities::iosBaseFailure(Function&& f_fonctionToProtect)
	{
		try
		{
			f_fonctionToProtect();
		}
		catch (std::ios_base::failure& f)
		{
			std::ostringstream oSS;

			oSS << "something got wrong while opening file ->";
			oSS << f.what();

			Stonepine::LOG_ERROR(oSS);
		}
	};

	const bool utilities::isAsciiCompliant(const std::string& fm_data) noexcept
	{
		for (auto &iTD : fm_data)
		{
#ifdef __WINDOWS__
			if (__isascii(iTD) == 0)
#else
			if (isascii(iTD) == 0)
#endif
			{
				return false;
			}
		}

		return true;
	}

	const bool utilities::isWideAsciiCompliant(const std::string& fm_data) noexcept
	{
		for (auto &iTD : fm_data)
		{
			if (iswascii(iTD) == 0)
			{
				return false;
			}
		}

		return true;
	}

	const std::string utilities::removeNonAsciiCharacters(const std::string& fm_data) noexcept
	{
		if (isAsciiCompliant(fm_data) == true)
		{
			return fm_data;
		}

		auto r = fm_data;

		r.erase(remove_if(r.begin(), r.end(), [](char fa_character)
		{
			if (fa_character >= 0 && fa_character < 128)
			{
				return false;
			}

			return true;
		}
		), r.end());

		return r;
	}

	const void utilities::convertToLowerCase(std::string& fm_data) noexcept
	{
		std::transform(fm_data.begin(), fm_data.end(), fm_data.begin(), [](unsigned char c) { return std::tolower(c); });
	}

	const void utilities::convertToUpperCase(std::string& fm_data) noexcept
	{
		std::transform(fm_data.begin(), fm_data.end(), fm_data.begin(), [](unsigned char c) { return std::toupper(c); });
	}

	const bool utilities::isAValidIpV4Address(const std::string& fm_data) noexcept
	{
		return Utilities::regex::isStringConformToRegexExpression(fm_data, "^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	}

	const bool utilities::isAValidIpV6Address(const std::string& fm_data) noexcept
	{
		return Utilities::regex::isStringConformToRegexExpression(fm_data, "^((([0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}:[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){5}:([0-9A-Fa-f]{1,4}:)?[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){4}:([0-9A-Fa-f]{1,4}:){0,2}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){3}:([0-9A-Fa-f]{1,4}:){0,3}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){2}:([0-9A-Fa-f]{1,4}:){0,4}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}((b((25[0-5])|(1d{2})|(2[0-4]d)|(d{1,2}))b).){3}(b((25[0-5])|(1d{2})|(2[0-4]d)|(d{1,2}))b))|(([0-9A-Fa-f]{1,4}:){0,5}:((b((25[0-5])|(1d{2})|(2[0-4]d)|(d{1,2}))b).){3}(b((25[0-5])|(1d{2})|(2[0-4]d)|(d{1,2}))b))|(::([0-9A-Fa-f]{1,4}:){0,5}((b((25[0-5])|(1d{2})|(2[0-4]d)|(d{1,2}))b).){3}(b((25[0-5])|(1d{2})|(2[0-4]d)|(d{1,2}))b))|([0-9A-Fa-f]{1,4}::([0-9A-Fa-f]{1,4}:){0,5}[0-9A-Fa-f]{1,4})|(::([0-9A-Fa-f]{1,4}:){0,6}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){1,7}:))$");
	}

	const bool utilities::isAValidIpAddress(const std::string& fm_data) noexcept
	{
		if (isAValidIpV4Address(fm_data) == true || isAValidIpV6Address(fm_data) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const std::string utilities::convertToLowerCase(const std::string& fm_data) noexcept
	{
		std::string d = fm_data;

		convertToLowerCase(d);

		return d;
	}

	const std::string utilities::convertToUpperCase(const std::string& fm_data) noexcept
	{
		std::string d = fm_data;

		convertToUpperCase(d);

		return d;
	}

	const std::string utilities::interactiveConsolePasswordPrompt(const std::string& fm_messageToAskForPassword) noexcept
	{
		std::cout << fm_messageToAskForPassword << std::endl;
		std::cout << "->";

		std::string p = {};

#ifdef __WINDOWS__
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode = 0;

		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

		std::cin.ignore();
		getline(std::cin, p);

		SetConsoleMode(hStdin, mode);
#else
		termios oldt;
		tcgetattr(STDIN_FILENO, &oldt);
		termios newt = oldt;
		newt.c_lflag &= ~ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);

		getline(std::cin, p);

		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
		return p;
	}

	const std::string utilities::removeAllSubstrings(const std::string& fm_inputString, const std::string& fm_substring) noexcept
	{
		auto r = fm_inputString;

		removeAllSubstrings(r, fm_substring);

		return r;
	}

	const void utilities::removeLastEndLine(std::string& fm_string) noexcept
	{
#if _WIN32
		fm_string.erase(std::remove(fm_string.begin(), fm_string.end(), '\n'), fm_string.end());
#else
#endif
	}

	const void utilities::removeAllSubstrings(std::string& fm_inputString, const std::string& fm_substring) noexcept
	{
		auto i = fm_inputString.find(fm_substring);

		while (i != std::string::npos)
		{
			fm_inputString.erase(i, fm_substring.length());

			i = fm_inputString.find(fm_substring, i);
		}
	}

	std::vector<char> stringToVectorChar(const std::string& fm_data)
	{
		auto r = std::vector<char>(fm_data.begin(), fm_data.end());

		r.push_back('\0');

		return r;
	}

	void sleep(const unsigned int f_milliSeconds)
	{
		auto sleep = true;
		auto start = std::chrono::system_clock::now();

		while (sleep)
		{
			auto now = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

			if (elapsed.count() > f_milliSeconds)
			{
				sleep = false;
			}
		}
	}

	void sleepThisThread(const unsigned int f_milliSeconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(f_milliSeconds));
	}

	std::unique_ptr<char> string2CharUniqueSmartPointer(const std::string& f_aString)
	{
		std::unique_ptr<char> output(new char[(f_aString.size() + 1) * sizeof(char)]);

#ifdef __WINDOWS__
		strcpy_s(output.get(), std::strlen(output.get()), f_aString.c_str());
#else
		strcpy(output.get(), f_aString.c_str());
#endif

		return output;
	}

	std::shared_ptr<char> string2CharSharedSmartPointer(const std::string& f_aString)
	{
		std::shared_ptr<char> output(new char[(f_aString.size() + 1) * sizeof(char)]);

#ifdef __WINDOWS__
		strcpy_s(output.get(), std::strlen(output.get()), f_aString.c_str());
#else
		strcpy(output.get(), f_aString.c_str());
#endif

		return output;
	}

	std::string generateAUuid(void)
	{
#ifdef __WINDOWS__
		UUID uuid;
		UuidCreate(&uuid);

		unsigned char* str;
		UuidToStringA(&uuid, &str);

		std::string s((char*)str);

		RpcStringFreeA(&str);
#else
		uuid_t uuid;
		uuid_generate_random(uuid);
		char s[37];
		uuid_unparse(uuid, s);
#endif
		return s;
	}

	std::string generate16BitsUuid(void)
	{
#ifdef __WINDOWS__
		UUID uuid;
		UuidCreate(&uuid);

		return std::to_string(uuid.Data2);
#else
		return std::string("ToDo");
#endif
	}

	std::unique_ptr<std::string> generateAUuidSp(void)
	{
#ifdef __WINDOWS__
		UUID uuid;
		UuidCreate(&uuid);

		unsigned char* str;
		UuidToStringA(&uuid, &str);

		auto s = std::make_unique<std::string>((char*)str);

		RpcStringFreeA(&str);
#else
		uuid_t uuid;
		uuid_generate_random(uuid);
		char ss[37];
		uuid_unparse(uuid, ss);

		auto s = std::unique_ptr<std::string>(new std::string((char*)ss));
#endif
		return s;
	}

	std::unique_ptr<std::vector<std::string>> getFolderListOfFiles(const std::string& f_folderPath, const std::string& f_regularExpression)
	{
		std::unique_ptr<std::vector<std::string>> fPS = nullptr;

#ifdef __WINDOWS__
		for (auto& iTDI : std::experimental::filesystem::v1::directory_iterator(f_folderPath))
		{
			if (std::experimental::filesystem::v1::is_regular_file(iTDI) == true)
			{
				auto sCP = std::experimental::filesystem::v1::system_complete(iTDI);

				if (Stonepine::Utilities::regex::isStringConformToRegexExpression(sCP.filename().generic_string(), f_regularExpression) == true)
				{
					fPS->push_back(sCP.generic_string());
				}
			}
		}

		return fPS;
#else
		DIR *dir = nullptr;
		struct dirent *ent = nullptr;

		if ((dir = opendir(f_folderPath.c_str())) != nullptr)
		{
			while ((ent = readdir(dir)) != nullptr)
			{
				if (f_regularExpression.size() != 0)
				{
					if (Stonepine::Utilities::regex::isStringConformToRegexExpression(ent->d_name, f_regularExpression) == true)
					{
						fPS->push_back(ent->d_name);
					}
				}
				else
				{
					fPS->push_back(ent->d_name);
			}
		}

			closedir(dir);
	}
		else
		{
			throw std::invalid_argument("unable to open the folder " + f_folderPath);
		}
#endif
		return fPS;
	}

	std::vector<std::string> getSplitedString(const std::string& f_string, const char f_delimiter)
	{
		std::vector<std::string> v;
		std::stringstream sS(f_string);
		std::string tS;

		while (std::getline(sS, tS, f_delimiter))
		{
			v.push_back(tS);
		}

		return v;
	}
	}
 
