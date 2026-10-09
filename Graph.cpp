#ifndef GRAPH_CPP
#define GRAPH_CPP
 
#include "Graph.h"
 
template<typename T>
Graph<T>::Graph():size(0){}
 
template<typename T>
Graph<T>::Graph(const std::unordered_map<T,std::vector<T>>& gr){
    
    for(const auto& pair : gr){
        addnode(pair.first);
        for(size_t i = 0 ; i < pair.second.size() ; i++){
            addedge(pair.first , pair.second[i]);
        }
    }
    size = graph.size();
}
/*
template<typename T>
Graph<T>::Graph(const std::vector<std::vector<T>>& edges){
    for(unsigned int i = 0 ; i < edges.size() ; i++){
        if(edges[i].size() != 2 ){
            throw std::invalid_argument("edge must have exactly 2 nodes");
        }
        addedge(edges[i][0], edges[i][1]);
    }
    size = graph.size();
}
*/
template<typename T>
Graph<T> :: Graph(std::initializer_list<std::pair<T, std::vector<T>>> ls){
    for(const auto& pair : ls){
        for(const auto& sec : pair.second){
            addedge(pair.first , sec);
        }
    }
}
/*
template<typename T>
Graph<T> :: Graph(std::initializer_list<std::pair<T, T>> ls){
    for(const auto& pair : ls){
        addedge(pair.first, pair.second);
    }
    size = graph.size();
}

template<typename T>
Graph<T>::Graph(const std::vector<T>& nodes) : size(0){
    for(unsigned int i = 0 ; i < nodes.size() ; i++){
        addnode(nodes[i]);
    }
}
*/
template<typename T>
Graph<T>::Graph(const Graph& other):graph(other.graph),size(other.size){}
 
template<typename T>
Graph<T>& Graph<T>::operator=(const Graph& other){
    if(this != &other){
        graph = other.graph;
        size = other.size;
    }
    return *this;
}
 
template<typename T>
bool Graph<T>::operator==(const Graph& other) const {
    return (this->graph == other.graph);
}
 
template<typename T>
Graph<T>::Graph(Graph&& other) noexcept: graph(std::move(other.graph)) , size(other.size) {
    other.size = 0;
}
 
template<typename T>
Graph<T>& Graph<T>::operator=(Graph&& other) noexcept {
    if (this != &other) {              
        graph = std::move(other.graph);
        size = other.size;
        other.size = 0;
    }
    return *this;
}

template<typename T>
Graph<T>& Graph<T>::operator=(std::initializer_list<std::pair<T, std::vector<T>>>& ls){
    clear();
    for(const auto& pair : ls){
        for(const auto& sec : pair.second){
            addedge(pair.first , sec);
        }
    }
    return *this;
}
 
template<typename T>
void Graph<T>::addedge(const T& a ,const T& b ){
    if(a == b){
        return;
    }
    addnode(a);
    addnode(b);
    for(unsigned int i = 0 ; i < graph[a].size() ; i++){
        if(graph[a][i] == b){
            return;
        }
    }
    graph[a].push_back(b);
    graph[b].push_back(a);
}
 
template<typename T>
void Graph<T>::removeedge(const T& a , const T& b){
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()) return;
    for(unsigned int i = 0 ; i < graph[a].size() ; i++){
        if(graph[a][i] == b){
            std::swap(graph[a][i] , graph[a][graph[a].size() - 1]);
            graph[a].pop_back();
            break;
        }
    }
    for(unsigned int i = 0 ; i < graph[b].size() ; i++){
        if(graph[b][i] == a){
            std::swap(graph[b][i] , graph[b][graph[b].size() - 1]);
            graph[b].pop_back();
            break;
        }
    }
}
 
template<typename T>
void Graph<T>::addnode(const T& node){
    if (graph.find(node) == graph.end()) {
        graph[node];
        size++;
    }
}
 
