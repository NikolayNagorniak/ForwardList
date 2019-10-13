#include<iostream>
//#include<initializer_list>
using namespace std;

//#define INDEX_OPERATOR_CHECK
//#define CONSTRUCTORS_CHECK
#define HARDCORE_CHECK

template<typename T>class List;

template<typename T>
class Element
{
	T Data;		//�������� ��������
	Element<T>* pNext;	//����� ���������� ��������
	static int count;
public:
	Element(T Data, Element<T>* pNext = nullptr)
	{
		this->Data = Data;
		this->pNext = pNext;
		count++;
		cout << "EConstructor:\t" << this << endl;
	}
	~Element()
	{
		count--;
		cout << "EDestructor:\t" << this << endl;
	}
	friend class List<T>;
};

template<typename T>
int Element<T>::count = 0;

template<typename T>
class List
{
	Element<T>* Head;	//����� ������ ������
	int size;		//������ ������
public:
	int get_size()const
	{
		return this->size;
	}

	List()
	{
		this->Head = nullptr;
		this->size = 0;
		cout << "LConstructor:\t" << this << endl;
	}
	List(int size) :List()
	{
		/*this->Head = nullptr;
		this->size = 0;*/
		while (size--)push_front(0);

	}

	~List()
	{
		while (Head)pop_front();
		cout << "LDestructor:\t" << this << endl;
	}

	//			Operators:
	const T& operator[](int Index)const
	{
		Element<T>* Temp = Head;
		for (int i = 0; i < Index; i++)Temp = Temp->pNext;
		return Temp->Data;
	}
	T& operator[](int Index)
	{
		Element<T>* Temp = Head;
		for (int i = 0; i < Index; i++)Temp = Temp->pNext;
		return Temp->Data;
	}

	//			Adding elements:
	void push_front(T Data)
	{
		//1)������� ����� �������
		Element<T>* New = new Element<T>(Data);
		//2)"����������� ����� ������� � ������ (Head)":
		New->pNext = Head;
		//3)������ ��������� �� ����� �������
		Head = New;
		size++;
	}

	//���������� �������� � ����� ������
	void push_back(T Data)
	{
		if (Head == nullptr)
		{
			push_front(Data);
			return;
		}
		//1) ������� �� ����� ������
		Element<T>* Temp = Head;
		while (Temp->pNext != nullptr)
		{
			Temp = Temp->pNext;
		}
		//2) ��������� �������
		Temp->pNext = new Element<T>(Data);
		size++;
	}

	//������� �������� �� ���������� �������
	void insert(int Index, T Data)
	{
		if (Index == 0)
		{
			push_front(Data);
			return;
		}
		if (Index >= size)
		{
			push_back(Data);
			return;
		}
		//1) ����� �� ������ �������
		Element<T>* Temp = Head;
		//if(sizeof(Temp))
		for (int i = 0; i < Index - 1; i++)Temp = Temp->pNext;
		//2) �������� ������� � ������
		Element<T>* New = new Element<T>(Data);
		New->pNext = Temp->pNext;
		Temp->pNext = New;
		size++;
	}

	//			Removing elements:
	void pop_front()
	{
		if (Head == nullptr)return;
		//1) ���������� ����� ���������� ��������
		Element<T>* to_del = Head;
		//2) ��������� ������� �� ������
		Head = Head->pNext;
		//3) ������� ������� �� ������
		delete to_del;
		size--;
	}
	void pop_back()
	{
		//���� ������ ����, ������� �� ���� ������
		//������� ����� ������� �� ������
		//if (Head == nullptr)return;
		if (Head == nullptr || Head->pNext == nullptr)
		{
			pop_front();
			return;
		}

		//1)����� �� ����� ������
		Element<T>* Temp = Head;
		while (Temp->pNext->pNext != nullptr)
		{
			Temp = Temp->pNext;
		}
		//2)������� �������
		delete Temp->pNext;
		Temp->pNext = nullptr;
		size--;
	}

	//�������� �������� �� ������ �� ���������� �������
	void erase(int Index)
	{
		if (Index == 0)
		{
			pop_front();
			return;
		}
		if (Index == size - 1)
		{
			pop_back();
			return;
		}
		if (Index >= size)
		{
			return;
		}
		Element<T>* Temp = Head;
		for (int i = 0; i < Index - 1; i++)Temp = Temp->pNext;
		Element<T>* to_del = Temp->pNext;
		Temp->pNext = Temp->pNext->pNext;
		delete to_del;
		size--;
	}

	void print()
	{
		Element<T>* Temp = Head;	//Temp - ��� ��������

		while (Temp != nullptr)
		{
			cout << Temp << "\t" << Temp->Data << "\t" << Temp->pNext << endl;
			Temp = Temp->pNext;
		}
		cout << "���������� ��������� ������: " << size << endl;
	}

	//	1. ����������� �����������
	List(const List<T>& other):List()
	{
		//this->Head = other.Head;
		Element<T> * Temp = other.Head;
		while (Temp)
		{
			push_back(Temp->Data);
			Temp = Temp->pNext;
		}
		this->size = other.size;
		cout << "CopyConstrutor:" << this << endl;
	}

