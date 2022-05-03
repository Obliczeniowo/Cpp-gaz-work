/* Application - zawiera zbi�r funnkcji zwi�zanych z dzia�aniem programu
   Autorem jest Krzysztof Zaj�czkowski 
   Wersja: 1.0 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <Windows.h>

#include <string>
#include <vector>

class Application
{
public:
	static void StartupFile(std::string &startupPath,std::string startupFileName);
	static void StartupPath(std::string &startupPath);
	static void GetFileFromCmdLine(LPSTR CmdLine,std::vector<std::string> &fileTable);
	static void GetFileFromCmdLine(LPSTR CmdLine,std::vector<std::string> &fileTable,std::vector<std::string> &filter);
};

#endif