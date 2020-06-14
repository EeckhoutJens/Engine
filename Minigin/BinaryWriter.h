#pragma once
#include <fstream>
#include <string>
class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();
	template <typename T>
	void Write(T& obj, const char* fileName);
	void WriteString(std::string& str, const char* fileName);
private:
	std::ofstream m_File;

};

template<typename T>
inline void BinaryWriter::Write(T & obj, const char* fileName)
{
	m_File.open(fileName, std::ios::out | std::ios::binary | std::ios::app);
	if (m_File.is_open())
	{
		m_File.write((const char *)&obj, sizeof(obj));
		m_File.close();
	}
}
