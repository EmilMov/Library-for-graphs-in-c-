#ifndef WEIGHTED_GRAPH_CPP
#define WEIGHTED_GRAPH_CPP
 
#include "Graph.h"

template<typename T , typename U>
WeightedGraph<T,U>::WeightedGraph():size(0){}

template<typename T , typename U>
WeightedGraph<T,U>::WeightedGraph(const std::unordered_map<T,std::unordered_map<T , U>>& gr){
    for(const auto& pair : gr){
        addnode(pair.first);
        for(const auto& edg : pair.second){
            addedge(pair.first , edg.first , edg.second);
        }
    }
}

template<typename T , typename U>
WeightedGraph<T,U>::WeightedGraph(std::initializer_list<std::pair<T, std::unordered_map<T , U>>> ls){
    for(const auto& pair : ls){
        addnode(pair.first);
        for(const auto& edg : pair.second){
            addedge(pair.first , edg.first , edg.second);
        }
    }
}
    
template<typename T , typename U>
WeightedGraph<T,U>::WeightedGraph(const WeightedGraph& other): graph(other.graph) , size(other.size){}

template<typename T , typename U>
WeightedGraph<T,U>& WeightedGraph<T,U>::operator=(const WeightedGraph& other){
    if(this != &other){
        graph = other.graph;
        size = other.size;
    }
    return *this;
}

template<typename T , typename U>
bool WeightedGraph<T,U>::operator==(const WeightedGraph& other) const{
    return (this->graph == other.graph);
}

template<typename T , typename U>
WeightedGraph<T,U>::WeightedGraph(WeightedGraph&& other) noexcept : graph(std::move(other.graph)) , size(other.size){}

template<typename T , typename U>
WeightedGraph<T,U>& WeightedGraph<T,U>::operator=(WeightedGraph&& other) noexcept{
    if (this != &other) {              
        graph = std::move(other.graph);
        size = other.size;
        other.size = 0;
    }
    return *this;
}

template<typename T , typename U>
WeightedGraph<T,U>& WeightedGraph<T,U>::operator=(std::initializer_list<std::pair<T, std::unordered_map<T,U>>> ls){
    clear();
    for(const auto& pair : ls){
        addnode(pair.first);
        for(const auto& edg : pair.second){
            addedge(pair.first, edg.first, edg.second);
        }
    }
    return *this;
}

template<typename T , typename U>
void WeightedGraph<T,U>::addedge(const T& a ,const T& b , const U& weight){
    addnode(a);
    addnode(b);
    graph[a][b] = weight; 
    graph[b][a] = weight;
}

template<typename T , typename U>
void WeightedGraph<T,U>::removeedge(const T& a , const T& b){
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()) return;
    graph[a].erase(b);
    graph[b].erase(a);
}

template<typename T , typename U>
void WeightedGraph<T,U>::addnode(const T& node){
    if (graph.find(node) == graph.end()) {
        graph[node];
        size++;
    }
}

template<typename T , typename U>
void WeightedGraph<T,U>::removenode(const T& node){
    if(graph.find(node) == graph.end()) return;
    
    std::vector<T> neighborsCopy;
    for(const auto& pair : graph[node]){
        neighborsCopy.push_back(pair.first);
    }
    for(const T& neighbor : neighborsCopy){
        removeedge(node, neighbor);
    }
    graph.erase(node);
    size--;
}

template<typename T , typename U>
void WeightedGraph<T,U>::clear(){
    size = 0;
    graph.clear();
}

template<typename T , typename U>
void WeightedGraph<T,U>::printall() const{
    for(const auto& pair : graph){
        std::cout << pair.first <<" : ";
        for(const auto& edg : pair.second){
            std::cout << edg.first <<" ( " << edg.second << " )  "; 
        }
        std::cout << std::endl;
    }
}

template<typename T , typename U>
size_t WeightedGraph<T,U>::getSize() const{ return size;}

template<typename T , typename U>
std::pair<U ,std::vector<T>> WeightedGraph<T,U>::Dijkstra_algorithm(const T& a ,const T& b) const {
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()){
        return {U{}, {}};
    }
    std::priority_queue<std::pair<U,T> , std::vector<std::pair<U,T>> , std::greater<std::pair<U,T>>> pq;
    std::unordered_map<T,U> dist;
    std::unordered_map<T,T> parent;
    
    for(const auto& pair : graph){
        dist[pair.first] = std::numeric_limits<U>::max();
    }
    dist[a] = U{};
    pq.push({U{}, a});
    
    while(!pq.empty()){
        U d = pq.top().first;
        T current = pq.top().second;
        pq.pop();
        
        if(d > dist[current]) continue;
        
        for(const auto& pair : graph.at(current)){
            T neighbour = pair.first;
            U weight = pair.second;
            U newweight = d + weight;
            if(newweight < dist[neighbour]){
                dist[neighbour] = newweight;
                parent[neighbour] = current;
                pq.push({newweight, neighbour});
            }
        }
    }
    if(dist[b] == std::numeric_limits<U>::max()){
        return {U{}, {}};
    }
    std::vector<T> path;
    T node = b;
    while(node != a){
        path.push_back(node);
        node = parent[node];
    }
    path.push_back(a);
    std::reverse(path.begin(), path.end());
    return {dist[b] , path };
    
}

