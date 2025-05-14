#include <iostream>
#include <stack>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// Orijinal koddan alınan sabitler ve yapılar
const int MAX = 10;

struct Edge {
    int to;
    Edge* next;
};

struct Vertex {
    char name;
    bool visited;
    Edge* adj;
};

// Test için gerekli global değişkenler (orijinal koddan alındı)
Vertex testGraph[MAX];
Vertex testTranspose[MAX];
stack<int> testFinishStack;

// Orijinal koddan alınan fonksiyonlar
void addVertex(Vertex graph[], char name) {
    for (int i = 0; i < MAX; ++i) {
        if (graph[i].name == '0') {
            graph[i].name = name;
            graph[i].visited = false;
            graph[i].adj = nullptr;
            break;
        }
    }
}

int getIndex(Vertex graph[], char name) {
    for (int i = 0; i < MAX; ++i) {
        if (graph[i].name == name) return i;
    }
    return -1;
}

void addEdge(Vertex graph[], char from, char to) {
    int fromIdx = getIndex(graph, from);
    int toIdx = getIndex(graph, to);
    Edge* edge = new Edge;
    edge->to = toIdx;
    edge->next = graph[fromIdx].adj;
    graph[fromIdx].adj = edge;
}

void dfs1(Vertex graph[], int idx) {
    graph[idx].visited = true;
    for (Edge* e = graph[idx].adj; e != nullptr; e = e->next) {
        if (!graph[e->to].visited) {
            dfs1(graph, e->to);
        }
    }
    testFinishStack.push(idx);
}

void dfs2(Vertex graph[], int idx, vector<char>& component) {
    graph[idx].visited = true;
    component.push_back(graph[idx].name);
    for (Edge* e = graph[idx].adj; e != nullptr; e = e->next) {
        if (!graph[e->to].visited) {
            dfs2(graph, e->to, component);
        }
    }
}

// Test için modifiye edilmiş Kosaraju algoritması - sonuçları bir vector olarak döndürür
vector<vector<char>> kosaraju_test() {
    // testFinishStack'i temizle (başka testlerden kalıntı olabilir)
    while (!testFinishStack.empty()) {
        testFinishStack.pop();
    }
    
    vector<vector<char>> result;
    
    for (int i = 0; i < MAX; ++i) {
        if (testGraph[i].name != '0' && !testGraph[i].visited) {
            dfs1(testGraph, i);
        }
    }

    for (int i = 0; i < MAX; ++i) {
        testTranspose[i].visited = false;
    }

    while (!testFinishStack.empty()) {
        int idx = testFinishStack.top();
        testFinishStack.pop();
        if (!testTranspose[idx].visited && testTranspose[idx].name != '0') {
            vector<char> component;
            dfs2(testTranspose, idx, component);
            result.push_back(component);
        }
    }
    
    return result;
}

// Graf ve transpose'u temizleme fonksiyonu
void resetGraphs() {
    // Mevcut kenarları temizle
    for (int i = 0; i < MAX; ++i) {
        Edge* current = testGraph[i].adj;
        while (current) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
        testGraph[i].adj = nullptr;
        
        current = testTranspose[i].adj;
        while (current) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
        testTranspose[i].adj = nullptr;
    }
    
    // Düğümleri sıfırla
    for (int i = 0; i < MAX; ++i) {
        testGraph[i].name = '0';
        testGraph[i].visited = false;
        testTranspose[i].name = '0';
        testTranspose[i].visited = false;
    }
}

// SCC'leri sıralamak için yardımcı fonksiyon
vector<vector<char>> sortSCCs(vector<vector<char>> sccs) {
    // Her bir SCC'nin içindeki elemanları sırala
    for (auto& scc : sccs) {
        sort(scc.begin(), scc.end());
    }
    
    // SCC'leri de sırala (karşılaştırmaları kolaylaştırmak için)
    sort(sccs.begin(), sccs.end(), [](const vector<char>& a, const vector<char>& b) {
        return a[0] < b[0];
    });
    
    return sccs;
}

