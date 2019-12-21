#include "GraphArea.h"
#include "Edge.h"
#include "Vertex.h"
#include "Edge.h"

template <typename T>
void GraphArea::coreAlgorithm(T minHeap)
{
    // Create a set (hashtable) of all unvisited vertices.
    unvisitedVertices = new UnvisitedVertices(static_cast<int>(adjacencyList.size()));
    // Mark all vertices as unvisited.
    for (unsigned int i = 0; i < adjacencyList.size(); ++i) {
        adjacencyList[i]->setID(static_cast<int>(i));
        unvisitedVertices->insertNode(static_cast<int>(i), adjacencyList[i]);
    }
    // Insert the source vertex into the priority queue.
    minHeap->insert(dijkstraSourceVertex);
    dijkstraSourceVertex->setInPriorityQueue(true);

    while (!minHeap->isEmpty()) {
        Vertex* minDist = minHeap->findMin();
        if (minDist != nullptr) {
            if (unvisitedVertices->exists(minDist->getID())) {
                actionsList.push_back(Action(Command::CURRENTVERTEX, minDist, dijkstraCurrentVertex, nullptr, minDist->getDistance()));
                dijkstraCurrentVertex = minDist;
                unvisitedVertices->removeNode(minDist->getID());
            }
            //            minDist->changeColor(Qt::gray);   // Original version no bonus
            minHeap->remove(minDist);
            minDist->setInPriorityQueue(false);
        }

        list<pair<Vertex*, Edge*>>& currentPairsList = dijkstraCurrentVertex->pairs();
        for (list<pair<Vertex*, Edge*>>::iterator it = currentPairsList.begin(); it != currentPairsList.end(); ++it) {
            // If visited, skip
            if (!unvisitedVertices->exists(it->first->getID())) {
                actionsList.push_back(Action(Command::VISITEDVERTEX, it->first, nullptr, nullptr, it->first->getDistance()));
                continue;
            }

            // Find neighbor vertex, with the edge connecting them
            int currentNeighborDistance = it->first->getDistance();
            int potentialNewDist = it->second->getWeight() + dijkstraCurrentVertex->getDistance();
            if (potentialNewDist < currentNeighborDistance) {
                if (it->first->getInPriorityQueue()) {
                    minHeap->remove(it->first);
                    it->first->setInPriorityQueue(false);
                    it->first->setDistance(potentialNewDist);
                    it->first->setPreviousVertex(dijkstraCurrentVertex);
                    actionsList.push_back(Action(Command::UPDATEDINHEAP, it->first, nullptr, it->second, it->first->getDistance()));
                }
                else {
                    it->first->setDistance(potentialNewDist);
                    it->first->setPreviousVertex(dijkstraCurrentVertex);
                    actionsList.push_back(Action(Command::VERTEXUPDATE, it->first, nullptr, it->second, it->first->getDistance()));
                }
                minHeap->insert(it->first);
                it->first->setInPriorityQueue(true);
            }
            else {
                actionsList.push_back(Action(Command::VERTEXNOUPDATE, it->first, nullptr, it->second, it->first->getDistance()));
            }
        }
    }
    cursor = Cursor::CALCULATED;
    isCalculated = true;
    actionsList.push_back(Action(Command::CLEARALLVERTEX));
}
