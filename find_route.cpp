//implemented in c++11 using vscode (wsl)
//find best route from given input
//modification of a shortest path algorithm

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

#define infinity 99999999 		//large number
#define max 500 				//for convenience; max number of vertices

typedef std::pair<int, int> int_pair;
int start_node;
int dest_node;

std::vector<int_pair> adjList[max];
std::vector<int> parent_node[max];
int global_weight[max];

std::vector<int> path_storage;

struct Node{
	int vertex, weight;
	Node(int vertex, int weight){
		this->vertex = vertex;
		this->weight = weight;
	}
	Node(){
		vertex = 0;
		weight = 0;
	}
	bool operator() (const Node& left, const Node& right){
		return left.weight > right.weight;
	}
};

void init(bool clear_adjList){
	for(int i=0; i<max; i++){
		if(clear_adjList) 
			adjList[i].clear();
			global_weight[i] = infinity;
			parent_node[i].clear();
	}
}

void find_route(){
	global_weight[start_node] = 0;
	std::priority_queue<Node, std::vector<Node>, Node> pq;
	pq.push(Node(start_node, 0));
	
	while(!pq.empty()){
		int current_weight = pq.top().weight;
		int current_vertex = pq.top().vertex;
		pq.pop();

		if(current_weight > global_weight[current_vertex]) continue;

		for(int i=0; i<(int)adjList[current_vertex].size(); i++){
			int next_weight = adjList[current_vertex][i].second;
			int next_vertex = adjList[current_vertex][i].first;
			if(global_weight[next_vertex] > current_weight + next_weight){
				global_weight[next_vertex] = current_weight + next_weight;
				pq.push(Node(next_vertex, current_weight + next_weight));
				
				parent_node[next_vertex].clear();
				parent_node[next_vertex].push_back(current_vertex);

			}

			else if(global_weight[next_vertex] == current_weight + next_weight){
				parent_node[next_vertex].push_back(current_vertex);
			}
		}
	}
}

void storepath(std::vector<int> const &prev, int i){
	if(i<0) return;
	storepath(prev, prev[i]);
	path_storage.push_back(i);
}

void find_route2(int x){
	global_weight[start_node] = 0;
	std::priority_queue<Node, std::vector<Node>, Node> pq;
	pq.push(Node(start_node, 0));
	std::vector<int> prev(x, -1);	
	std::vector<int> final_path;


	while(!pq.empty()){
		int current_weight = pq.top().weight;
		int current_vertex = pq.top().vertex;
		pq.pop();

		if(current_weight > global_weight[current_vertex]) continue;

		for(int i=0; i<(int)adjList[current_vertex].size(); i++){
			int next_weight = adjList[current_vertex][i].second;
			int next_vertex = adjList[current_vertex][i].first;
			if(global_weight[next_vertex] > (current_weight + next_weight)){
				global_weight[next_vertex] = current_weight + next_weight;
				pq.push(Node(next_vertex, current_weight + next_weight));

				prev[next_vertex] = current_vertex;

				parent_node[next_vertex].clear();
				parent_node[next_vertex].push_back(current_vertex);

			}

			else if(global_weight[next_vertex] == current_weight + next_weight){
				parent_node[next_vertex].push_back(current_vertex);

				prev[next_vertex] = current_vertex;
			}
		}
	}

	for(int i=0; i<x; i++){
		storepath(prev, i);
	}

	int i = path_storage.size() - 1;
	while(path_storage[i]!=start_node){
		final_path.insert(final_path.begin(), path_storage[i]);
		i--;
	}

	final_path.insert(final_path.begin(), start_node);
	if(final_path[final_path.size()-1]!=dest_node){
		final_path.push_back(dest_node);
	}

		if(global_weight[dest_node] == infinity){
		std::ofstream myfile;
		myfile.open("output.txt");
		myfile << -1;
		myfile << "\n";
		myfile << -1;
		myfile.close();
	}

	else{
		std::ofstream myfile;
		myfile.open("output.txt");
		myfile << global_weight[dest_node];
		myfile << "\n";
		for(int i=0; i<final_path.size(); i++){
			myfile << final_path[i] << " ";
		}
		myfile.close();
	}
}


int main(){
	
	std::ifstream infile("input.txt");
	int x, y;
	infile >> x >> y;
	init(true);
	infile >> start_node >> dest_node;
	for(int i=0; i<y; i++){
		int start, end, edge_weight;
		infile >> start >> end >> edge_weight;
		adjList[start].push_back(int_pair(end, edge_weight));
	}
	infile.close();

	//do routing
	find_route();

	//remove path included in the shortest path for finding the "best route"
	int current_vertex = dest_node;
	std::queue<int> storage;
	storage.push(current_vertex);

	while(!storage.empty()){
		int current_vertex = storage.front();
		storage.pop();

		for(int i=0; i<(int)parent_node[current_vertex].size(); i++){
			int end = current_vertex;
			int start = parent_node[current_vertex][i];

			if(start != start_node)
				storage.push(start);

			for(int i=0; i<(int)adjList[start].size(); i++){
				if(adjList[start][i].first == end){
					adjList[start].erase(adjList[start].begin()+i);
					break;

				}
			}
		}
	}

	//implement routing again
	init(false);

	find_route2(x);
}