// Test sonuçlarını yazdırmak için fonksiyon
void printSCCs(const vector<vector<char>>& sccs) {
    cout << "Strongly Connected Components:" << endl;
    for (const auto& component : sccs) {
        cout << "SCC: ";
        for (char v : component) {
            cout << v << " ";
        }
        cout << endl;
    }
}

// Karşılaştırma için yardımcı fonksiyon
bool compareSCCs(const vector<vector<char>>& actual, const vector<vector<char>>& expected) {
    if (actual.size() != expected.size()) {
        return false;
    }
    
    auto sortedActual = sortSCCs(actual);
    auto sortedExpected = sortSCCs(expected);
    
    for (size_t i = 0; i < sortedActual.size(); ++i) {
        if (sortedActual[i] != sortedExpected[i]) {
            return false;
        }
    }
    
    return true;
}

// Test 1: Orijinal örnekteki graf
void testOriginalGraph() {
    cout << "Test 1: Orijinal Graf Testi" << endl;
    
    resetGraphs();
    
    // Düğümleri ekle
    for (char c = 'a'; c <= 'h'; ++c) {
        addVertex(testGraph, c);
        addVertex(testTranspose, c);
    }
    
    // Normal graf bağlantıları
    addEdge(testGraph, 'a', 'b');
    addEdge(testGraph, 'b', 'e');
    addEdge(testGraph, 'e', 'a');
    addEdge(testGraph, 'b', 'f');
    addEdge(testGraph, 'b', 'c');
    addEdge(testGraph, 'c', 'd');
    addEdge(testGraph, 'd', 'c');
    addEdge(testGraph, 'd', 'h');
    addEdge(testGraph, 'h', 'h');
    addEdge(testGraph, 'g', 'h');
    addEdge(testGraph, 'f', 'g');
    addEdge(testGraph, 'g', 'f');
    addEdge(testGraph, 'e', 'f');
    
    // Transpose graf bağlantıları
    addEdge(testTranspose, 'b', 'a');
    addEdge(testTranspose, 'e', 'b');
    addEdge(testTranspose, 'a', 'e');
    addEdge(testTranspose, 'f', 'b');
    addEdge(testTranspose, 'c', 'b');
    addEdge(testTranspose, 'd', 'c');
    addEdge(testTranspose, 'c', 'd');
    addEdge(testTranspose, 'h', 'd');
    addEdge(testTranspose, 'h', 'h');
    addEdge(testTranspose, 'h', 'g');
    addEdge(testTranspose, 'g', 'f');
    addEdge(testTranspose, 'f', 'g');
    addEdge(testTranspose, 'f', 'e');
    
    // Kosaraju algoritmasını çalıştır
    vector<vector<char>> result = kosaraju_test();
    
    // Beklenen SCC'ler: {a,b,e}, {c,d}, {f,g}, {h}
    vector<vector<char>> expected = {
        {'a', 'b', 'e'},
        {'c', 'd'},
        {'f', 'g'},
        {'h'}
    };
    
    // Sonuçları göster
    cout << "Bulunan SCC'ler:" << endl;
    printSCCs(result);
    
    cout << "Beklenen SCC'ler:" << endl;
    printSCCs(expected);
    
    // Sonuçları karşılaştır
    if (compareSCCs(result, expected)) {
        cout << "Test 1 BAŞARILI" << endl;
    } else {
        cout << "Test 1 BAŞARISIZ" << endl;
    }
    cout << endl;
}

// Test 2: Tek bir güçlü bağlı bileşen
void testSingleSCC() {
    cout << "Test 2: Tek SCC Testi" << endl;
    
    resetGraphs();
    
    // Düğümleri ekle
    for (char c = 'a'; c <= 'd'; ++c) {
        addVertex(testGraph, c);
        addVertex(testTranspose, c);
    }
    
    // Normal graf bağlantıları - tam döngü
    addEdge(testGraph, 'a', 'b');
    addEdge(testGraph, 'b', 'c');
    addEdge(testGraph, 'c', 'd');
    addEdge(testGraph, 'd', 'a');
    
    // Transpose graf bağlantıları
    addEdge(testTranspose, 'b', 'a');
    addEdge(testTranspose, 'c', 'b');
    addEdge(testTranspose, 'd', 'c');
    addEdge(testTranspose, 'a', 'd');
    
    // Kosaraju algoritmasını çalıştır
    vector<vector<char>> result = kosaraju_test();
    
    // Beklenen SCC: {a,b,c,d}
    vector<vector<char>> expected = {
        {'a', 'b', 'c', 'd'}
    };
    
    // Sonuçları göster
    cout << "Bulunan SCC'ler:" << endl;
    printSCCs(result);
    
    cout << "Beklenen SCC'ler:" << endl;
    printSCCs(expected);
    
    // Sonuçları karşılaştır
    if (compareSCCs(result, expected)) {
        cout << "Test 2 BAŞARILI" << endl;
    } else {
        cout << "Test 2 BAŞARISIZ" << endl;
    }
    cout << endl;
}