template<typename T>
void Graph<T>::removenode(const T& node){
    if(graph.find(node) == graph.end()) return;
 
    std::vector<T> neighbors = graph[node];
    for(const T& neighbor : neighbors){
        removeedge(node, neighbor);
    }
    graph.erase(node);
    size--;
}
 
template<typename T>
void Graph<T>::clear(){
    size = 0;
    graph.clear();
}
 
template<typename T>
void Graph<T>::printall() const {
    for (const auto& x : graph) {           
        std::cout << x.first << " : ";
        for (const auto& y : x.second) {
            std::cout << y << " ";
        }
        std::cout << std::endl;
    }
}
 
template<typename T>
std::vector<T> Graph<T>::BFS(T a,T b) const {
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()){ return {};}
    std::unordered_map<T,T> parent;
    std::unordered_map<T,bool> visited;
    std::queue<T> q;
    for(const auto& pair : graph){
        visited[pair.first] = false;
    }
    q.push(a);
    visited[a] = true;
    while(!q.empty()){
        T current = q.front();
        q.pop();
        
        if(current == b){
            std::vector<T> res;
            T node = b;
            while(node != a){
                res.push_back(node);
                node = parent[node];
            }
            res.push_back(a);
            std::reverse(res.begin() , res.end());
            return res;
        }
        const auto& neighbors = graph.at(current); 
        for(size_t i = 0 ; i < neighbors.size() ; i++){
            if(visited[neighbors[i]] == false){
                visited[neighbors[i]] = true;
                parent[neighbors[i]] = current;
                q.push(neighbors[i]);
            }
        }
    }
    return {};
}
 
template<typename T>
std::vector<T> Graph<T>::DFS(const T& a , const T& b) const {
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()){ return {} ; }
    std::unordered_set<T> visited;
    std::unordered_map<T,T> parent;
    std::stack<T> s;
    s.push(a);
    
    while(!s.empty()){
        T current = s.top();
        s.pop();
        
        if(visited.count(current)){ continue; }
        visited.insert(current);
        
        if(current == b){
            std::vector<T> res;
            T node = b;
            while(node != a){
                res.push_back(node);
                node = parent[node];
            }
            res.push_back(a);
            std::reverse(res.begin() , res.end());
            return res;
        }
        
        const auto& neighbors = graph.at(current);
        for(size_t i = 0 ; i < neighbors.size() ; i++){
            if(!visited.count(neighbors[i])){
                parent[neighbors[i]] = current;
                s.push(neighbors[i]);
            }
        }
    }
    return {};
}
 
template<typename T>
size_t Graph<T>::getSize() const { return size; }
 
template<typename T>
bool Graph<T>::loop_check() const{
    std::unordered_map<T,bool> visited;
    
    for(const auto& pair : graph){
        visited[pair.first] = false;
    }
    
    for(const auto& str : graph){
        if(visited[str.first]) continue;
        
        std::stack<std::pair<T,T>> s;
        s.push({str.first ,str.first});
        
        visited[str.first] = true;
        while(!s.empty()){
            T current = s.top().first; 
            T parent = s.top().second;
            s.pop();
            
            const auto& neighbours = graph.at(current);
            for(const T& neighbour : neighbours){
                if(!visited[neighbour]){
                    visited[neighbour] = true;
                    s.push({neighbour , current});
                }else if(neighbour != parent){
                    return true;
                }
            }
        }
    }
    return false;
}
template<typename T>
size_t Graph<T>::countComponents() const {
    std::unordered_map<T,bool> visited;
    for(const auto& pair : graph){
        visited[pair.first] =false;
    }
    size_t count = 0;
    for(const auto& pair : graph){
        if(visited[pair.first]) continue;
        count++;
        std::queue<T> q;
        q.push(pair.first);
        visited[pair.first] = true;
        while(!q.empty()){
            T current = q.front();
            q.pop();
            const auto& neighbors = graph.at(current);
            for(size_t i = 0 ; i < neighbors.size() ; i++){
                if(visited[neighbors[i]]) continue;
                    visited[neighbors[i]] = true;
                    q.push(neighbors[i]);
            }
        }
    }
    return count;
}

