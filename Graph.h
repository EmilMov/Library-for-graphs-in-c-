#ifndef GRAPH_H
#define GRAPH_H
 
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <limits>
 
template<typename T>
class Graph{
    protected:
        std::unordered_map<T,std::vector<T>> graph;
        size_t size;
        
        
        bool expand(std::queue<T>& q , std::unordered_set<T>& visitedthis , const std::unordered_set<T>& vistiedother , std::unordered_map<T,T>& parent , T& meetingpoiny) const;
    public:
        Graph();
        Graph(int n) = delete;
        Graph(const std::unordered_map<T,std::vector<T>>& gr);
//        Graph(const std::vector<std::vector<T>>& edges);
        Graph(std::initializer_list<std::pair<T, std::vector<T>>> ls);
//        Graph(std::initializer_list<std::pair<T, T>> ls);
//        Graph(const std::vector<T>& nodes);
        Graph(const Graph& other);
        Graph& operator=(const Graph& other);
        bool operator==(const Graph& other) const;
        Graph(Graph&& other) noexcept;
        Graph& operator=(Graph&& other) noexcept;
        Graph& operator=(std::initializer_list<std::pair<T, std::vector<T>>>& ls);
/*
Strongly Connected Components (Тарьян/Косараджу) — для направленных графов, более точная альтернатива твоему countComponents.
Bidirectional BFS — отложили на потом.*/
        // nayem paralel programing , 
        virtual void addedge(const T& a ,const T& b );
        virtual void removeedge(const T& a , const T& b);
        void addnode(const T& node);
        void removenode(const T& node);
        void clear();
        void printall() const;
        std::vector<T> BFS(T a,T b) const;
        std::vector<T> DFS(const T& a , const T& b) const;
        virtual bool loop_check() const;
        size_t getSize() const;
        size_t countComponents() const;
        bool hasEulerCycle() const;
        virtual std::vector<T> EulerCycle();
        std::vector<T> HamiltonCycle() const ;
        virtual std::vector<T> bidirectionalBFS(const T& a,const T& b) const;
        virtual ~Graph();
};

template <typename T>
class DirectedGraph : public Graph<T> {
    public:
    DirectedGraph();
    DirectedGraph(int n) = delete;
    DirectedGraph(const std::unordered_map<T,std::vector<T>>& gr);
    DirectedGraph(const std::vector<std::vector<T>>& edges);
    DirectedGraph(std::initializer_list<std::pair<T, std::vector<T>>> ls);
//    DirectedGraph(std::initializer_list<std::pair<T, T>> ls);

//    DirectedGraph(const std::vector<T>& nodes);
    DirectedGraph(const DirectedGraph& other);
    DirectedGraph& operator=(const DirectedGraph& other);
    bool operator==(const DirectedGraph& other) const;
    DirectedGraph(DirectedGraph&& other) noexcept;
    DirectedGraph& operator=(DirectedGraph&& other) noexcept;
    DirectedGraph& operator=(std::initializer_list<std::pair<T, std::vector<T>>> ls);
        
    void addedge(const T& a , const T& b) override ;
    void removeedge(const T& a , const T& b) override ;
    bool loop_check() const override;
    std::vector<T> EulerCycle() override;
    std::vector<std::vector<T>> stronglyConnectedComponents() const;
    std::vector<T> bidirectionalBFS(const T& a,const T& b) const override;
    DirectedGraph<T> getReverse() const;
    ~DirectedGraph() override;
    
    private:
    std::unordered_map<T, std::vector<T>> getTranspose() const;
    void fillOrder(const T& node, std::unordered_set<T>& visited, std::stack<T>& finishOrder) const ;
    void collectSCC(const T& node, const std::unordered_map<T, std::vector<T>>& transposed, std::unordered_set<T>& visited, std::vector<T>& component) const;

    
};

