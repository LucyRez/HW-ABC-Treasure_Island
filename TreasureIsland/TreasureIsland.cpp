/*
ВАРИАНТ 25
Первая задача об Острове Сокровищ. Шайка пиратов под
предводительством Джона Сильвера высадилась на берег Острова Сокровищ.
Не смотря на добытую карту старого Флинта, местоположение сокровищ попрежнему остается загадкой,
поэтому искать клад приходится практически
на ощупь.Так как Сильвер ходит на деревянной ноге, то самому бродить по
джунглям ему не с руки.Джон Сильвер поделил остров на участки, а пиратов
на небольшие группы.Каждой группе поручается искать клад на одном из
участков, а сам Сильвер ждет на берегу.Пираты, обшарив свою часть
острова, возвращаются к Сильверу и докладывают о результатах.Требуется
создать многопоточное приложение с управляющим потоком, моделирующее
действия Сильвера и пиратов.
*/

#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <cstdio>

int indexTreasure; // Положение сокровища на острове.
int numberGroups; // Количество групп пиратов.
int width; // Ширина острова.
int height; // Высота острова.
std::string input; // Строка для хранения пользовательского ввода.

std::mutex lock;
std::vector<std::thread> threads;

std::vector<int> island; // Ячейки массива - части острова.
std::vector<int> piratesInGroups; // Массив, содержащий группы пиратов (кол-во в каждой группе).
std::vector<int> numsOfParts; // Массив с количеством частей острова для каждой группы.


/// <summary>
/// Метод осуществляет парсинг пользовательского ввода.
/// </summary>
/// <param name="input">Строка, введённая пользователем.</param>
/// <returns>Целое число, полученное в результате парсинга.</returns>
int checkInput(std::string input) {
	// Отлавливаем все исключения.
	try {
		int val = atoi(input.c_str());
		if (val <= 0)
		{
			return 0;
		}
		return val;
	}
	catch (const std::exception& e) {
		return 0;
	}

}

/// <summary>
/// Метод запрашивает повтор ввода в случае, если пользователь ввёл некорректное значение.
/// </summary>
/// <param name="value">Число, проверяемое на корректность.</param>
/// <returns>Корректное целое число.</returns>
int repeatInput(int value) {
	std::string str;
	while (value == 0) {
		std::cout << "Вы ввели некорректное значение, оно должно быть больше 0. Повторите ввод, пожалуйста. \n";
		std::cin >> str;
		value = checkInput(str);
	}
	return value;
}

/// <summary>
/// Метод, создающий остров и начинающий работу программы.
/// </summary>
void start() {
	std::cout << "Введите ширину острова (целое число > 0): ";
	std::cin >> input;
	width = repeatInput(checkInput(input)); // Проверяем ввод.
	std::cout << "Введите высоту острова (целое число > 0): ";
	std::cin >> input;
	height = repeatInput(checkInput(input));

	// Заполняем массив острова.
	for (int i = 0; i < width * height; i++)
	{
		island.push_back(0);
	}

	indexTreasure = std::rand() % (width * height);

	island[indexTreasure] = 1; // Место с сокровищем помечается единицей.
}

/// <summary>
/// Метод выводит карту острова (без значения ячеек) на экран.
/// </summary>
void printIsland() {
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cout << "*" << " ";
		}
		std::cout << "\n";
	}
}

