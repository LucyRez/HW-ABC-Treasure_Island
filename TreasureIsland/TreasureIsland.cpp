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

int indexTreasure;
int numberGroups;
int width;
int height;

std::mutex lock;

std::vector<int> island;
std::vector<int> piratesInGroups;
std::vector<int> numsOfParts;
std::vector<std::thread> threads;



void start() {
	std::cout << "Введите ширину острова (целое число > 0): ";
	std::cin >> width;
	std::cout << "Введите высоту острова (целое число > 0): ";
	std::cin >> height;

	for (int i = 0; i < width * height; i++)
	{
		island.push_back(0);
	}

	indexTreasure = std::rand() % (width * height);

	island[indexTreasure] = 1;
}

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

void islandMap(int indexGroup, int explored ) {
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (explored != 0)
			{
				std::cout << island[j + i * width] << " ";
				explored--;
			}
			
		}
		std::cout << "\n";
	}
}

void createCrew() {
	int numPirates;
	int numGroup = 0;
	int numFree;
	std::cout << "Введите количество пиратов в команде: ";
	std::cin >> numPirates;
	numFree = numPirates;

	std::cout << "Давайте разделим остров на участки и сформируем группы пиратов. \n";
	while (numFree != 0) {
		int numInGroup;

		if (numGroup == width * height)
		{
			std::cout << "Вы не можете создать больше групп. Все свободные пираты были распределены в последнюю группу. ";
			piratesInGroups[numGroup - 1] += numFree;
		}

		std::cout << "Формируем группу №" << (numGroup + 1) << "\n";
		std::cout << "Количество нераспределённых пиратов: " << numFree << "\n";
		std::cout << "Введите количество человек в этой группе: ";
		std::cin >> numInGroup;

		while (numInGroup > numFree) {
			std::cout << "Вы ввели слишком большое число, количество свободных пиратов - " << numFree << "\n";
			std::cout << "Попробуйте ещё раз: ";
			std::cin >> numInGroup;
		}

		numFree -= numInGroup;
		piratesInGroups.push_back(numInGroup);
		numGroup++;

	}

	std::cout << "Все группы сформированы, осталось разделить территорию между ними. \n";
	numberGroups = numGroup;

	
}

void partCrew() {
	int unsearched = width * height;
	int area;
	for (int i = 0; i < piratesInGroups.size() - 1; i++)
	{
		std::cout << "Не распределено ещё " << unsearched << " квадрат. м. острова. \n";
		std::cout << "Назначьте группе №" << (i + 1) << " размер участка острова (1 целое число): ";
		std::cin >> area;
		while (area > unsearched || area > (width * height) / numberGroups) {
			std::cout << "Вы ввели слишком большое число. Замечание: группе нельзя назначить больше " << (width * height) / numberGroups << " кв. м. \n";
			std::cout << "Попробуйте ещё раз: ";
			std::cin >> area;
		}
		numsOfParts.push_back(area);
		unsearched -= area;

	}
	std::cout << "Последняя группа берёт на себя всю оставшуюся территорию острова - " << unsearched << " кв. м. \n";
	numsOfParts.push_back(unsearched);
	unsearched = 0;


}

void groupAction(int groupsNum, int indexGroup) {

	
	int explored = 0;
	for ( int j = 0; j < indexGroup-1; j++)
	{
		explored += numsOfParts[j];
	}

	bool found =false;
	for (int i = 0; i < numsOfParts[indexGroup-1] ; i++)
	{
		if (island[i + explored] == 1) {
			found = true;
		}
	}
	explored += numsOfParts[indexGroup-1];
	
	lock.lock();
	std::cout << "Группа №" << indexGroup << " вернулась из поисков. \n";
	if (found)
	{
		std::cout << "Она нашла сокровища! \n";

	}
	else {
		std::cout << "Она не нашла сокровища. \n";
	}
	std::cout << "Обновлённая карта острова: \n";
	islandMap(indexGroup-1, explored);
	lock.unlock();
	
}

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "rus");
	start();
	printIsland();
	createCrew();
	partCrew();

	for (int i = 0; i < numberGroups; i++)
	{
		
		std::thread thr(groupAction, numberGroups, i+1);
		thr.join();
		
	}
	
}


