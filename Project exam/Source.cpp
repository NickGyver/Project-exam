/*Основное задание

Реализовать программу записная книжка с возможностью сохранения записей в файл.

Предоставить пользователю возможности выбора в меню :
1.	Добавить запись
2.	Просмотреть ближайшую по дате запись
3.	Просмотреть все записи
4.	Просмотреть запись / записи по критериям поискового запроса

Пример содержания записи :

#1 28.01.2020 Запись....
#2 29.01.2020 Запись....

Дополнительное задание

Реализовать возможность сохранения каждой записи в отдельном файле с возможностью ее редактирования(изменения заметки).*/



#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <conio.h>
#include <ctime>
using namespace std;



class Date   //не задействован
{
public:
	Date(int d = 0, int m = 0, int y = 0)
	{
		setDate(d, m, y);
	}

	Date(const Date& date)
	{
		setDate(date.day, date.month, date.year);
	}

	~Date()
	{

	}

	int getDay() const
	{
		return day;
	}

	int getMonth() const
	{
		return month;
	}

	int getYear() const
	{
		return year;
	}

	void setDay(int day)
	{
		this->day = day;
	}

	void setMonth(int month)
	{
		this->month = month;
	}

	void setYear(int year)
	{
		this->year = year;
	}

	void setDate(int day, int month, int year)
	{
		if (isValidDate(day, month, year))
		{
			setDay(day);
			setMonth(month);
			setYear(year);
		}
		else {
			throw  string("Not valid date");
		}
	}

	void print() const
	{
		cout << day << '.'
			<< month << '.'
			<< year << endl;
	}

	static bool isValidDate(int day, int month, int year)
	{
		if ((day < 1 || day>31) || (month < 1 || month>12))
		{
			cout << "Not valid date" << endl;
		}
		return true;
	}

	static bool isLeapYear(int year)
	{

		return ((year % 4 == 0 && year % 100 != 0) || year % 400);

	}