// Test 3: Bağlantısız graf
void testDisconnectedGraph() {
    cout << "Test 3: Bağlantısız Graf Testi" << endl;
    
    resetGraphs();
    
    // Düğümleri ekle
    for (char c = 'a'; c <= 'f'; ++c) {
        addVertex(testGraph, c);
        addVertex(testTranspose, c);
    }
    
    // İki bağlantısız alt graf oluştur
    // Alt graf 1: a -> b -> c -> a
    addEdge(testGraph, 'a', 'b');
    addEdge(testGraph, 'b', 'c');
    addEdge(testGraph, 'c', 'a');
    
    // Alt graf 2: d -> e -> f -> d
    addEdge(testGraph, 'd', 'e');
    addEdge(testGraph, 'e', 'f');
    addEdge(testGraph, 'f', 'd');
    
    // Transpose graf bağlantıları
    // Alt graf 1
    addEdge(testTranspose, 'b', 'a');
    addEdge(testTranspose, 'c', 'b');
    addEdge(testTranspose, 'a', 'c');
    
    // Alt graf 2
    addEdge(testTranspose, 'e', 'd');
    addEdge(testTranspose, 'f', 'e');
    addEdge(testTranspose, 'd', 'f');
    
    // Kosaraju algoritmasını çalıştır
    vector<vector<char>> result = kosaraju_test();
    
    // Beklenen SCC'ler: {a,b,c}, {d,e,f}
    vector<vector<char>> expected = {
        {'a', 'b', 'c'},
        {'d', 'e', 'f'}
    };
    
    // Sonuçları göster
    cout << "Bulunan SCC'ler:" << endl;
    printSCCs(result);
    
    cout << "Beklenen SCC'ler:" << endl;
    printSCCs(expected);
    
    // Sonuçları karşılaştır
    if (compareSCCs(result, expected)) {
        cout << "Test 3 BAŞARILI" << endl;
    } else {
        cout << "Test 3 BAŞARISIZ" << endl;
    }
    cout << endl;
}

// Test 4: DAG (Directed Acyclic Graph) - Hiç döngü olmayan graf
void testDAG() {
    cout << "Test 4: DAG (Yönsüz Döngüsüz Graf) Testi" << endl;
    
    resetGraphs();
    
    // Düğümleri ekle
    for (char c = 'a'; c <= 'e'; ++c) {
        addVertex(testGraph, c);
        addVertex(testTranspose, c);
    }
    
    // DAG yapısı: a -> b -> c -> d -> e
    addEdge(testGraph, 'a', 'b');
    addEdge(testGraph, 'b', 'c');
    addEdge(testGraph, 'c', 'd');
    addEdge(testGraph, 'd', 'e');
    
    // Transpose graf bağlantıları
    addEdge(testTranspose, 'b', 'a');
    addEdge(testTranspose, 'c', 'b');
    addEdge(testTranspose, 'd', 'c');
    addEdge(testTranspose, 'e', 'd');
    
    // Kosaraju algoritmasını çalıştır
    vector<vector<char>> result = kosaraju_test();
    
    // Beklenen SCC'ler: Her düğüm kendi başına bir SCC olmalı
    vector<vector<char>> expected = {
        {'a'},
        {'b'},
        {'c'},
        {'d'},
        {'e'}
    };
    
    // Sonuçları göster
    cout << "Bulunan SCC'ler:" << endl;
    printSCCs(result);
    
    cout << "Beklenen SCC'ler:" << endl;
    printSCCs(expected);
    
    // Sonuçları karşılaştır
    if (compareSCCs(result, expected)) {
        cout << "Test 4 BAŞARILI" << endl;
    } else {
        cout << "Test 4 BAŞARISIZ" << endl;
    }
    cout << endl;
}

