#ifndef DIRECTED_GRAPH_CPP
#define DIRECTED_GRAPH_CPP
 
#include "Graph.h"

template<typename T>
DirectedGraph<T> :: DirectedGraph(){}

template<typename T> 
DirectedGraph<T> :: DirectedGraph(const std::unordered_map<T,std::vector<T>>& gr){
    for(const auto& pair : gr){
        this->addnode(pair.first);
        for(size_t i = 0 ; i < pair.second.size() ; i++){
            this->addedge(pair.first , pair.second[i]);
        }
    }
    this->size = this->graph.size();
}
/*
template<typename T>
DirectedGraph<T> :: DirectedGraph(const std::vector<std::vector<T>>& edges){
    for(unsigned int i = 0 ; i < edges.size() ; i++){
        if(edges[i].size() != 2 ){
            throw std::invalid_argument("edge must have exactly 2 nodes");
        }
        this->addedge(edges[i][0], edges[i][1]);
    }
    this->size = this->graph.size();
}
*/
template<typename T>
DirectedGraph<T> :: DirectedGraph(std::initializer_list<std::pair<T, std::vector<T>>> ls){
    for(const auto& pair : ls){
        for(const auto& sec : pair.second){
            addedge(pair.first , sec);
        }
    }
}
/*
template<typename T>
DirectedGraph<T> :: DirectedGraph(std::initializer_list<std::pair<T, T>> ls){
    for(const auto& pair : ls){
        this->addedge(pair.first, pair.second);
    }
    this->size = this->graph.size();
}

template<typename T>
DirectedGraph<T> :: DirectedGraph(const std::vector<T>& nodes){
    for(unsigned int i = 0 ; i < nodes.size() ; i++){
        this->addnode(nodes[i]);
    }
}
*/   
template<typename T>
DirectedGraph<T> :: DirectedGraph(const DirectedGraph& other) : Graph<T>(other){}
    
template<typename T>
DirectedGraph<T>& DirectedGraph<T> ::operator=(const DirectedGraph& other){
    if(this != &other){
        this->graph = other.graph;
        this->size = other.size;
    }
    return *this;
}
    
template<typename T>
bool DirectedGraph<T> :: operator==(const DirectedGraph& other) const{
    return (this->graph == other.graph);
}

template<typename T>
DirectedGraph<T>::DirectedGraph(DirectedGraph&& other) noexcept : Graph<T>(std::move(other)){
    other.size = 0;
}

template<typename T>
DirectedGraph<T>& DirectedGraph<T> :: operator=(DirectedGraph&& other) noexcept{
    if (this != &other) {              
        this->graph = std::move(other.graph);
        this->size = other.size;
        other.size = 0;
    }
    return *this;
}

template<typename T>
DirectedGraph<T>& DirectedGraph<T>::operator=(std::initializer_list<std::pair<T, std::vector<T>>> ls){
    this->clear();
    for(const auto& pair : ls){
        for(const auto& sec : pair.second){
            addedge(pair.first , sec);
        }
    }
    return *this;
}
    
template<typename T>
void DirectedGraph<T>::addedge(const T& a , const T& b) {
    if(a == b) { return ; }
    this->addnode(a);
    this->addnode(b);
    this->graph[a].push_back(b);
}

template<typename T>
void DirectedGraph<T>::removeedge(const T& a , const T& b){
     if(this->graph.find(a) == this->graph.end() || this->graph.find(b) == this->graph.end()) return ;
    for(unsigned int i = 0 ; i < this->graph[a].size() ; i++){
        if(this->graph[a][i] == b){
            std::swap(this->graph[a][i] , this->graph[a][this->graph[a].size() - 1]);
            this->graph[a].pop_back();
            break;
        }
    }
}

