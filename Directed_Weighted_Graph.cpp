#ifndef DIRECTED_WEIGHTED_GRAPH_CPP
#define DIRECTED_WEIGHTED_GRAPH_CPP

#include "Graph.h"

template<typename T, typename U>
DirectedWeightedGraph<T,U> :: DirectedWeightedGraph(){}

template<typename T, typename U>
DirectedWeightedGraph<T,U> :: DirectedWeightedGraph(const std::unordered_map<T,std::unordered_map<T, U>>& gr) {
	for(const auto& pair : gr) {
		this->addnode(pair.first);
		for(const auto& edg : pair.second) {
			this->addedge(pair.first, edg.first, edg.second);
		}
	}
}

template<typename T, typename U>
DirectedWeightedGraph<T,U> :: DirectedWeightedGraph(std::initializer_list<std::pair<T, std::unordered_map<T, U>>> ls) {
	for(const auto& pair : ls) {
		this->addnode(pair.first);
		for(const auto& edg : pair.second) {
			this->addedge(pair.first, edg.first, edg.second);
		}
	}
}

template<typename T, typename U>
DirectedWeightedGraph<T,U> :: DirectedWeightedGraph(const DirectedWeightedGraph& other) : WeightedGraph<T,U>(other) {}

template<typename T, typename U>
DirectedWeightedGraph<T,U>& DirectedWeightedGraph<T,U> :: operator=(const DirectedWeightedGraph& other) {
	if(this != &other) {
		this->graph = other.graph;
		this->size = other.size;
	}
	return *this;
}

template<typename T, typename U>
bool DirectedWeightedGraph<T,U> :: operator==(const DirectedWeightedGraph& other) const {
	return (this->graph == other.graph);
}

template<typename T, typename U>
DirectedWeightedGraph<T,U> :: DirectedWeightedGraph(DirectedWeightedGraph&& other) noexcept: WeightedGraph<T,U>(std::move(other)) {
    other.size = 0;
}

template<typename T, typename U>
DirectedWeightedGraph<T,U>& DirectedWeightedGraph<T,U> :: operator=(DirectedWeightedGraph&& other) noexcept {
	if (this != &other) {
		this->graph = std::move(other.graph);
		this->size = other.size;
		other.size = 0;
	}
	return *this;
}

template<typename T, typename U>
DirectedWeightedGraph<T,U>& DirectedWeightedGraph<T,U> :: operator=(std::initializer_list<std::pair<T, std::unordered_map<T,U>>> ls) {
	this->clear();
	for(const auto& pair : ls) {
		this->addnode(pair.first);
		for(const auto& edg : pair.second) {
			this->addedge(pair.first, edg.first, edg.second);
		}
	}
	return *this;
}

template<typename T, typename U>
void DirectedWeightedGraph<T,U> :: addedge(const T& a,const T& b, const U& weight) {
	this->addnode(a);
	this->addnode(b);
	this->graph[a][b] = weight;
}

template<typename T, typename U>
void DirectedWeightedGraph<T,U> :: removeedge(const T& a, const T& b){
     if(this->graph.find(a) == this->graph.end() || this->graph.find(b) == this->graph.end()) return ;
	this->graph[a].erase(b);
}

template<typename T, typename U>
bool DirectedWeightedGraph<T,U> :: loop_check() const {
	using NeighborIt = typename std::unordered_map<T,U>::const_iterator;

	std::unordered_map<T,int> color;
	for(const auto& pair : this->graph) {
		color[pair.first] = 0;
	}

	for(const auto& pair : this->graph) {
		if(color[pair.first] != 0) continue;
		
		std::stack<std::pair<T, NeighborIt>> s;
		const std::unordered_map<T,U>& startNeighbours = this->graph.at(pair.first);
		s.push({pair.first, startNeighbours.begin()});
        color[pair.first] = 1;

        while(!s.empty()){
            T node = s.top().first;
            NeighborIt& it = s.top().second;
            const std::unordered_map<T,U>& neighbours = this->graph.at(node);

            if(it != neighbours.end()){
                const T& neighbour = it->first;
                ++it;
                if(color[neighbour] == 1){
                    return true; 
                }
                if(color[neighbour] == 0){
                    const std::unordered_map<T,U>& nextNeighbours = this->graph.at(neighbour);
                    color[neighbour] = 1;
                    s.push({neighbour, nextNeighbours.begin()});
                }
            } else {
                color[node] = 2;
                s.pop();
            }
        }
	}
	return false;
}


template<typename T , typename U>
std::vector<T> DirectedWeightedGraph<T,U>::EulerCycle() const{
    
    std::unordered_map<T, int> outDegree, inDegree;
    for(const auto& pair : this->graph){
        outDegree[pair.first] += pair.second.size();
        for(const auto& edge : pair.second){
            inDegree[edge.first]++;
        }
    }
    
    for(const auto& pair : this->graph){
        if(outDegree[pair.first] != inDegree[pair.first]){
            return {};
        }
    }
    
    
    std::unordered_map< T , std::unordered_map < T , U >> tmpgr = this->graph;
    
    T start;
    bool foundstart = false;
    
    for(const auto& pair : tmpgr){
        if(!pair.second.empty()){
            start = pair.first;
            foundstart = true;
            break;
        }
    }
    
    if(!foundstart) return {};
    
    std::stack<T> s;
    std::vector<T> circuit;
    s.push(start);
    
    while(!s.empty()){
        T curr = s.top();
        
        if(!tmpgr[curr].empty()){
            T next = tmpgr[curr].begin()->first;
            tmpgr[curr].erase(next);
            s.push(next);
        }else{
            circuit.push_back(curr);
            s.pop();
        }
    }
    return circuit;
}

