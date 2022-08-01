#include <chrono>
#include <ctime>
#include <random>
#include <iostream>

uint32_t GetNanos() {
	using namespace std::chrono;
	return static_cast<uint32_t>(duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count());
}

void radixSort(int* arrIn, int arrLen) {
	int *tempArr = new int[arrLen]; 
	int numPasses = 0;
	int sig = 0;
	int* buckets = new int[(arrLen + 1) * 10];
	for (int i = 0; i < 10; i++) buckets[((arrLen + 1) * i) + arrLen] = 0;

	for (int i = 0; i < arrLen; i++) {
		int tempS = 0;
		while (tempS > -1) {
			tempS++;
			if (arrIn[i] == arrIn[i] % int(std::pow(10, tempS))) break;
		}
		numPasses = std::max(numPasses, tempS);
		tempArr[i] = arrIn[i];
	}
	if (numPasses < 1) return;
	for (int i = 0; i < numPasses; i++) {
		for (int j = 0; j < arrLen; j++) {
			int digit = (tempArr[j] % int(std::pow(10.0, i + 1))) / std::pow(10.0, i);
			buckets[digit * (arrLen + 1) + buckets[digit * (arrLen + 1) + arrLen]] = tempArr[j];
			buckets[digit * (arrLen + 1) + arrLen] += 1;
		}
		int ins = 0;
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < buckets[(j * (arrLen + 1)) + arrLen]; k++) {
				tempArr[ins] = buckets[(j * (arrLen + 1)) + k];
				ins++;
			}
			buckets[(j * (arrLen + 1)) + arrLen] = 0;
		}
	}
	for (int i = 0; i < arrLen; i++)
		arrIn[i] = tempArr[i];
	delete[] tempArr;
	delete[] buckets;
}

int main() {
	std::cout << "Day 01\n";
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> distribution(0.00, 1.00);
	double distRange = 10000;
	int inCount;
	std::cout << "Input the number of random numbers to generate: ";
	std::cin >> inCount;
	int* distArray = new int[inCount];
	std::cout << "\nYour randomly generated array is [";
	for (int i = 0; i < inCount - 1; i++) {
		distArray[i] = distRange * distribution(generator);
		std::cout << distArray[i] << ", ";
	}
	distArray[inCount - 1] = distRange * distribution(generator);
	std::cout << distArray[inCount - 1] << "]\n";
	uint32_t startTime = GetNanos();
	radixSort(distArray, inCount);
	uint32_t endTime = GetNanos();
	std::cout << "\nYour radix-sorted array is [";
	for (int i = 0; i < inCount - 1; i++) {
		std::cout << distArray[i] << ", ";
	}
	std::cout << distArray[inCount - 1] << "]\n";
	std::cout << "It took " << endTime - startTime << " nanoseconds.\n";
	return 0;
}