template<typename T , typename U>
std::vector<T> WeightedGraph<T,U>::BFS(T a,T b) const{
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()){ return {}; }
    std::unordered_map<T,T> parent;
    std::unordered_map<T,bool> visited;
    std::queue<T> q;
    
    for(auto pair : graph){
        visited[pair.first] = false;
    }
    
    q.push(a);
    visited[a] = true;
    
    while(!q.empty()){
        T node = q.front();
        q.pop();
        
        if(node == b){
            std::vector<T> res ;
            T tmp = b;
            while(tmp != a){
                res.push_back(tmp);
                tmp = parent[tmp];
            }
            res.push_back(a);
            std::reverse(res.begin() , res.end());
            return res;
        }
        
        const auto& neighbours = graph.at(node);
        for(const auto& pair : neighbours){
            T neighbour = pair.first;
            U weight = pair.second;
            if(!visited[neighbour]){
                visited[neighbour] = true;
                parent[neighbour] = node;
                q.push(neighbour);
            }
        }
    }
    return {};
}

template<typename T , typename U>
std::vector<T> WeightedGraph<T,U>::DFS(const T& a , const T& b) const{
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()){ return {}; }
    
    std::unordered_map<T,T> parent;
    std::unordered_set<T> visited;
    std::stack<T> s;
    
    s.push(a);
    
    while(!s.empty()){
        T curr = s.top();
        s.pop();
        
        if(visited.find(curr) != visited.end()) continue;
        
        visited.insert(curr);
        
        if(curr == b){
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
        
        const auto& neighbours = graph.at(curr);
        for(const auto& pair : neighbours){
            T neighbour = pair.first;
            U weight = pair.second;
            if(visited.find(neighbour) == visited.end()){
                parent[neighbour] = curr;
                s.push(neighbour);
            }
        }
    }
    return {};
}

template<typename T , typename U>
bool WeightedGraph<T,U>::loop_check() const{
    std::unordered_map<T,bool> visited;
    
    for(const auto& pair : graph){
        visited[pair.first] = false;
    }
    
    for(const auto& pair : graph){
        if(visited[pair.first]) continue;
        
        std::stack<std::pair<T,T>> s;
        s.push({pair.first , pair.first});
        visited[pair.first] = true;
        
        while(!s.empty()){
            T curr = s.top().first;
            T parent = s.top().second;
            s.pop();
            const auto& neighbours = graph.at(curr);
            for(const auto& pair : neighbours){
                T neighbour = pair.first;
                U weight = pair.second;
                if(!visited[neighbour]){
                    visited[neighbour] = true;
                    s.push({neighbour , curr});
                }else if(neighbour != parent){
                    return true;
                }
            }
        }
    }
    return false;
}

template<typename T , typename U>        
size_t WeightedGraph<T,U>::countComponents() const{
    std::unordered_map<T,bool> visited;
    for(const auto& pair : graph){
        visited[pair.first] = false;
    }
    size_t count = 0;
    for(const auto& pair : graph){
        if(visited[pair.first]) continue;
        
        count++;
        
        std::queue<T> q;
        q.push(pair.first);
        visited[pair.first] = true;
        
        while(!q.empty()){
            T node = q.front();
            q.pop();
            
            const auto& neighbours = graph.at(node);
            for(const auto& pair : neighbours){
                T neighbour = pair.first;
                U weight = pair.second;
                if(visited[neighbour]) continue;
                visited[neighbour] = true;
                q.push(neighbour);
            }
        }
    }
    return count;
}

template<typename T , typename U>
bool WeightedGraph<T,U>::hasEulerCycle() const{
    if (!graph.empty() && countComponents() > 1) return false;
    for(const auto& pair : graph){
        if(pair.second.size() % 2 != 0) return false;
    }
    return true;
}

template<typename T , typename U>
std::vector<T> WeightedGraph<T,U>::EulerCycle() const{
    if(!hasEulerCycle()) return {};
    
    std::unordered_map< T , std::unordered_map < T , U >> tmpgr = graph;
    
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
            tmpgr[next].erase(curr);
            s.push(next);
        }else{
            circuit.push_back(curr);
            s.pop();
        }
    }
    return circuit;
}