/// <summary>
/// Метод выводит открытую часть острова на экран.
/// </summary>
/// <param name="indexGroup">Индекс вернувшейся группы пиратов.</param>
/// <param name="explored">Количество открытых ячеек.</param>
void islandMap(int indexGroup, int explored) {
	std::cout << "Группа отметила исследованную зону на карте: \n";
	//int groupExplored = explored - numsOfParts[indexGroup];
	int index = explored;
	int ind = 0;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (explored!= numsOfParts[indexGroup])
			{
				std::cout << "*" << " ";
				explored--;
			}
			else if(ind != numsOfParts[indexGroup])
			{
				std::cout << island[j + i * width] << " ";
				ind++;
			}
			else {
				std::cout << "*" << " ";
			}

		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

/// <summary>
/// Метод осуществляет разделение пиратов на группы.
/// </summary>
void createCrew() {
	int numPirates;
	int numGroup = 0;
	int numFree;
	std::cout << "\n \n";
	std::cout << "Введите общее количество пиратов: ";
	std::cin >> input;
	numPirates = repeatInput(checkInput(input)); // Проверяем ввод.
	numFree = numPirates;
	std::cout << "\n \n";
	std::cout << "Давайте разделим остров на участки и сформируем группы пиратов. \n";
	while (numFree != 0) {
		int numInGroup;

		if (numGroup == width * height)
		{
			std::cout << "Вы не можете создать больше групп. Все свободные пираты были распределены в последнюю группу. \n ";
			piratesInGroups[numGroup - 1] += numFree;
			numFree == 0;
			break;
		}

		std::cout << "Формируем группу №" << (numGroup + 1) << "\n";
		std::cout << "Количество нераспределённых пиратов: " << numFree << "\n";
		std::cout << "Введите количество человек в этой группе: ";
		std::cin >> input;
		numInGroup = repeatInput(checkInput(input));

		while (numInGroup > numFree) {
			std::cout << "Вы ввели слишком большое число, количество свободных пиратов - " << numFree << "\n";
			std::cout << "Попробуйте ещё раз: ";
			std::cin >> input;
			numInGroup = repeatInput(checkInput(input));
		}

		numFree -= numInGroup;
		piratesInGroups.push_back(numInGroup);
		numGroup++;
		std::cout << "\n \n";
	}

	std::cout << "Все группы сформированы, осталось разделить территорию между ними. \n";
	numberGroups = numGroup;
	std::cout << "\n \n";

}

/// <summary>
/// Метод осуществляет распределение территории острова между командами.
/// </summary>
void partCrew() {
	int unsearched = width * height; // Количество неназначеных никому ячеек.
	int area; // Территория, назначаемая 1 команде.
	for (int i = 0; i < piratesInGroups.size() - 1; i++)
	{
		std::cout << "Не распределено ещё " << unsearched << " квадрат. м. острова. \n";
		std::cout << "Назначьте группе №" << (i + 1) << " размер участка острова (1 целое число): ";
		std::cin >> input;
		area = repeatInput(checkInput(input));
		// Территория, которую может взять 1 группа, ограничена. ( Чтобы всем группам досталась часть острова.) 
		while (area > unsearched || area > (width * height) / numberGroups) {
			std::cout << "Вы ввели слишком большое число. Замечание: группе нельзя назначить больше " << (width * height) / numberGroups << " кв. м. \n";
			std::cout << "Попробуйте ещё раз: ";
			std::cin >> input;
			area = repeatInput(checkInput(input));
		}
		numsOfParts.push_back(area);
		unsearched -= area;
		std::cout << "\n \n";
	}
	std::cout << "Последняя группа берёт на себя всю оставшуюся территорию острова - " << unsearched << " кв. м. \n";
	numsOfParts.push_back(unsearched);
	unsearched = 0;
	std::cout << "\n \n";

}

/// <summary>
/// Метод осуществляет действие потока.
/// </summary>
/// <param name="groupsNum">Количество групп.</param>
/// <param name="indexGroup">Номер группы.</param>
void groupAction(int groupsNum, int indexGroup) {
	bool found = false;
	int explored = 0; // Количество исследованных ячеек острова.

	for (int j = 0; j < indexGroup - 1; j++)
	{
		explored += numsOfParts[j];
	}

	found = false; // Найден ли клад.
	for (int i = 0; i < numsOfParts[indexGroup - 1]; i++)
	{
		// Если значение ячейки острова == 1.
		if (island[i + explored] == 1) {
			found = true;
		}
	}
	explored += numsOfParts[indexGroup - 1];

	// Вывод не доступен, пока один поток не завершит вывод.
	lock.lock();
	std::cout << "Группа №" << indexGroup << " вернулась из поисков. \n";
	if (found)
	{
		std::cout << "Она нашла сокровища! \n";
	}
	else {
		std::cout << "Она не нашла сокровища. \n";
	}

	islandMap(indexGroup - 1, explored);
	lock.unlock();
	// Открываем доступ.
}

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "rus");
	start(); // Начало работы.
	printIsland(); // Выводим на экран остров.
	createCrew(); // Формируем группы пиратов.
	partCrew(); // Разделяем территорию острова между группами.

	// Создаём кол-во потоков, равное количеству групп.
	for (int i = 0; i < numberGroups; i++)
	{
		threads.push_back(std::thread(groupAction, numberGroups, i + 1));
	}

	for (int i = 0; i < numberGroups; i++)
	{
		threads.at(i).join();
	}

	// Для выхода нужно ввести что-нибудь.
	std::string str;
	std::cin >> str;
}


