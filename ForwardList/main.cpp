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
	T Data;		//Значение элемента
	Element<T>* pNext;	//Адрес следующего элемента
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
	Element<T>* Head;	//Адрес начала списка
	int size;		//Размер списка
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
		//1)Создаем новый элемент
		Element<T>* New = new Element<T>(Data);
		//2)"Привязываем новый элемент к голове (Head)":
		New->pNext = Head;
		//3)Голову переводим на новый элемент
		Head = New;
		size++;
	}

	//Добавление элемента в конец списка
	void push_back(T Data)
	{
		if (Head == nullptr)
		{
			push_front(Data);
			return;
		}
		//1) Доходим до конца списка
		Element<T>* Temp = Head;
		while (Temp->pNext != nullptr)
		{
			Temp = Temp->pNext;
		}
		//2) Добавляем элемент
		Temp->pNext = new Element<T>(Data);
		size++;
	}

	//Вставка значения по указанному индексу
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
		//1) Дойти до нужной позиции
		Element<T>* Temp = Head;
		//if(sizeof(Temp))
		for (int i = 0; i < Index - 1; i++)Temp = Temp->pNext;
		//2) Добавить элемент в список
		Element<T>* New = new Element<T>(Data);
		New->pNext = Temp->pNext;
		Temp->pNext = New;
		size++;
	}

	//			Removing elements:
	void pop_front()
	{
		if (Head == nullptr)return;
		//1) Запоминаем адрес удаляемого элемента
		Element<T>* to_del = Head;
		//2) Исключаем элемент из списка
		Head = Head->pNext;
		//3) Удаляем элемент из списка
		delete to_del;
		size--;
	}
	void pop_back()
	{
		//Если список пуст, удалять из него нечего
		//поэтому сразу выходим из метода
		//if (Head == nullptr)return;
		if (Head == nullptr || Head->pNext == nullptr)
		{
			pop_front();
			return;
		}

		//1)Дойти до конца списка
		Element<T>* Temp = Head;
		while (Temp->pNext->pNext != nullptr)
		{
			Temp = Temp->pNext;
		}
		//2)Удалить элемент
		delete Temp->pNext;
		Temp->pNext = nullptr;
		size--;
	}

	//Удаление элемента из списка по указанному индексу
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
		Element<T>* Temp = Head;	//Temp - это итератор

		while (Temp != nullptr)
		{
			cout << Temp << "\t" << Temp->Data << "\t" << Temp->pNext << endl;
			Temp = Temp->pNext;
		}
		cout << "Количество элементов списка: " << size << endl;
	}

	//	1. Конструктор копирования
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

	//	2. Конструктор присваивания
	List<T>& operator=(const List<T>& other)	//Ссылка List& в начале строки - нельзя присвоить ссылку в "значение"
		//	"ссылка" = "ссылка"
	{
		if (this->Head != nullptr) this->Head = nullptr;	//Очистка от старых значений
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

	//	3. Конструктор перемещения - MoveConstructor
	List(List<T>&& other)
	{
		//Копирование элементов и их количества с исходного списка
		/*Element* Temp = other.Head;
		while (Temp)
		{
			push_back(Temp->Data);
			Temp = Temp->pNext;
		}*/
		this->Head = other.Head;
		size = other.size;
		//"Отвязка" элементов исходного списка (освобождение)
		//Деструктор не повторяет освобождение памяти
		other.Head = nullptr;
		other.size = 0;
		cout << "MoveConstrutor:" << this << endl;
	}

	//	4. Перемещающее присвоение - MoveAssignment
	List<T>& operator=(List<T>&& other)
	{
		//Копирование элементов и их количества с исходного списка
		if (this->Head != nullptr) this->Head = nullptr;	//Очистка от старых значений
		Element<T>* Temp = other.Head;
		while (Temp)
		{
			push_back(Temp->Data);
			Temp = Temp->pNext;
		}
		size = other.size;
		//"Отвязка" элементов исходного списка (освобождение)
		other.Head = nullptr;
		other.size = 0;
		cout << "MoveAssignment: " << this << endl;
		return *this;
	}

	//	5. Конструктор списка инициализации
	List(initializer_list<T> list) : List()
	{
		for (auto& item : list)
		{
			push_back(item);
		}
	}

};

//	6. Конкатенация списков
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
	cout << "Введите размер списка: "; cin >> n;
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
	cout << "Введите индекс добавляемого элемента:  "; cin >> Index;
	cout << "Введите значение добавляемого элемента:"; cin >> Data;
	list.insert(Index, Data);
	list.print();
	cout << "Введите индекс удаляемого элемента:  "; cin >> Index;
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
	//1. Конструктор копирования
	cout << "\t1. Конструктор копирования" << endl;
	List<int> list01;
	for (int i = 0; i < n; i++)
	{
		list01.push_back(rand() % 100);
	}
	list01.print();
	cout << "\n\tКонструктор копирования:" << endl;
	List<int> list02 = List(list01);
	list02.print();

	//2. Конструктор присваивания
	cout << "\n\t2. Конструктор присваивания" << endl;
	List<int> list03;
	/*for (int i = 0; i < n; i++)
	{
		list03.push_back(33);
	}*/
	list03.print();
	list03 = list01;
	list03.print();

	//3. Конструктор перемещения
	cout << "\n\t3. Конструктор перемещения" << endl;
	List list04(move(list01));
	list04.print();
	cout << "\tИсходный список после перемещения:" << endl;
	list01.print();

	//4. Перемещающее присвоение
	cout << "\n\t4. Перемещающее присвоение" << endl;
	List list05;
	cout << "Новый список до перемещения:" << endl;
	list05.print();
	cout << "Старый список list02 до перемещения:" << endl;
	list02.print();
	cout << "Операция перемещающего присвоения:" << endl;
	list05 = (move(list02));
	cout << "Новый список после перемещения с присвоением:" << endl;
	list05.print();
	cout << "Старый список list02 после перемещения:" << endl;
	list02.print();
#endif // CONSTRUCTORS_CHECK


#ifdef HARDCORE_CHECK
	cout << "\n\t5.Проверка initializer_list:" << endl;

	List<int> list1 = { 3, 5, 8, 13, 21 };
	list1.print();
	List<int> list2 = { 34, 55, 89 };
	list2.print();

	cout << "\n\t6.Конкатенация списков" << endl;
	List<int> list3 = list1 + list2;	//Конкатенация списков.
	list3.print();
	//http://www.cplusplus.com/reference/initializer_list/initializer_list/
#endif // HARDCORE_CHECK

	List<double> dbl_list = { 2.5, 3.14, 8.2 };
	dbl_list.print();

}