#include "MiniginPCH.h"
#include "BinaryReader.h"


BinaryReader::BinaryReader()
{
}


BinaryReader::~BinaryReader()
{
}

std::string BinaryReader::ReadString(int size, const char* fileName)
{
	std::string returnString{};
	m_File.open(fileName, std::ios::in | std::ios::binary);
	if (m_File.is_open())
	{
		m_File.seekg(m_CurrentPos);
		m_File.read((char *)&size, sizeof(size));
		returnString.resize(size);
		m_File.read(&returnString[0], size);
		m_CurrentPos = m_File.tellg();
		m_File.close();
	}
	return returnString;
}
