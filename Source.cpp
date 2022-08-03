#include <chrono>
#include <ctime>
#include <random>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>

class pathNode;

class edge {
public:
	int cost;
	pathNode *endpoint;
	edge() {
		cost = 0;
		endpoint = nullptr;
	}
	edge(pathNode *nodeIn, int costIn) {
		cost = costIn;
		endpoint = nodeIn;
	}
};

class pathNode {
public:
	char ID;
	int heuristic;
	std::vector<edge> edgeOut;
	pathNode* nextNode;
	pathNode* prevNode;
	int thisCost;

	pathNode() {
		ID = ' ';
		nextNode = nullptr;
		prevNode = nullptr;
		thisCost = -1;
		heuristic = 0;
	}

	pathNode(char idIn) {
		ID = idIn;
		nextNode = nullptr;
		prevNode = nullptr;
		thisCost = -1;
		heuristic = 0;
	}

	void cascadeCost(int decrement) {
		thisCost -= decrement;
		if (nextNode != nullptr) nextNode->cascadeCost(decrement);
	}

	int getEdgeCost(char goal) {
		for (int i = 0; i < edgeOut.size(); i++) {
			if (edgeOut[i].endpoint->ID == goal) return edgeOut[i].cost;
		}
		return -1;
	}

	void addEdge(pathNode* nodeIn, int costIn) {
		for (int i = 0; i < edgeOut.size(); i++) {
			if (nodeIn->ID == edgeOut[i].endpoint->ID) {
				edgeOut[i].cost = std::min(costIn, edgeOut[i].cost);
				return;
			}
		}
		edgeOut.push_back(edge(nodeIn, costIn));
	}
};

bool shouldAddNode(std::vector<pathNode> potentials, pathNode candidate) {
	bool visitedPass = true;
	bool potentialPass = true;

	for (int i = 0; i < potentials.size(); i++) {
		return true;
	}
	return false;
}

int pathAStar(std::vector<pathNode> nodesIn, char sNode, char eNode) {
	if (sNode == eNode) return 0;

	pathNode* startN = nullptr, * endN = nullptr;
	for (int i = 0; i < nodesIn.size(); i++) {
		if (nodesIn[i].ID == sNode) startN = &nodesIn[i];
		else if (nodesIn[i].ID == eNode) endN = &nodesIn[i];
	}
	if (startN == nullptr || endN == nullptr) {

		return -1;
	}

	std::vector<pathNode*> potNodes;
	std::vector<pathNode*> visNodes;

	visNodes.push_back(startN);
	visNodes[0]->thisCost = 0;
	visNodes[0]->prevNode = visNodes[0];
	pathNode* current = startN;
	for (int i = 0; i < visNodes[0]->edgeOut.size(); i++) {
		potNodes.push_back(visNodes[0]->edgeOut[i].endpoint);
		potNodes[i]->prevNode = current;
		//potNodes[i]->thisCost = visNodes[0]->edgeOut[i].cost;
	}

	while (potNodes.size() != 0) {
		int costMin = INT32_MAX;
		int potIndex = -1;
		for (int i = 0; i < potNodes.size(); i++) {
			if (potNodes[i]->ID == eNode) {
				return potNodes[i]->prevNode->thisCost + potNodes[i]->prevNode->getEdgeCost(eNode);
			}
			else if (costMin > current->getEdgeCost(potNodes[i]->ID) + potNodes[i]->heuristic) {
				costMin = current->getEdgeCost(potNodes[i]->ID) + potNodes[i]->heuristic;
				potIndex = i;
			}
		}
		current->nextNode = potNodes[potIndex];
		current = potNodes[potIndex];
		if (current->thisCost == -1) {
			current->thisCost = current->prevNode->getEdgeCost(current->ID) + current->prevNode->thisCost;
		}

		for (int i = 0; i < current->edgeOut.size(); i++) {
			if (current->edgeOut[i].endpoint->prevNode == nullptr) {
				potNodes.push_back(current->edgeOut[i].endpoint);
				current->edgeOut[i].endpoint->prevNode = current;
			}
			else if (current->edgeOut[i].endpoint->thisCost > current->thisCost + current->edgeOut[i].cost) {
				current->edgeOut[i].endpoint->prevNode = current;
				current->edgeOut[i].endpoint->thisCost = current->thisCost + current->edgeOut[i].cost;
			}
		}
		potNodes.erase(potNodes.begin() + potIndex);
	}
	return -1;

}

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
	
	std::vector<pathNode> nodeList;
	char nodes[2];
	int cost;
	char endC, startC;
	std::ifstream fileIn("Paths.txt");
	std::string line;
	while (std::getline(fileIn, line))
	{
		for (int i = 0; i < line.size(); i += 0)
			if (line[i] == '(' || line[i] == ')' || line[i] == ',') line.erase(i, 1);
			else i++;

		std::stringstream lineStream(line); 
		int inCount = 0;
		while (lineStream.good()) {
			std::string inVal;
			std::getline(lineStream, inVal, ' '); 
			if (inCount < 2) nodes[inCount] = inVal[0];
			else cost = std::stoi(inVal);
			inCount++;
		}
		if (inCount == 3) {
			int foundA = -1;
			int foundB = -1;
			for (int i = 0; i < nodeList.size(); i++) {
				if (nodeList[i].ID == nodes[0]) foundA = i;
				else if (nodeList[i].ID == nodes[1]) foundB = i;
			}
			if (foundA == -1) {
				foundA = nodeList.size();
				nodeList.push_back(pathNode(nodes[0]));
			}
			if (foundB == -1) {
				foundB = nodeList.size();
				nodeList.push_back(pathNode(nodes[1]));
			}
			nodeList[foundA].addEdge(&nodeList[foundB], cost);
			nodeList[foundB].addEdge(&nodeList[foundA], cost);
		}
		else if (inCount == 2) {
			// Set goal/ start
			startC = nodes[0];
			endC = nodes[1];
		}
		else {
			// Bad input
			;
		}
	}
	std::cout << "====\n" << pathAStar(nodeList, startC, endC) << "\n====\n";
	return 0;
}