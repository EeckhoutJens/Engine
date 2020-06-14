#include "MiniginPCH.h"
#include "BinaryWriter.h"


BinaryWriter::BinaryWriter()
{
}

BinaryWriter::~BinaryWriter()
{
	m_File.close();
}

void BinaryWriter::WriteString(std::string& str, const char* fileName)
{
	m_File.open(fileName, std::ios::out | std::ios::binary | std::ios::app);
	if (m_File.is_open())
	{
		m_File.write(str.c_str(), str.size());
		m_File.close();
	}
}