template<typename T , typename U = int>
class WeightedGraph{
    protected:
        std::unordered_map<T , std::unordered_map< T , U >> graph;
        size_t size ;
    public:
        WeightedGraph();
        WeightedGraph(int n) = delete;
        WeightedGraph(const std::unordered_map<T,std::unordered_map<T , U>>& gr);
//        Graph(const std::vector<std::vector<T>>& edges);
        WeightedGraph(std::initializer_list<std::pair<T, std::unordered_map<T , U>>> ls);
//        Graph(std::initializer_list<std::pair<T, T>> ls);
//        Graph(const std::vector<T>& nodes);
        WeightedGraph(const WeightedGraph& other);
        WeightedGraph& operator=(const WeightedGraph& other);
        bool operator==(const WeightedGraph& other) const;
        WeightedGraph(WeightedGraph&& other) noexcept;
        WeightedGraph& operator=(WeightedGraph&& other) noexcept;
        WeightedGraph<T,U>& operator=(std::initializer_list<std::pair<T, std::unordered_map<T,U>>> ls);

        virtual void addedge(const T& a ,const T& b , const U& weight);
        virtual void removeedge(const T& a , const T& b);
        void addnode(const T& node);
        void removenode(const T& node);
        void clear();
        void printall() const;
        std::vector<T> BFS(T a,T b) const;
        std::pair<U ,std::vector<T>> Dijkstra_algorithm(const T& a ,const T& b) const;
        std::vector<T> DFS(const T& a , const T& b) const;
        virtual bool loop_check() const;
        size_t getSize() const;
        virtual size_t countComponents() const;
        bool hasEulerCycle() const;
        virtual std::vector<T> EulerCycle() const;
        std::vector<T> HamiltonCycle() const;
        std::pair<U,std::vector<T>> Bellman_Ford(const T& a , const T& b) const;
        virtual std::vector<T> bidirectionalBFS(const T& a, const T& b) const;
        virtual ~WeightedGraph();
    protected:
        bool expand(std::queue<T>& q, std::unordered_set<T>& visitedthis,const std::unordered_set<T>& visitedother,std::unordered_map<T,T>& parent, T& meetingpoint) const;
};

template<typename T , typename U = int>
class DirectedWeightedGraph : public WeightedGraph<T,U> {
    public:
        DirectedWeightedGraph();
        DirectedWeightedGraph(int n) = delete;
        DirectedWeightedGraph(const std::unordered_map<T,std::unordered_map<T , U>>& gr);
        DirectedWeightedGraph(std::initializer_list<std::pair<T, std::unordered_map<T , U>>> ls);
        DirectedWeightedGraph(const DirectedWeightedGraph& other);
        DirectedWeightedGraph& operator=(const DirectedWeightedGraph& other);
        bool operator==(const DirectedWeightedGraph& other) const;
        DirectedWeightedGraph(DirectedWeightedGraph&& other) noexcept;
        DirectedWeightedGraph& operator=(DirectedWeightedGraph&& other) noexcept;
        DirectedWeightedGraph<T,U>& operator=(std::initializer_list<std::pair<T, std::unordered_map<T,U>>> ls);

        void addedge(const T& a ,const T& b , const U& weight) override;
        void removeedge(const T& a , const T& b) override;
        bool loop_check() const override;
        std::vector<T> EulerCycle() const override;
        virtual std::vector<T> bidirectionalBFS(const T& a, const T& b) const override;
        DirectedWeightedGraph<T,U> getReverse() const;
        size_t countComponents() const override;
        
        ~DirectedWeightedGraph() override;
        
    private:
        std::unordered_map<T, std::unordered_map<T, U>> getTranspose() const ;
        void fillOrder(const T& node, std::unordered_set<T>& visited, std::stack<T>& finishOrder) const;
        void collectSCC(const T& node, const std::unordered_map<T, std::unordered_map<T, U>>& transposed, std::unordered_set<T>& visited, std::vector<T>& component) const;

};

#include "Graph.cpp"
#include "Directed_Graph.cpp" 
#include "Weighted_Graph.cpp"
#include "Directed_Weighted_Graph.cpp"
#endif