	static int dayInMonth(int month)
	{
		int days[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (isLeapYear)days[1] = 29;
		return days[month - 1];
	}

private:
	int day;
	int month;
	int year;
};

class Contact
{
public:
	char fname[30];
	char sname[30];
	char address[150];
	char age[5];
	char phone[15];
};

const int EXT = 5;

void AddContact(Contact*&, int&, int&, Contact&);
void DeleteContact(Contact*& MainPtr, int&, int, int);
void ShowContact(Contact*, int);
void ShowAll(Contact*, int);
void FindContactFromNumber(Contact* arr, int count, char* Number);
void FindContactFromName(Contact* arr, int count, char* Name);
void ReadFile(Contact*& notebook, int& size, int& count, ifstream& ifile);
void WriteFile(Contact* arr, int count, ofstream& ofile);

int main()
{
	int size = 2;
	int count = 2;
	int ContactNumber;

	Contact* MainPtr = new Contact[size];
	MainPtr[0] = Contact{ "Vasily", "Ivanov", "Kriviy Rih", "25", "0123456789" };
	MainPtr[1] = Contact{ "Igor", "Petrov", "Kyiv", "32", "0242426789" };

	Contact n_contact;

	char MenuChoise, Exit;
	//bool sorting;

	do
	{
		do
		{
			system("cls");
			cout << "******** NOTE BOOK ********" << endl << endl;
			cout << "(1) ADD NEW CONTACT" << endl << "(2) EDIT CONTACT" << endl << "(3) DELETE CONTACT" << endl
				<< "(4) SHOW ALL CONTACTS" << endl << "(5) SHOW ONE CONTACT" << endl << "(6) FIND CONTACT" <<
				endl << "(7) SAVE FILE" << endl << "(8) LOAD FILE" << endl << "(9) EXIT" << endl << endl;
			MenuChoise = _getch();
			// cin.ignore();
		} while (MenuChoise < '1' && MenuChoise > '9');

		switch (MenuChoise)
		{
		case '1':
			cout << "Entering new contact..." << endl << endl;
			cout << "Enter first name: " << endl;
			cin.getline(n_contact.fname, 30);
			cout << "Enter second name: " << endl;
			cin.getline(n_contact.sname, 30);
			cout << "Enter address: " << endl;
			cin.getline(n_contact.address, 150);
			cout << "Enter age: " << endl;
			cin.getline(n_contact.age, 5);
			cout << "Enter phone number (10 digits): " << endl;
			cin.getline(n_contact.phone, 15);

			AddContact(MainPtr, size, count, n_contact);
			break;
		case '3':
			cout << "Enter the number of contact for deleting: " << endl;
			cin >> ContactNumber;
			cin.ignore();
			if (ContactNumber > count || ContactNumber < 1)
			{
				cout << "Invalid number!" << endl;
				break;
			}
			DeleteContact(MainPtr, count, size, ContactNumber - 1);
			break;
		case '4':
			ShowAll(MainPtr, count);
			break;
		case '5':
			cout << "Enter the number of contact: " << endl;
			cin >> ContactNumber;
			cin.ignore();
			if (ContactNumber > count || ContactNumber < 1)
			{
				cout << "Invalid number!" << endl;
				break;
			}
			ShowContact(MainPtr, ContactNumber - 1);
			break;
		case '6':
			cout << "Enter the number of contact for searching: (0 - Name, 1 - Number, else - break)" << endl;
			cin >> ContactNumber;
			cin.ignore();
			if (ContactNumber == 0)
			{
				char Name[30];
				cout << "Enter name: " << endl;
				cin.getline(Name, 30);
				FindContactFromName(MainPtr, count, Name);
			}
			else if (ContactNumber == 1)
			{
				char Number[150];
				cout << "Enter number: " << endl;
				cin.getline(Number, 150);
				FindContactFromNumber(MainPtr, count, Number);
			}

			break;
		case '7':
		{

			cout << "Enter file name for writing: ";	//можно приписывать сразу с расширением файла
			char fName[30];
			cin.getline(fName, 30);
			//cin.ignore();
			ofstream ofile(fName);
			if (!ofile)
				cout << "Error in openind file for writing.\n";
			else
				WriteFile(MainPtr, count, ofile);
		}
		break;
		case '8':
		{

			cout << "Enter file name for reading: " << endl;
			cin.ignore();
			char fName[30];
			cin.getline(fName, 30);
			ifstream ifile(fName);
			if (!ifile)
				cout << "Error in openind file for reading.\n";
			else
			{
				ReadFile(MainPtr, size, count, ifile);
			}
		}
		break;

		case '9':
			return 0;
		}

		cout << endl << "Do you want to make another operation? (1 - yes, 0 - no)" << endl;
		Exit = _getch();
	} while (Exit != '0');


	delete[]MainPtr;
	MainPtr = nullptr;
}

void AddContact(Contact*& notebook, int& size, int& count, Contact& NewContact)
{
	if (count + 1 >= size) //расширяем массив, если нет пустого места
	{
		size += EXT;
		Contact* temp = new Contact[size];
		for (int i = 0; i < count + 1; i++)
		{
			temp[i] = notebook[i];
		}

		delete[]notebook;
		notebook = temp;
	}
	notebook[count] = NewContact;

	cout << endl << "New cоntact has been added:" << endl;
	ShowContact(notebook, count);
	count++;
}

void DeleteContact(Contact*& notebook, int& count, int size, int number)
{
	Contact* temp = new Contact[size];
	for (int i = 0; i < number; i++)
	{
		temp[i] = notebook[i];
	}
	for (int i = number; i < size; i++)
	{
		temp[i] = notebook[i];
	}

	delete[]notebook;
	notebook = temp;
	count--;

	cout << endl << "Cоntact has been removed." << endl;

}

void FindContactFromNumber(Contact* arr, int count, char* Number)
{
	for (int i = 0; i < count; i++)
		if (strcmp(arr[i].phone, Number) == 0)
		{
			ShowContact(arr, i);
			//return;
		}
	cout << endl << "Cоntact not found" << endl;
}

void FindContactFromName(Contact* arr, int count, char* Name)
{
	for (int i = 0; i < count; i++)
		if (strcmp(arr[i].fname, Name) == 0)
		{
			ShowContact(arr, i);
			//return;
		}
	cout << endl << "Cоntact not found" << endl;
}

void ShowContact(Contact* ptr, int index)
{
	cout << endl << "Contact number: " << index + 1 << endl;
	cout << "First name: " << ptr[index].fname << endl;
	cout << "Second name: " << ptr[index].sname << endl;
	cout << "Address: " << ptr[index].address << endl;
	cout << "Age: " << ptr[index].age << endl;
	cout << "Phone number: " << ptr[index].phone << endl;
}

void ShowAll(Contact* arr, int count)
{
	for (int i = 0; i < count; i++)
		ShowContact(arr, i);
}
void ReadFile(Contact*& notebook, int& size, int& count, ifstream& ifile)
{
	char str[100];
	Contact NewContact;
	while (ifile.getline(str, 100))
	{
		sscanf(str, "%s %s %s %s %s", NewContact.fname, NewContact.sname, NewContact.address, NewContact.age, NewContact.phone);
		AddContact(notebook, size, count, NewContact);
	}

}
void WriteFile(Contact* arr, int count, ofstream& ofile)
{
	//Date d1(31, 1, 2020); 




	for (int i = 0; i < count; i++)
	{
		time_t now = i+time(0);
		tm* ltm = localtime(&now);
		ofile << endl << "#" << i + 1 << " "
			//<< dt << " "
			<< 1900 + ltm->tm_year << "/" << 1 + ltm->tm_mon << "/" << ltm->tm_mday << " "
			<< ltm->tm_hour << ":" << 30 + ltm->tm_min << ":" << ltm->tm_sec << endl << " "

			<< arr[i].fname << " "
			<< arr[i].sname << " "
			<< arr[i].address << " "
			<< arr[i].age << " "
			<< arr[i].phone << endl;

	}

}