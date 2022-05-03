#include "kompilator.h"

LPSTR error = "B³¹d";

//#define SHOW_COMPILATOR_ERROR_MESSAGE
//#define SHOW_SECTION_ERROR_MESSAGE

double Silnia(double i){
	double s = 1;
	int copyI = int(floor(i));
	if(copyI <= 0)
		return 0;
	for(int j = 1; j <= copyI; j++){
		s *= j;
	}
	return s;
}

#pragma region Klasa Section

Section::Section(enum sectionType st,enum Operators ot,double *value){
	this->st = st;
	this->ot = ot;
	this->Value(value);
}
Section::Section(fu fn){
	st = functionType;
	ot = None;
	this->fn = fn;
}
Section::~Section(){
}
double Section::Value(){
	if(st == numberType)
		return numericValue;
	if(st == valueType)
		return  *value;
#ifdef SHOW_SECTION_ERROR_MESSAGE
	MessageBox(NULL,"sectionType::operatorType w Section::Value()\nten b³¹d mo¿e powodowaæ wyst¹pienie powa¿niejszego b³êdu kompilacji",error,MB_OK|MB_ICONWARNING);
#endif
	return 0;
}
void Section::Value(double* value){
	if(st == numberType){
		numericValue = *value;
		this->value = NULL;
	}else if(st == valueType){
		this->value = value;
		numericValue = 0;
	}
#ifdef SHOW_SECTION_ERROR_MESSAGE
	else if(st != operatorType){
		MessageBox(NULL,"sectionType::operatorType w Section::Value(double *value)\nten b³¹d mo¿e powodowaæ wyst¹pienie powa¿niejszego b³êdu kompilacji",error,MB_OK|MB_ICONWARNING);
	}
#endif
}

#pragma endregion

#pragma region Klasa mVarible

mVarible::mVarible(LPSTR varibleName,double value){
	this->varibleName = varibleName;
	this->value = value;
}
mVarible::~mVarible(){
}

#pragma endregion

#pragma region Klasa mFunction

mFunction::mFunction(std::string fName,fu fn){
	SetFunction(fName,fn);
}

void mFunction::SetFunction(std::string fName,fu fn){
	this->fName = fName;
	this->fn = fn;
}

#pragma endregion

#pragma region Klasa Kompilator

