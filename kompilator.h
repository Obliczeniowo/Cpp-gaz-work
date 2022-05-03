#ifndef KOMPILATOR_H
#define KOMPILATOR_H

#include <Windows.h>
#include <vector>

#include "myMath.h"

#pragma region Sta³e Oprarators i sectionType

typedef double (_cdecl* fu)(double);

enum Operators : int // Dostêpne operatory
{
	Not				= '!',
	And				= '&',
	Or				= '|',
	Xor				= '^',
	Plus			= '+',
	Minus			= '-',
	Multiplication	= '*',
	Division		= '/',
	Equal			= '=',
	More			= '>',
	Less			= '<',
	MoreOrEqual		= '>' * '=',
	LessOrEqual		= '<' * '=',
	NotEqual		= '!' * '=',
	LeftBracket		= '(',
	RightBracket	= ')',
	Power			= '#',
	None			= -1
};

enum sectionType : int	// typ sekcji
{
	operatorType,
	numberType,
	valueType,
	functionType
};

#pragma endregion

#pragma region Deklaracja klasy Section

class Section{
public:
	enum sectionType	st;
	enum Operators		ot;
	double* value;	// dla zmiennej
	double numericValue; // dla sta³ej (litera³u)
	double Value();
	void Value(double *value);
	fu fn;
	Section(enum sectionType st,enum Operators ot,double* value);
	Section(fu fn);
	~Section();
};

#pragma endregion

#pragma region Deklaracja klasy mVarible

class mVarible{
public:
	std::string varibleName;
	double value;
	mVarible(LPSTR varibleName, double value);
	~mVarible();
};

#pragma endregion

#pragma region Deklaracja klasy mFunction

class mFunction{
public:
	mFunction(std::string fName,fu fn);

	void SetFunction(std::string fName,fu fn);

	std::string fName;
	fu fn;
};

#pragma endregion

#pragma region Deklaracja klasy Kompilator

class Kompilator{
	std::string str;
	bool CheckBracket();
	bool CheckOperatorsChilds();
	std::vector <mVarible>	varible;
	std::vector <mFunction> functions;
	std::vector <Section>	Sections;
	// Funkcje sprawdzaj¹ce
	bool			IsNumeric(std::string num);
	bool			HaveOperatorInName(std::string varible);
	bool			CalcRest(std::vector<Section> &temp);
	bool			CreateSections();
	// Funkcja zwracaj¹ca typ operatora
	enum Operators	OperatorType(int index);
	double*			ItIsVarible(std::string Varible);	// Zwraca adres do zmiennej typu double
	fu				ItIsFunction(std::string text);
	int				FirstOperatorIndex(int BeginIndex);
public:
	Kompilator(LPSTR str);
	~Kompilator();
	void	Str(LPSTR str);
	bool	AddVarible(LPSTR varibleName,double value);
	//int		NrOfVaribles();
	double	Calculate();
	double*	VaribleAddress(LPSTR varibleName);
};

#pragma endregion

#endif