template<typename T>
bool DirectedGraph<T>::loop_check() const {
    std::unordered_map<T,int> color;
    for(const auto& pair : this->graph){
        color[pair.first] = 0;
    }
    for(const auto& pair : this->graph){
        if(color[pair.first] != 0) continue;
        
        std::stack<std::pair<T ,size_t>> s;
        s.push({pair.first , 0});
        color[pair.first] = 1;
        
        while(!s.empty()){
            T& node = s.top().first;
            size_t& idx = s.top().second;
            const auto& neighbours = this->graph.at(node);
            
            if(idx < neighbours.size()){
                const T& neighbour = neighbours[idx];
                idx++;
                
                if(color[neighbour] == 1) return true;
                
                if(color[neighbour] == 0){
                    color[neighbour] = 1;
                    s.push({neighbour, 0});
                }
            }else{
                color[node] = 2;
                s.pop();
            }
        }
    }
    return false;
}   

template<typename T>
std::vector<T> DirectedGraph<T>::EulerCycle(){
    std::unordered_map<T,std::vector<T>> tmpgr = this->graph;
    
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
        s.pop();
        
        if(!tmpgr[curr].empty()){
            T next = tmpgr[curr].back();
            tmpgr[curr].pop_back();
            s.push(next);
        } else {
            circuit.push_back(curr);
            s.pop();
        }
    }
    std::reverse(circuit.begin() , circuit.end());
    return circuit;
}

template<typename T>
void DirectedGraph<T>::fillOrder(const T& node, std::unordered_set<T>& visited, std::stack<T>& finishOrder) const {
    visited.insert(node);
    
    for(const auto& neighbor : this->graph.at(node)){
        if(!visited.count(neighbor)){
            fillOrder(neighbor, visited, finishOrder);
        }
    }
    
    finishOrder.push(node);
}

template<typename T>
std::unordered_map<T, std::vector<T>> DirectedGraph<T>::getTranspose() const{
    std::unordered_map<T,std::vector<T>> transpose;
    
    for(const auto& neighbor : this->graph){
        transpose[neighbor.first];
        for(const auto& node : neighbor.second){
            transpose[node].push_back(neighbor.first);
        }
    }
    
    return transpose;
}

template<typename T>
void DirectedGraph<T>::collectSCC(const T& node, const std::unordered_map<T, std::vector<T>>& transposed, std::unordered_set<T>& visited, std::vector<T>& component) const {
    visited.insert(node);
    component.push_back(node);
    
    for(const auto& neighbor : transposed.at(node)){
        if(!visited.count(neighbor)){
            collectSCC(neighbor, transposed, visited, component);
        }
    }
}
 
template<typename T>
std::vector<std::vector<T>> DirectedGraph<T>::stronglyConnectedComponents() const {
    std::stack<T> finishOrder;
    std::unordered_set<T> visited;
    std::vector<std::vector<T>> result;
    
    for(const auto& pair : this->graph){
        if(!visited.count(pair.first)){
            fillOrder(pair.first, visited, finishOrder);
        }
    }
    
    std::unordered_map<T , std::vector<T>> transposed = getTranspose();
    
    visited.clear();
    
    while(!finishOrder.empty()){
        T node = finishOrder.top();
        finishOrder.pop();
        
        if(!visited.count(node)){
            std::vector<T> component;
            collectSCC(node, transposed, visited, component);
            result.push_back(component);
        }
    }
    return result;
}

template<typename T>
std::vector<T> DirectedGraph<T>::bidirectionalBFS(const T& a,const T& b) const {
    if(this->graph.find(a) == this->graph.end() || this->graph.find(b) == this->graph.end()) return {};
    if(a == b) return {a};
    
    DirectedGraph<T> reversedGraphObj;
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

template<typename T>
DirectedGraph<T> DirectedGraph<T>::getReverse() const{
    DirectedGraph<T> result;
    
    for(const auto& pair : this->graph){
        result.addnode(pair.first);
        for(const auto& neighbor : pair.second){
            result.addedge(neighbor, pair.first);
        }
    }
    
    return result;
}
    

template<typename T>
DirectedGraph<T>::~DirectedGraph(){}

#endif