template<typename T ,typename U>
std::vector<T> WeightedGraph<T,U>::HamiltonCycle() const {
    if(size < 3) return {};
    for(const auto& pair : graph){
        if(pair.second.size() < 2) return {};
    }
    
    T start = graph.begin()->first;
    std::vector<T> path = {start};
    std::unordered_set<T> visited = {start};
    using It = typename std::unordered_map<T,U>::const_iterator;
    std::stack<It> idx;
    idx.push(graph.at(start).begin());
    
    while(!path.empty()){
        if(path.size() == size){
            const auto& neighbors = graph.at(path.back());
            bool canClose = false;
            for(const auto& neighbor : neighbors){
                if(neighbor.first == start){
                    canClose = true;
                    break;
                }
            }
            if(canClose){
                path.push_back(start);
                return path;
            }
        }else{
            const T& curr = path.back();
            const std::unordered_map<T,U>& neighbors = graph.at(curr);
            It neighbor = idx.top();
            bool moved = false;
            
            while(neighbor != neighbors.end()){
                const T& current = neighbor->first;
                neighbor++;
                
                if(!visited.count(current)){
                    idx.top() = neighbor;
                    
                    path.push_back(current);
                    visited.insert(current);
                    idx.push(graph.at(current).begin());
                    
                    moved = true;
                    break;
                }
               
            }

            if(moved) continue;
            
        }
        visited.erase(path.back());
        path.pop_back();
        idx.pop();
    }
    return {};
}

template<typename T ,typename U>
std::pair<U,std::vector<T>>  WeightedGraph<T,U>::Bellman_Ford(const T& a , const T& b) const {
    if(graph.find(a) == graph.end() || graph.find(b) == graph.end()){ return { U{} , {} } ;}
    
    std::unordered_map<T,U> dist;
    std::unordered_map<T,T> parent;
    
    for(const auto& pair : graph){
        dist[pair.first] = std::numeric_limits<U>::max();
    }
    
    dist[a] = U{};
    
    for(size_t i = 0 ; i < size - 1 ; i ++){
        bool update = false;
        
        for(const auto& frompair : graph){
            const T& from = frompair.first;
            if(dist[from] == std::numeric_limits<U>::max()) continue;
            
            for(const auto& topair : frompair.second){
                const T& to = topair.first;
                U weight = topair.second;
                
                if(dist[from] + weight < dist[to]){
                    dist[to] = dist[from] + weight;
                    parent[to] = from;
                    update = true;
                }
            }
        }
        
        if(!update) break;
    }
    
    for(const auto& frompair : graph){
        const T& from = frompair.first;
        if(dist[from ] == std::numeric_limits<U>::max()) continue;
        
        for(const auto& topair : frompair.second){
            const T& to = topair.first;
            U weight = topair.second;
            
            if(dist[from] + weight < dist[to]){
                throw std::runtime_error("graph contains a negative-weight cycle");
            }
        }
    }
    
    if(dist[b] == std::numeric_limits<U>::max()){
        return { U{},{}};
    }
    std::vector<T> path;
    T node = b;
    while(node != a){
        path.push_back(node);
        node = parent[node];
    }
    path.push_back(a);
    std::reverse(path.begin() , path.end());
    return {dist[b] , path };
}

template<typename T, typename U>
std::vector<T> WeightedGraph<T,U>::bidirectionalBFS(const T& a, const T& b) const {
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
        found = expand(aq, avisited, bvisited, aparent, meetingpoint);
        if(found) break;
        found = expand(bq, bvisited, avisited, bparent, meetingpoint);
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
    
    std::vector<T> bpath;
    node = meetingpoint;
    while(node != b){
        node = bparent[node];
        bpath.push_back(node);
    }
    apath.insert(apath.end(), bpath.begin(), bpath.end());
    return apath;
}

template<typename T, typename U>
bool WeightedGraph<T,U>::expand(std::queue<T>& q, std::unordered_set<T>& visitedthis,const std::unordered_set<T>& visitedother,std::unordered_map<T,T>& parent, T& meetingpoint) const{
    size_t sz = q.size();
    for(size_t i = 0 ; i < sz ; i++){
        T current = q.front();
        q.pop();
        for(const auto& neighbor : graph.at(current)){
            if(visitedthis.count(neighbor.first)) continue;
            visitedthis.insert(neighbor.first);
            parent[neighbor.first] = current;
            if(visitedother.count(neighbor.first)){
              meetingpoint = neighbor.first;
              return true;
            }
            q.push(neighbor.first);
        }
    }  
    return false;
}

template<typename T , typename U>
WeightedGraph<T,U>::~WeightedGraph(){}

#endif