Kompilator::Kompilator(LPSTR str){
	mFunction f((std::string)"sin",sin);
	functions.push_back(f);
	f.SetFunction((std::string)"cos",cos);
	functions.push_back(f);
	f.SetFunction((std::string)"tan",tan);
	functions.push_back(f);
	f.SetFunction((std::string)"sinh",sinh);
	functions.push_back(f);
	f.SetFunction((std::string)"cosh",cosh);
	functions.push_back(f);
	f.SetFunction((std::string)"tanh",tanh);
	functions.push_back(f);
	f.SetFunction((std::string)"asin",asin);
	functions.push_back(f);
	f.SetFunction((std::string)"acos",acos);
	functions.push_back(f);
	f.SetFunction((std::string)"atan",atan);
	functions.push_back(f);
	f.SetFunction((std::string)"abs",abs);
	functions.push_back(f);
	f.SetFunction((std::string)"floor",floor);
	functions.push_back(f);
	f.SetFunction((std::string)"ceil",ceil);
	functions.push_back(f);
	f.SetFunction((std::string)"ln",log);
	functions.push_back(f);
	f.SetFunction((std::string)"log10",log10);
	functions.push_back(f);
	f.SetFunction((std::string)"exp",exp);
	functions.push_back(f);
	f.SetFunction((std::string)"rad",myMath::DegreesToRadians);
	functions.push_back(f);
	f.SetFunction((std::string)"deg",myMath::RadiansToDegrees);
	functions.push_back(f);
	f.SetFunction((std::string)"sqrt",sqrt);
	functions.push_back(f);
	f.SetFunction((std::string)"silnia",Silnia);
	functions.push_back(f);

	AddVarible("pi",M_PI);

	Str(str);
}
Kompilator::~Kompilator(){
	str.clear();
}
void Kompilator::Str(LPSTR str){
	this->str = str;
	for(std::string::size_type i = 0; i < this->str.size(); i++){
		if(this->str.at(i) == ','){
			this->str.at(i) = '.';
		}else if(this->str.at(i) == ' '){
			this->str.erase(this->str.begin() + i);
		}
	}
	CreateSections();
}
bool Kompilator::CheckOperatorsChilds(){
	for(std::string::size_type i = 0; i < Sections.size(); i++){
		if(Sections.at(i).st == operatorType){
			switch(Sections.at(i).ot){
				case Xor:
				case Or:
				case And:
					{
						if(i == 0)
							return false;
						if(i + 1 < Sections.size()){
							if(
								!Sections.at(i + 1).ot == Not ||
								!Sections.at(i + 1).ot == Plus ||
								!Sections.at(i + 1).ot == Minus ||
								!Sections.at(i + 1).ot == LeftBracket ||
								!Sections.at(i + 1).ot == None)
							{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Prawy potomek operatorów ^, |, &",error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}else{
							return false;
						}
						if(i > 0){
							if(
								!Sections.at(i + 1).ot == RightBracket &&
								!Sections.at(i + 1).ot == None)
							{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Prawy potomek operatorów ^, |, &",error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}
					}
					break;
				case LessOrEqual:
				case Less:
				case MoreOrEqual:
				case NotEqual:
				case More:
				case Equal:
					{
						if(i == 0)
							return false;
						if(i + 1 < Sections.size()){
							if(
								!Sections.at(i + 1).ot == Not &&
								!Sections.at(i + 1).ot == Plus &&
								!Sections.at(i + 1).ot == Minus &&
								!Sections.at(i + 1).ot == LeftBracket &&
								!Sections.at(i + 1).ot == None ||
								Sections.at(i + 1).st == functionType)
							{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Lewy potomek operatorów <=, <, >=, !=, >, ==",error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}else{
							return false;
						}
						if(i > 0){
							if(
								!Sections.at(i - 1).ot == RightBracket &&
								!Sections.at(i - 1).ot == None)
							{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Prawy potomek operatorów <=, <, >=, !=, >, ==",error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}
					}
					break;
				case Division:
				case Multiplication:
				case Power:
					{
						if(i == 0)
							return false;
						if(i + 1 < Sections.size()){
							if(
								!Sections.at(i + 1).ot == Plus &&
								!Sections.at(i + 1).ot == Minus &&
								!Sections.at(i + 1).ot == LeftBracket &&
								!Sections.at(i + 1).ot == None ||
								!Sections.at(i + 1).st == functionType){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Lewy potomek operatorów /, *",error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}
						if(i > 0){
							if(
								!Sections.at(i - 1).ot == RightBracket &&
								!Sections.at(i - 1).ot == None)
							{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Prawy potomek operatorów /, *",error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}
					}
					break;
				case Minus:
				case Plus:
					{
						if(i + 1 < Sections.size()){
							if(	Sections.at(i + 1).ot == LeftBracket &&
								!Sections.at(i + 1).ot == None ||
								!Sections.at(i + 1).st == functionType){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Lewy potomek operatorów +, -", error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}
						if(i > 0){
							if(
								!Sections.at(i - 1).ot == Plus && 
								!Sections.at(i - 1).ot == Minus)
							{
								MessageBox(NULL,"Prawy potomek operatorów +, -", error,MB_OK|MB_ICONERROR);
								return false;
							}
						}
					}
					break;
				case LeftBracket:
					if(i + 1 < Sections.size()){
						if(
							!(Sections.at(i + 1).ot == Plus) &&
							!(Sections.at(i + 1).ot == Minus) &&
							!(Sections.at(i + 1).ot == None) &&
							!(Sections.at(i + 1).ot == LeftBracket))
						{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
							MessageBox(NULL,"Lewy potomek operatora (",error,MB_OK|MB_ICONERROR);
#endif
							return false;
						}
					}
					if(i > 0){
						if(
							!Sections.at(i - 1).ot == LeftBracket ||
							Sections.at(i - 1).ot == RightBracket ||
							//Sections.at(i - 1).ot == LeftBracket ||
							(Sections.at(i - 1).ot == None && 
							Sections.at(i - 1).st != functionType))
						{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
							MessageBox(NULL,"Prawy potomek operatora (",error,MB_OK|MB_ICONERROR);
#endif
							return false;
						}
					}
					break;
				case RightBracket:
					if(i == 0)
						return false;
					if(i + 1 < Sections.size()){
						if(
							!Sections.at(i + 1).ot == RightBracket ||
							Sections.at(i + 1).ot == LeftBracket ||
							Sections.at(i + 1).ot == None || 
							Sections.at(i + 1).st == functionType)
						{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
							MessageBox(NULL,"Lewy potomek )",error,MB_OK|MB_ICONERROR);
#endif
							return false;
						}
					}
					if(i > 0){
						if(!Sections.at(i - 1).ot == None){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
							MessageBox(NULL,"Prawy potomek )",error,MB_OK|MB_ICONERROR);
#endif
							return false;
						}
					}
					break;
				case Not:
					{
						if(i + 1 < Sections.size()){
							if(
								Sections.at(i + 1).ot == LeftBracket ||
								!Sections.at(i + 1).ot && Not ||
								!Sections.at(i + 1).ot && Plus ||
								!Sections.at(i + 1).ot && Minus ||
								!Sections.at(i + 1).ot && RightBracket ||
								(!Sections.at(i + 1).ot && None &&
								!Sections.at(i + 1).st == functionType)){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Lewy potomek !",error,MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}else{
							return false;
						}
						if(i > 0){
							if(
								Sections.at(i - 1).ot == Not ||
								Sections.at(i - 1).ot == Plus ||
								Sections.at(i - 1).ot == Minus ||
								Sections.at(i - 1).ot == Multiplication ||
								Sections.at(i - 1).ot == Division ||
								Sections.at(i - 1).ot == LeftBracket){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
								MessageBox(NULL,"Prawy potomek !","Error",MB_OK|MB_ICONERROR);
#endif
								return false;
							}
						}
					}
					break;

				case None:
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
					MessageBox(NULL,"Operator nie mo¿e byæ typu None\ngdy SectionType jest równe OperatorType",error,MB_OK|MB_ICONERROR);
#endif
					return false;
			}
		}
	}
	return true;
}
bool Kompilator::CheckBracket(){
	int nrOfLeftBracket = 0;
	int nrOfRightBracket = 0;
	for(std::string::size_type i = 0; i < str.size(); i++){
		if(str.at(i) == '('){
			nrOfLeftBracket ++;
		}else if(str.at(i) == ')'){
			nrOfRightBracket ++;
			if(nrOfLeftBracket < nrOfRightBracket){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
				MessageBox(NULL,"Nawias zamykaj¹cy bez nawiasu otwieraj¹cego",error,MB_OK|MB_ICONERROR);
#endif
				return false;
			}
		}
	}
	if(nrOfLeftBracket == nrOfRightBracket)
		return true;
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
	if(nrOfLeftBracket > nrOfRightBracket)
		MessageBox(NULL,"W funkcji CheckBracket() wykryto b³¹d:\nniepoprawna liczba nawiasów otwieraj¹cych",error,MB_OK|MB_ICONERROR);
	else
		MessageBox(NULL,"W funkcji CheckBracket() wykryto b³¹d:\nniepoprawna liczba nawiasów zamykaj¹cych",error,MB_OK|MB_ICONERROR);
#endif
	return false;
}
bool Kompilator::AddVarible(LPSTR varibleName,double value){
	mVarible tempVarible(varibleName, value);
	if(IsNumeric(tempVarible.varibleName)){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
		MessageBox(NULL,"Zmienna nie mo¿e mieæ nazwy liczbowej",error,MB_OK|MB_ICONERROR);
#endif
		return false;
	}
	if(HaveOperatorInName(tempVarible.varibleName)){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
		MessageBox(NULL,"Zmienna nie mo¿e mieæ w nazwie operatora",error,MB_OK|MB_ICONERROR);
#endif
		return false;
	}
	for(std::string::size_type i = 0; i < this->varible.size(); i++){
		if(!tempVarible.varibleName.compare(this->varible.at(i).varibleName)){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
			MessageBox(NULL,"Zmienna o tej nazwie ju¿ istnieje",error,MB_OK|MB_ICONERROR);
#endif
			return false;
		}
	}
	this->varible.push_back(tempVarible);
	return true;
}
bool Kompilator::IsNumeric(std::string num){
	bool eExist = false;
	bool dotExist = false;
	for(std::string::size_type i = 0; i < num.size(); i++){
		switch((int)num.at(i)){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case '.':
				if(!dotExist){
					dotExist = true;
				}else{
					return false;
				}
				break;
			case 'e':
				if(i == num.size() - 1 || eExist){	// e mo¿e wyst¹piæ tylko raz i w odleg³oœci conajmniej 1-go znaku od koñca
					return false;
				}else if(num.size() - 2 >= i){	// minus mo¿e wyst¹piæ po e tylko w odleg³oœci conajmniej 2-ch znaków od koñca np 1e-2 - poprawny 1e- - b³êdny
					if(num.at(i + 1) == '-')
						i++;
				}
				eExist = true;
				break;
			case '-':	// Gdy minus na pocz¹tku
				if(i != 0)
					return false;
				break;
			default:
				return false;
		}
	}
	return true;
}
bool Kompilator::HaveOperatorInName(std::string varible){
	if(varible.find_first_of("!") != varible.npos || 
		varible.find_first_of("|") != varible.npos ||
		varible.find_first_of("&") != varible.npos ||
		varible.find_first_of("+") != varible.npos ||
		varible.find_first_of("-") != varible.npos ||
		varible.find_first_of("*") != varible.npos ||
		varible.find_first_of("/") != varible.npos ||
		varible.find_first_of("<") != varible.npos ||
		varible.find_first_of("=") != varible.npos ||
		varible.find_first_of(">") != varible.npos ||
		varible.find_first_of("(") != varible.npos ||
		varible.find_first_of(")") != varible.npos)
		return true;
	return false;
}
bool Kompilator::CreateSections(){	// Tworzenie sekcji
	Sections.clear();
	if(!this->CheckBracket())
		return false;
	if(!str.size())
		return false;

	int startIndex = 0;
	int lastIndex = 0;
	while(lastIndex != str.size()){
		lastIndex = FirstOperatorIndex(startIndex);
		if(lastIndex == -1){
			lastIndex = (int) str.size();
		}
		double* value;

		std::string tempStr;
		tempStr = str.c_str();
		if(lastIndex != startIndex){
			const char *lpc = str.c_str();
			tempStr = (lpc + startIndex);
			tempStr.resize(lastIndex - startIndex);
			if(value = ItIsVarible(tempStr)){
				Section s(valueType, None, value);
				this->Sections.push_back(s);
			}else{
				fu fn = ItIsFunction(tempStr);
				if(fn){
					Section s(fn);
					Sections.push_back(s);
				}else if(IsNumeric(tempStr)){
					double w = atof(tempStr.c_str());
					value = &w;
					Section s(numberType,None,value);
					Sections.push_back(s);
				}else{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
					MessageBox(NULL,"Nieznana wartoœæ nieliczbowa","ERROR",MB_OK|MB_ICONERROR);
#endif
					Sections.clear();
					return false;
				}
			}
		}else{
			enum Operators ot = OperatorType(startIndex);
			if(ot != None){
				Section s(operatorType,OperatorType(startIndex),NULL);
				Sections.push_back(s);
				if((int)ot > 256)
					lastIndex += 2;
				else
					lastIndex ++;
			}else{
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
				MessageBox(NULL,"Nieznany b³¹d","ERROR",MB_OK|MB_ICONERROR);
#endif
				Sections.clear();
				return false;
			}
		}
		startIndex = lastIndex;
	}
	if(!this->CheckOperatorsChilds()){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
		MessageBox(NULL, "B³¹d operatora","B³¹d",MB_OK|MB_ICONERROR);
#endif
		Sections.clear();
		return false;
	}
	return true;
}
enum Operators Kompilator::OperatorType(int index){
	if(index >= (int)str.size()){
#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
		MessageBox(NULL,"Index poza rozmiarem tablicy w funkcji OperatorType",error,MB_OK|MB_ICONERROR);
#endif
		return None;
	}
	switch((int)str.at(index)){
		case And:
		case Division:
		case LeftBracket:
		case Minus:
		case Multiplication:
		case Or:
		case Plus:
		case RightBracket:
		case Xor:
		case Power:
			return (Operators)str.at(index);
		case Equal:
			if(index + 1 < (int) str.size())
				return Equal;
			else{
				int temp = (int)str.at(index) * (int)str.at(index + 1);
				switch(temp)
				{
					case LessOrEqual:
					case MoreOrEqual:
					case NotEqual:
						return (Operators)temp;
				}
			}
			break;
		case Less:
			{
				if(index + 1 >= str.size()){
					return Less;
				}
				int temp = (int)str.at(index) * (int)str.at(index + 1);
				if((Operators)temp == LessOrEqual)
				{
					return LessOrEqual;
				}else if((Operators)str.at(index) == Less){
					return Less;
				}else{
					return None;
				}
			}
			break;
		case More:
			{
				if(index + 1 >= str.size()){
					return More;
				}
				int temp = (int)str.at(index) * (int)str.at(index + 1);
				if((Operators)temp == MoreOrEqual)
				{
					return MoreOrEqual;
				}else if((Operators)str.at(index) == More){
					return More;
				}else{
					return None;
				}
			}
			break;
		case Not:
			{
				if(index + 1 >= str.size()){
					return Not;
				}
				int temp = (int)str.at(index) * (int)str.at(index + 1);
				if((Operators)temp==NotEqual)
				{
					return NotEqual;
				}else if((Operators)str.at(index) == Not){
					return Not;
				}else{
					return None;
				}
			}
			break;
	}
	return None;
}
double* Kompilator::ItIsVarible(std::string Varible){
	if(!this->varible.size())
		return NULL;
	for(std::vector<mVarible>::iterator i = varible.begin(); i < varible.end(); i++){
		if(!Varible.compare(i->varibleName)){
			return &i->value;
		}
	}
	return NULL;
}
fu Kompilator::ItIsFunction(std::string text){
	for(std::vector<mFunction>::iterator i = this->functions.begin(); i < functions.end(); i++){
		if(!i->fName.compare(text)){
			return i->fn;
		}
	}
	return NULL;
}
int Kompilator::FirstOperatorIndex(int BeginIndex){
	for(int i = BeginIndex; i < (int)str.size(); i++){
		switch((int)str.at(i)){
			case And:
			case Division:
			case Equal:
			case Less:
			case More:
			case Multiplication:
			case Not:
			case Or:
			case Plus:
			case Xor:
			case LeftBracket:
			case RightBracket:
			case Power:
				return (int) i;
			case Minus:
				//if(i - 1 > 0 && str.size() > 0){
				if(i > 0){
					if(str.at(i - 1) == 'e'){
						if((i - 1) > 0){
							if((str.at(i - 2) < '0' || str.at(i - 2) > '9') && str.at(i - 2) != '.'){
								return i;
							}
						}
					}else if(str[i - 1] == '('){
						//if(i + 1 < str.size() && (str[i + 1] < '0' || str[i + 1] > '9')){
						//	return i;
						//}else{
						if(i + 1 < str.size()){
							if(str[i + 1] < '0' || str[i + 1] > '9'){
								return i;
							}
						}
					}else{
						return i;
					}
						//}
					//}
				}else if(i == 0){
					if(i + 1 < str.size()){
						if(str[i + 1] < '0' || str[i + 1] > '9'){
							return i;
						}
					}
				}else{
					return i;
				}
				//}
		}
	}
	return -1;
}
double Kompilator::Calculate(){
	if(!Sections.size()){
//#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
//		MessageBox(NULL,"Brak danych do przeliczenia","ERROR",MB_OK|MB_ICONERROR);
//#endif
		return 0;
	}
	std::vector<Section> temp;
	for(std::string::size_type i = 0; i < Sections.size(); i++){
		temp.push_back(Sections[i]);
	}
	if(!this->CalcRest(temp)){
		return 0;
	}
	return temp[0].Value();
}
bool Kompilator::CalcRest(std::vector<Section> &temp){
	int lIndex;
	int rIndex;
	do{ // nawiasy (wyliczanie)
		lIndex = -1;
		rIndex = -1;	
		int count = 0;
		for(std::string::size_type i = 0; i < temp.size(); i++){
			if(temp[i].ot == LeftBracket){
				if(count == 0)
					lIndex = (int) i;
				count ++;
			}else if(temp[i].ot == RightBracket){
				if(count == 1){
					rIndex = (int) i;
					i = temp.size();
				}
				count --;
				if(count < 0){
	#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
					MessageBox(NULL,"Problem z nawiasami",error,MB_OK|MB_ICONERROR);
	#endif
					return false;
				}
			}
		}
		if(lIndex != -1 && rIndex != -1){
			std::vector<Section> rTemp;
			for(int i = lIndex + 1; i < rIndex; i++){
				rTemp.push_back(temp[i]);
			}
			if(!CalcRest(rTemp))
				return false;
			temp.erase(temp.begin() + lIndex,temp.begin() + rIndex + 1);
			temp.insert(temp.begin() + lIndex, rTemp[0]);
		}
	}while(lIndex != -1 && rIndex != -1);
	bool serch;
	do{ // funkcje
		serch = false;
		if(temp.size() < 2)
			break;
		for(std::string::size_type i = 0;i < temp.size() - 1;i++){
			if(temp[i].st == functionType){
				double t = temp[i].fn(temp[i + 1].Value());
				temp[i].ot = None;
				temp[i].st = numberType;
				temp[i].Value(&t);
				temp.erase(temp.begin() + i + 1);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);

	do{ // potêgowanie
		serch = false;
		if(temp.size() < 3)
			break;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == Power){
				double t = pow(temp[i - 1].Value(),temp[i + 1].Value());
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);

	do{ // mno¿enie
		serch = false;
		if(temp.size() < 3)
			break;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == Multiplication){
				double t = temp[i - 1].Value() * temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);
	do{ // dzielenie
		if(temp.size() < 3)
			break;
		serch = false;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == Division){
				//if(temp[i + 1].Value()){
					double t = temp[i - 1].Value() / temp[i + 1].Value();
					if(temp[i - 1].st == valueType){
						temp[i - 1].st = numberType;
					}
					temp[i - 1].Value(&t);
					temp.erase(temp.begin() + i,temp.begin() + i  + 2);
					if(temp.size() == 1)
						return true;
					serch = true;
//				}else{
//#ifdef SHOW_COMPILATOR_ERROR_MESSAGE
//					MessageBox(NULL,"Rozdzielon przez zero",error,MB_OK|MB_ICONERROR);
//#endif
//
//					return false;
//				}
			}
		}
	}while(serch);
	do{ // dodawanie
		serch = false;
		if(temp[0].ot == Plus){
			temp.erase(temp.begin());
		}else if(temp[0].ot == Minus){
			if(temp[1].st == valueType || temp[1].st == numberType){
				double t = -temp[1].Value();
				temp[1].st = numberType;
				temp[1].Value(&t);
			}
			temp.erase(temp.begin());
		}
		if(temp.size() < 3)
			break;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == Plus){
				double t = temp[i - 1].Value() + temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}else if(temp[i].ot == Minus){
				double t = temp[i - 1].Value() - temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);
	do{ // odejmowanie
		if(temp.size() < 3)
			break;
		serch = false;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == More){
				double t = temp[i - 1].Value() > temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}else if(temp[i].ot == MoreOrEqual){
				double t = temp[i - 1].Value() || temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}else if(temp[i].ot == Less){
				double t = temp[i - 1].Value() < temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}else if(temp[i].ot == LessOrEqual){
				double t = temp[i - 1].Value() <= temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}else if(temp[i].ot == NotEqual){
				double t = temp[i - 1].Value() != temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}else if(temp[i].ot == Equal){
				double t = temp[i - 1].Value() == temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);
	do{ // zaprzeczenie
		serch = false;
		for(std::string::size_type i = 0;i < temp.size() - 1;i++){
			if(temp[i].ot == Not){
				double t = !temp[i + 1].Value();
				temp[i].st = numberType;
				temp[i].Value(&t);
				temp.erase(temp.begin() + i + 1);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);
	do{ // oraz
		if(temp.size() < 3)
			break;
		serch = false;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == And){
				double t = temp[i - 1].Value() && temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);
	do{ // lub
		if(temp.size() < 3)
			break;
		serch = false;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == Or){
				double t = temp[i - 1].Value() || temp[i + 1].Value();
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);
	do{ // xor
		if(temp.size() < 3)
			break;
		serch = false;
		for(std::string::size_type i = 1;i < temp.size() - 1;i++){
			if(temp[i].ot == Xor){
				double t = (int)(temp[i - 1].Value()) ^ (int)(temp[i + 1].Value());
				if(temp[i - 1].st == valueType){
					temp[i - 1].st = numberType;
				}
				temp[i - 1].Value(&t);
				temp.erase(temp.begin() + i,temp.begin() + i  + 2);
				if(temp.size() == 1)
					return true;
				serch = true;
			}
		}
	}while(serch);
	return true;
}
double* Kompilator::VaribleAddress(LPSTR varibleName){
	std::string temp = varibleName;
	return ItIsVarible(temp);
}

#pragma endregion