	//	2. ����������� ������������
	List<T>& operator=(const List<T>& other)	//������ List& � ������ ������ - ������ ��������� ������ � "��������"
		//	"������" = "������"
	{
		if (this->Head != nullptr) this->Head = nullptr;	//������� �� ������ ��������
		Element<T>* Temp = other.Head;
		while (Temp)
		{
			push_back(Temp->Data);
			Temp = Temp->pNext;
		}
		size = other.size;
		cout << "CopyAssignment: " << this << endl;
		return *this;
	}

	//	3. ����������� ����������� - MoveConstructor
	List(List<T>&& other)
	{
		//����������� ��������� � �� ���������� � ��������� ������
		/*Element* Temp = other.Head;
		while (Temp)
		{
			push_back(Temp->Data);
			Temp = Temp->pNext;
		}*/
		this->Head = other.Head;
		size = other.size;
		//"�������" ��������� ��������� ������ (������������)
		//���������� �� ��������� ������������ ������
		other.Head = nullptr;
		other.size = 0;
		cout << "MoveConstrutor:" << this << endl;
	}

	//	4. ������������ ���������� - MoveAssignment
	List<T>& operator=(List<T>&& other)
	{
		//����������� ��������� � �� ���������� � ��������� ������
		if (this->Head != nullptr) this->Head = nullptr;	//������� �� ������ ��������
		Element<T>* Temp = other.Head;
		while (Temp)
		{
			push_back(Temp->Data);
			Temp = Temp->pNext;
		}
		size = other.size;
		//"�������" ��������� ��������� ������ (������������)
		other.Head = nullptr;
		other.size = 0;
		cout << "MoveAssignment: " << this << endl;
		return *this;
	}

	//	5. ����������� ������ �������������
	List(initializer_list<T> list) : List()
	{
		for (auto& item : list)
		{
			push_back(item);
		}
	}

};

//	6. ������������ �������
template<typename T>
List<T> operator+(const List<T>& left, const List<T>& right)
{
	List<T> result = left;
	for (int i = 0; i < right.get_size(); i++)
	{
		result.push_back(right[i]);
	}
	return result;
}

void main()
{
	setlocale(LC_ALL, "");
	int n;
	cout << "������� ������ ������: "; cin >> n;
	//List list;
	/*for (int i = 0; i < n; i++)
	{
		list.push_back(rand() % 100);
	}*/
	//list.push_back(123);
	//list.print();
	/*list.pop_back();
	list.pop_front();*/

	/*List list2;
	for (int i = 0; i < 3; i++)list2.push_back(rand());
	list2.print();
	int Index;
	int Data;
	cout << "������� ������ ������������ ��������:  "; cin >> Index;
	cout << "������� �������� ������������ ��������:"; cin >> Data;
	list.insert(Index, Data);
	list.print();
	cout << "������� ������ ���������� ��������:  "; cin >> Index;
	list.erase(Index);
	list.print();*/

#ifdef INDEX_OPERATOR_CHECK
	List list(n);
	list.print();
	for (int i = 0; i < list.get_size(); i++)
	{
		list[i] = rand() % 100;
	}
	for (int i = 0; i < list.get_size(); i++)
	{
		cout << list[i] << "\t";
	}
	cout << endl;
#endif // INDEX_OPERATOR_CHECK

#ifdef CONSTRUCTORS_CHECK
	//1. ����������� �����������
	cout << "\t1. ����������� �����������" << endl;
	List<int> list01;
	for (int i = 0; i < n; i++)
	{
		list01.push_back(rand() % 100);
	}
	list01.print();
	cout << "\n\t����������� �����������:" << endl;
	List<int> list02 = List(list01);
	list02.print();

	//2. ����������� ������������
	cout << "\n\t2. ����������� ������������" << endl;
	List<int> list03;
	/*for (int i = 0; i < n; i++)
	{
		list03.push_back(33);
	}*/
	list03.print();
	list03 = list01;
	list03.print();

	//3. ����������� �����������
	cout << "\n\t3. ����������� �����������" << endl;
	List list04(move(list01));
	list04.print();
	cout << "\t�������� ������ ����� �����������:" << endl;
	list01.print();

	//4. ������������ ����������
	cout << "\n\t4. ������������ ����������" << endl;
	List list05;
	cout << "����� ������ �� �����������:" << endl;
	list05.print();
	cout << "������ ������ list02 �� �����������:" << endl;
	list02.print();
	cout << "�������� ������������� ����������:" << endl;
	list05 = (move(list02));
	cout << "����� ������ ����� ����������� � �����������:" << endl;
	list05.print();
	cout << "������ ������ list02 ����� �����������:" << endl;
	list02.print();
#endif // CONSTRUCTORS_CHECK


#ifdef HARDCORE_CHECK
	cout << "\n\t5.�������� initializer_list:" << endl;

	List<int> list1 = { 3, 5, 8, 13, 21 };
	list1.print();
	List<int> list2 = { 34, 55, 89 };
	list2.print();

	cout << "\n\t6.������������ �������" << endl;
	List<int> list3 = list1 + list2;	//������������ �������.
	list3.print();
	//http://www.cplusplus.com/reference/initializer_list/initializer_list/
#endif // HARDCORE_CHECK

	List<double> dbl_list = { 2.5, 3.14, 8.2 };
	dbl_list.print();

}