template<typename T, typename U>
std::unordered_map<T, std::unordered_map<T, U>> DirectedWeightedGraph<T, U>::getTranspose() const {
    std::unordered_map<T, std::unordered_map<T, U>> transposed;
    
    for(const auto& node: this->graph) {
        transposed[node.first];
        for(const auto& neighbor : node.second) {
            transposed[neighbor.first][node.first] = neighbor.second;
        }
    }
    
    return transposed;
}

template<typename T, typename U>
void DirectedWeightedGraph<T, U>::fillOrder(const T& node, std::unordered_set<T>& visited, std::stack<T>& finishOrder) const {
    visited.insert(node);
    
    if (this->graph.count(node)) {
        for(const auto& neighbor : this->graph.at(node)) {
            if(!visited.count(neighbor.first)) {
                fillOrder(neighbor.first, visited, finishOrder);
            }
        }
    }
    
    finishOrder.push(node);
}

template<typename T, typename U>
void DirectedWeightedGraph<T, U>::collectSCC(const T& node, const std::unordered_map<T, std::unordered_map<T, U>>& transposed, std::unordered_set<T>& visited, std::vector<T>& component) const {
    visited.insert(node);
    component.push_back(node);
    
    if (transposed.count(node)) {
        for(const auto& neighbor: transposed.at(node)) {
            if(!visited.count(neighbor.first)) {
                collectSCC(neighbor.first, transposed, visited, component);
            }
        }
    }
}

template<typename T, typename U>
std::vector<T> DirectedWeightedGraph<T,U>::bidirectionalBFS(const T& a,const T& b) const{
    if(this->graph.find(a) == this->graph.end() || this->graph.find(b) == this->graph.end()) return {};
    if(a == b) return {a};
    
    DirectedWeightedGraph<T,U> reversedGraphObj;
    reversedGraphObj.graph = getTranspose();
    reversedGraphObj.size = reversedGraphObj.graph.size();

    std::unordered_map<T,T> aparent;
    std::unordered_map<T,T> bparent;
    std::unordered_set<T> avisited = {a};
    std::unordered_set<T> bvisited = {b};
    std::queue<T> aq;
    std::queue<T> bq;
    
    aq.push(a);
    bq.push(b);
    
    T meetingpoint;
    bool found = false;
    
    while(!aq.empty() && !bq.empty() && !found){
        found = this->expand(aq,avisited,bvisited,aparent,meetingpoint);
        if(found) break;
        found = reversedGraphObj.expand(bq,bvisited,avisited,bparent,meetingpoint);
    }
    
    if(!found) return {};
    
    std::vector<T> apath;
    T node = meetingpoint;
    while(node != a){
        apath.push_back(node);
        node = aparent[node];
    }
    apath.push_back(a);
    std::reverse(apath.begin(), apath.end());
    
    node = meetingpoint;
    std::vector<T> bpath;
    while(node != b){
        node = bparent[node];
        bpath.push_back(node);
    }
    apath.insert(apath.end(), bpath.begin(), bpath.end());
    return apath;
}

template<typename T, typename U>
DirectedWeightedGraph<T,U> DirectedWeightedGraph<T,U>::getReverse() const {
    DirectedWeightedGraph<T,U> result;
    
    for(const auto& pair : this->graph){
        result.addnode(pair.first);
        for(const auto& neighbor : pair.second){
            result.addedge(neighbor.first, pair.first, neighbor.second);
        }
    }
    
    return result;
}

template<typename T , typename U>        
size_t DirectedWeightedGraph<T,U>::countComponents() const{
    std::unordered_map<T, std::vector<T>> undirected;
    for(const auto& pair : this->graph){
        undirected[pair.first]; 
        for(const auto& edge : pair.second){
            undirected[pair.first].push_back(edge.first);
            undirected[edge.first].push_back(pair.first); 
        }
    }
    
    std::unordered_map<T,bool> visited;
    for(const auto& pair : undirected){
        visited[pair.first] = false;
    }
    size_t count = 0;
    for(const auto& pair : undirected){
        if(visited[pair.first]) continue;
        count++;
        std::queue<T> q;
        q.push(pair.first);
        visited[pair.first] = true;
        while(!q.empty()){
            T node = q.front();
            q.pop();
            for(const T& neighbour : undirected.at(node)){
                if(visited[neighbour]) continue;
                visited[neighbour] = true;
                q.push(neighbour);
            }
        }
    }
    return count;
}

template<typename T , typename U >
DirectedWeightedGraph<T,U> :: ~DirectedWeightedGraph(){}

#endif