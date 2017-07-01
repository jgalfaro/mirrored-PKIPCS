/*
2016, February
Pierre-Olivier Rocher
*/

#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <memory>

#include "bsonParser.h"

// Get the file size
int getFileSize(const std::string& f_filePath) noexcept(false)
{
	// From https://stackoverflow.com/questions/2409504/using-c-filestreams-fstream-how-can-you-determine-the-size-of-a-file

	auto s = 0;
	std::ifstream f;

	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		f.open(f_filePath.c_str(), std::ios::binary | std::ios::ate);

		s = static_cast<int>(f.tellg());
	}
	catch (std::ifstream::failure& e)
	{
		Stonepine::logger::error("unable to open or to read the file -> " + std::string(e.what()));

		std::exit(EXIT_SUCCESS);
	}

	f.close();

	return s;
}

int main(int argc, char **argv)
{
	std::string filePath;

	if (argc == 2)
	{
		filePath = argv[1];
	}
	else
	{
		Stonepine::logger::info("usage : bsonParser data.bson");

		return EXIT_SUCCESS;
	}

	auto fileSize = getFileSize(filePath);

	std::vector<char> fileData;
	fileData.reserve(fileSize);

	// If the file isn'e empty, read it
	if (fileSize == 0)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		// From https://stackoverflow.com/questions/4761529/efficient-way-of-reading-a-file-into-an-stdvectorchar

		std::ifstream f;

		f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			f.open(filePath.c_str(), std::ios::binary);

			fileData.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
		}
		catch (std::ifstream::failure& e)
		{
			Stonepine::logger::error("unable to open or to read the file -> " + std::string(e.what()));

			std::exit(EXIT_SUCCESS);
		}

		f.close();
	}

	std::chrono::time_point<std::chrono::system_clock> bT, eT;

	bT = std::chrono::system_clock::now();

	// Create a BSON parser, parse and print key/value pairs
	auto bP = std::make_unique<Stonepine::Parsers::bson>(fileData);

	bP->parseData();

	eT = std::chrono::system_clock::now();

	std::chrono::duration<double> duration = eT - bT;

	Stonepine::logger::info("parsing time: " + std::to_string(duration.count()));

	bP->print();

	// Get internal structure
	auto bD = bP->getBSONDocuments();

	// Access all read ID elements
	for (auto &iTBD : bD)
	{
		for (auto &jTBD : *iTBD.second)
		{
			Stonepine::Parsers::Bson::x07Token* x07T = dynamic_cast<Stonepine::Parsers::Bson::x07Token*>(jTBD.get());

			if (x07T != nullptr)
			{
				Stonepine::logger::info("BSON documents IDs : " + x07T->getObjectIdValue());
			}

			x07T = nullptr;
		}
	}

	return EXIT_SUCCESS;
}