template<typename T>
bool Graph<T>::hasEulerCycle() const{
    if (!graph.empty() && countComponents() > 1) return false;  
    for(const auto& pair : graph){
        if(pair.second.size() % 2 != 0) return false;
    }
    return true;
}

template<typename T>
std::vector<T> Graph<T>::EulerCycle(){
    if(!hasEulerCycle()) return {};
    std::unordered_map<T,std::vector<T>> tmpgr = graph;
    
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
            T next = tmpgr[curr].back();
            tmpgr[curr].pop_back();
            
            auto& vec = tmpgr[next];
            for(size_t i = 0 ; i < vec.size() ; i++){
                if(vec[i] == curr){
                    std::swap(vec[i] , vec.back());
                    vec.pop_back();
                    break;
                }
            }
            s.push(next);
        } else {
            circuit.push_back(curr);
            s.pop();
        }
    }
    return circuit;
}

template<typename T>
std::vector<T> Graph<T>::HamiltonCycle() const {
    if(size < 3) return {};
    for(const auto pair : graph){
        if(pair.second.size() < 2) return {};
    }
    
    T start = graph.begin()->first;
    std::vector<T> path = {start};
    std::unordered_set<T> visited = {start};
    std::stack<size_t> idx;
    
    idx.push(0);
    
    while(!path.empty()){
        if(path.size() == size){
            for(const auto& neighbor : graph.at(path.back())){
                if(neighbor == start){
                    path.push_back(start);
                    return path;
                }
            }
        } else {
            const T curr = path.back();
            const std::vector<T>& neighbours = graph.at(curr);
            size_t ind = idx.top();
            bool moved = false;
            
            while(ind < neighbours.size()){
                if(!visited.count(neighbours[ind])){
                    idx.top() = ind + 1;
                    
                    path.push_back(neighbours[ind]);
                    visited.insert(neighbours[ind]);
                    idx.push(0);
                    
                    moved = true;
                    break;
                }
                ind++;
            }
            
            if(moved) continue;
        }
        visited.erase(path.back());
        path.pop_back();
        idx.pop();
    }
    return {};
}

template<typename T>
std::vector<T> Graph<T>::bidirectionalBFS(const T& a,const T& b) const {
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()) return {};
    if(a == b) return {a};
    
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
        found = expand(aq,avisited,bvisited,aparent,meetingpoint);
        if(found) break;
        
        found = expand(bq,bvisited,avisited,bparent,meetingpoint);
    }
    
    if(!found) return {};
    
    std::vector<T> apath;
    T node = meetingpoint;
    while(node != a){
        apath.push_back(node);
        node = aparent[node];
    }
    apath.push_back(a);
    std::reverse(apath.begin() , apath.end());
    
    std::vector<T> bpath;
    node = meetingpoint;
    while(node != b){
        node = bparent[node];
        bpath.push_back(node);
    }
    apath.insert(apath.end() , bpath.begin() , bpath.end());
    return apath;
    
    
}

template<typename T>
bool Graph<T>::expand(std::queue<T>& q , std::unordered_set<T>& visitedthis , const std::unordered_set<T>& vistiedother , std::unordered_map<T,T>& parent , T& meetingpoint) const{
    size_t sz = q.size();
    for(size_t i = 0 ; i < sz ; i++){
        T current = q.front();
        q.pop();
        for(const auto& neighbor : graph.at(current)){
            if(visitedthis.count(neighbor)) continue;
            visitedthis.insert(neighbor);
            parent[neighbor] = current;
            if(vistiedother.count(neighbor)){
              meetingpoint = neighbor;
              return true ;
            }
            q.push(neighbor);
        }
    }  
    return false;
}

template<typename T>
Graph<T>::~Graph(){}


#endif