// Test 5: Karmaşık graf - birden fazla SCC ve döngüler
void testComplexGraph() {
    cout << "Test 5: Karmaşık Graf Testi" << endl;
    
    resetGraphs();
    
    // Düğümleri ekle
    for (char c = 'a'; c <= 'i'; ++c) {
        addVertex(testGraph, c);
        addVertex(testTranspose, c);
    }
    
    // Normal graf bağlantıları
    // SCC1: {a, b, e}
    addEdge(testGraph, 'a', 'b');
    addEdge(testGraph, 'b', 'e');
    addEdge(testGraph, 'e', 'a');
    
    // SCC2: {c, d, h}
    addEdge(testGraph, 'c', 'd');
    addEdge(testGraph, 'd', 'h');
    addEdge(testGraph, 'h', 'c');
    
    // SCC3: {f, g}
    addEdge(testGraph, 'f', 'g');
    addEdge(testGraph, 'g', 'f');
    
    // SCC4: {i} (tek düğüm)
    
    // SCC'ler arası bağlantılar
    addEdge(testGraph, 'b', 'c'); // SCC1 -> SCC2
    addEdge(testGraph, 'h', 'i'); // SCC2 -> SCC4
    addEdge(testGraph, 'e', 'f'); // SCC1 -> SCC3
    addEdge(testGraph, 'g', 'i'); // SCC3 -> SCC4
    
    // Transpose graf bağlantıları
    // SCC1: {a, b, e}
    addEdge(testTranspose, 'b', 'a');
    addEdge(testTranspose, 'e', 'b');
    addEdge(testTranspose, 'a', 'e');
    
    // SCC2: {c, d, h}
    addEdge(testTranspose, 'd', 'c');
    addEdge(testTranspose, 'h', 'd');
    addEdge(testTranspose, 'c', 'h');
    
    // SCC3: {f, g}
    addEdge(testTranspose, 'g', 'f');
    addEdge(testTranspose, 'f', 'g');
    
    // SCC'ler arası bağlantılar
    addEdge(testTranspose, 'c', 'b'); // SCC2 -> SCC1
    addEdge(testTranspose, 'i', 'h'); // SCC4 -> SCC2
    addEdge(testTranspose, 'f', 'e'); // SCC3 -> SCC1
    addEdge(testTranspose, 'i', 'g'); // SCC4 -> SCC3
    
    // Kosaraju algoritmasını çalıştır
    vector<vector<char>> result = kosaraju_test();
    
    // Beklenen SCC'ler: {a,b,e}, {c,d,h}, {f,g}, {i}
    vector<vector<char>> expected = {
        {'a', 'b', 'e'},
        {'c', 'd', 'h'},
        {'f', 'g'},
        {'i'}
    };
    
    // Sonuçları göster
    cout << "Bulunan SCC'ler:" << endl;
    printSCCs(result);
    
    cout << "Beklenen SCC'ler:" << endl;
    printSCCs(expected);
    
    // Sonuçları karşılaştır
    if (compareSCCs(result, expected)) {
        cout << "Test 5 BAŞARILI" << endl;
    } else {
        cout << "Test 5 BAŞARISIZ" << endl;
    }
    cout << endl;
}

int main() {
    cout << "Kosaraju Algoritması Test Programı" << endl;
    cout << "=====================================" << endl << endl;
    
    // Tüm testleri çalıştır
    testOriginalGraph();    // Test 1: Orijinal graf örneği
    testSingleSCC();        // Test 2: Tek bir SCC
    testDisconnectedGraph();// Test 3: Bağlantısız graf
    testDAG();              // Test 4: DAG (Hiç döngü olmayan graf)
    testComplexGraph();     // Test 5: Karmaşık graf
    
    cout << "Tüm testler tamamlandı!" << endl;
    return 0;
}