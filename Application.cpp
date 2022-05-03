/* Application - zawiera zbiór funnkcji zwi¹zanych z dzia³aniem programu
   Autorem jest Krzysztof Zaj¹czkowski 
   Wersja: 1.0 */

#include "Application.h"

void Application::StartupFile(std::string &startupPath,std::string startupFileName){
	StartupPath(startupPath);
	startupPath += startupFileName;
}

void Application::StartupPath(std::string &startupPath){
	char *path	= GetCommandLine();
	char *end	= NULL;
	char temp2 = ' ';
	if(path[0] == 34){
		path++;
		end = strchr(path, '"');
		if(!end)
			return ;
		temp2 = *end;
	}else{
		end = strchr(path, ' ');
		if(!end){
			end = path + strlen(path) - 1;
		}
		temp2 = *end;
	}
	*end = NULL;
	char *p		= strrchr(path,'\\');
	p ++;
	char temp = *p;
	*p			= NULL;
	startupPath = path;
	*p = temp;
	*end = temp2;
}

void Application::GetFileFromCmdLine(LPSTR CmdLine,std::vector<std::string> &fileTable){
	char *end;
	char *begin = strlen(CmdLine) ? CmdLine + 1 : NULL;
	while(begin){
		if(*(begin - 1) == '"'){
			end = strchr(begin,'"');
			if(end){
				*end = NULL;
				std::string s = begin;
				fileTable.push_back(s);
				*end = '"';
				if(strlen(end) > 1){
					begin = end + 3;
				}else{
					begin = NULL;
				}
			}else{
				begin = NULL;
			}
		}else{
			end = strchr(begin, ' ');
			if(end){
				*end = NULL;
				std::string s = begin - 1;
				fileTable.push_back(s);
				*end = ' ';
				if(strlen(end) > 1){
					begin = end + 2;
				}else{
					begin = NULL;
				}
			}else if(strlen(begin)){
				std::string s = begin - 1;
				fileTable.push_back(s);
				begin = NULL;
			}else{
				begin = NULL;
			}
		}
	}
}

void Application::GetFileFromCmdLine(LPSTR CmdLine, std::vector<std::string> &fileTable, std::vector<std::string> &filter){
	strlwr(CmdLine);
	GetFileFromCmdLine(CmdLine,fileTable);

	if(fileTable.size()){
		for(int i = 0; i < (int) fileTable.size(); i++){
			bool w = false; // Jak false to usuñ z listy
			for(std::vector<std::string>::iterator j = filter.begin(); j < filter.end(); j++){
				if(fileTable[i].size() > j->size()){
					std::string temp = fileTable[i].c_str() + fileTable[i].size() - j->size();
					if(temp.compare(*j) == 0){
						w = true;
						j = filter.end() - 1;
					}
				}
			}
			if(!w){
				fileTable.erase(fileTable.begin() + i);
				i --;
			}
		}
	}
}