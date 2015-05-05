//============================================================================
//				Instituto Superior Tecnico
//			Projecto Analise e Sintese de Algoritmos
//
// Date:		: Marco, 2014
// Author		: Vasco Loureiro, 70993
// Author		: Nuno Cameira, 69769
// Version		: 1.0.0.0
// Description	:
//============================================================================

#include <vector>
#include <stdio.h>
#include <iostream>
using namespace std;

class Node {

private:
	int _id;
	int _index;
	int _low;
	int _scc;

	bool _visited;
	vector<int> _followers;
public:
	Node() :
			_id(-1), _index(-1), _low(-1), _scc(-1), _visited(false), _followers(
					0) {
	}
	Node(int id, int index, int max) :
			_id(id), _index(index), _low(index), _scc(-1), _visited(false), _followers(
					0) {
	}

	int getFollowers() {
		return _followers.size();
	}

	void addFollower(int follower) {
		_followers.push_back(follower);

	}
	std::vector<int>::iterator firstFollower() {
		return _followers.begin();
	}
	std::vector<int>::iterator lastFollower() {
		return _followers.end();
	}

	int getLow() const {
		return _low;
	}
	void setLow(int low) {
		_low = low;
	}

	int getIndex() const {
		return _index;
	}

	void setIndex(int index) {
		_index = index;
	}
	int getID() const {
		return _id;
	}
	bool isVisited() {
		return _visited;
	}

	void setVisited(bool visited) {
		_visited = visited;
	}

	void setSCC(int val) {
		_scc = val;
	}
	int getSCC() {
		return _scc;
	}

	bool operator!=(const Node &other) const {
		return (_id != other._id);
	}

};

class Stack {
private:
	vector<int> visitedOrder;
	vector<int> visitedOrderSorted;
public:
	Stack(int numberOfElements) :
			visitedOrder(0), visitedOrderSorted(numberOfElements) {
	}

	void push_back(int val) {
		visitedOrder.push_back(val);
		visitedOrderSorted[val] = 1;
	}

	int pop_back() {
		int lastElement = visitedOrder.back();
		visitedOrder.pop_back();
		visitedOrderSorted[lastElement] = 0;
		return lastElement;
	}

	bool contains(int val) {

		return (visitedOrderSorted[val] == 1);

	}

};

void tarzan_visit(int pos, Node graph[], int *index, Stack *stack,
		int groups[]) {
	Node *visitingNode = &graph[pos];
	visitingNode->setIndex(*index);
	visitingNode->setLow(*index);
	visitingNode->setVisited(true);
	*index += 1;
	stack->push_back(visitingNode->getID());

	// Consider successors of visitingNode
	Node *nextFollower = 0;

	for (std::vector<int>::iterator it = visitingNode->firstFollower();
			it != visitingNode->lastFollower(); ++it) {
		nextFollower = &graph[*it];

		if (!nextFollower->isVisited()) {
			tarzan_visit(*it, graph, index, stack, groups);

			visitingNode->setLow(
					min(visitingNode->getLow(), nextFollower->getLow()));

		} else if (stack->contains(*it)) {
			visitingNode->setLow(
					min(visitingNode->getLow(), nextFollower->getIndex()));
		}

	} //For: NextChild

	// If visitingNode is a root node, pop the stack and generate a SCC
	if (visitingNode->getLow() == visitingNode->getIndex()) {

		groups[0]++;
		Node *followerChild = 0;
		Node *follower = 0;
		int groupSize = 0;
		bool isSelfish = true;

		do {
			follower = &graph[stack->pop_back()];
			follower->setSCC(groups[0]);

			for (std::vector<int>::iterator it = follower->firstFollower();
					it != follower->lastFollower(); ++it) {
				followerChild = &graph[*it];
				if (followerChild->getSCC() == -1)
					continue; //Child is in Stack

				if (followerChild->getSCC() != follower->getSCC()) {
					isSelfish = false;
					break; //At least one element has an outside reference
				}
			}

			groupSize++;
		} while (follower != visitingNode);

		if (isSelfish)
			groups[2]++;
		groups[1] = max(groups[1], groupSize);

	}
}

int main() {
	int nNodes = 0;
	int nConnections = 0;
	scanf("%i %i", &nNodes, &nConnections);
	Node graph[nNodes];
	for (int i = 0; i < nNodes; i++) {
		graph[i] = Node(i, 0, nNodes);
	}

	int leader = 0;
	int nextFollower = 0;

	while (scanf("%i %i", &leader, &nextFollower) != EOF) {
		graph[leader - 1].addFollower(nextFollower - 1);

	}

	int level = 0;

	Stack stack = Stack(nNodes);
	int groups[3] = { 0, 0, 0 };
	for (int i = 0; i < nNodes; i++) {

		if (!graph[i].isVisited()) {
			tarzan_visit(i, graph, &level, &stack, groups);
		}

	}

	cout << groups[0] << endl;
	cout << groups[1] << endl;
	cout << groups[2] << endl;

	return 0;
}
