/*
Télécom SudParis
2017, Pierre-Olivier Rocher
*/

#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>

namespace fs = std::experimental::filesystem::v1;

int main(void)
{
	std::ofstream m_outputFile;

	std::vector<std::string> vE = { ".mp4" , ".wmv" , ".mov" , ".rm" ,".ram" ,".mpg" ,".m4v" , ".mpeg", ".avi", ".flv", ".m4v" };

	m_outputFile.open("transcode.bat", std::ios::out | std::ios::app);

	for (auto& iTRD : fs::recursive_directory_iterator("."))
	{
		if (fs::is_regular_file(iTRD) == true)
		{
			auto fP = fs::system_complete(iTRD);
			auto nFP = fP;

			if (std::find(vE.begin(), vE.end(), fP.extension()) != vE.end())
			{
				if (m_outputFile.is_open() == true)
				{
					m_outputFile << "ffmpeg -i \"" << fP << "\" -c:v libx265 -preset medium -qscale 0 -x265-params crf=20 \"" << nFP.replace_extension(".mkv") << "\"" << std::endl;
				}
			}
		}
	}

	if (m_outputFile.is_open() == true)
	{
		m_outputFile.close();
	}

	return EXIT_SUCCESS;
}
