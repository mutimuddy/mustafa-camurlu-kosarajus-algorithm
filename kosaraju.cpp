#include <iostream>
#include <stack> //stack yapısını kullanmak için
using namespace std;

const int MAX = 10; //en fazla 10 düğüm olacağını gösterir

struct Edge { //bir kenarı temsil eder
    int to; //to;kenarların yöneldiği düğümün indeksini tutar 
    Edge* next; //next;bir sonraki kenarı ifade eder
};

struct Vertex { //vertex;grafikteki düğümü ifade eder
    char name; //düğümün ismini belirtir
    bool visited; //visited; düğümün DFS sırasında ziyaret edilip edilmediğini belirtir
    Edge* adj; //adj; düğüme bağlı komşuları ifade eder
};

Vertex graph[MAX]; //orijinal graf
Vertex transpose[MAX];//ters graf
stack<int> finishStack; //DFS bitiş sırasını tutmak için stack

void addVertex(Vertex graph[], char name) { //bu fonksiyon verilen name parametresi ile bir düğüm ekler
    for (int i = 0; i < MAX; ++i) {
        if (graph[i].name == '0') {
            graph[i].name = name;
            graph[i].visited = false; //düğümün visited durumu false olarak ayarlanır 
            graph[i].adj = nullptr; //adj(komşu kenar) başlatılır ve nullptr olarak ayarlanır
            break;
        }
    }
}

int getIndex(Vertex graph[], char name) { //düğümün sırası bulunur
    for (int i = 0; i < MAX; ++i) {
        if (graph[i].name == name) return i;
    }
    return -1; //eğer böyle bir düğüm yoksa -1 döner
}

void addEdge(Vertex graph[], char from, char to) { //bu fonksiyonda form düğümünden to düğümüne bir bağlantı çizer
    int fromIdx = getIndex(graph, from);
    int toIdx = getIndex(graph, to);
    Edge* edge = new Edge; //yeni bağlantı oluşturulur ve mevcutların başına eklenir
    edge->to = toIdx;
    edge->next = graph[fromIdx].adj;
    graph[fromIdx].adj = edge;
}

void dfs1(Vertex graph[], int idx) { //ilk DFS gerçekleştirilir
    graph[idx].visited = true; //idx gösterilen düğüm ve kenarlara ziyaret edilir
    for (Edge* e = graph[idx].adj; e != nullptr; e = e->next) {
        if (!graph[e->to].visited) {
            dfs1(graph, e->to);
        }
    }
    finishStack.push(idx); //en son ziyaret edilen düğüm yığına eklenir
}

void dfs2(Vertex graph[], int idx) { //ters graf üzerinde DFS yapar ve güçlü bağlı bileşenleri(SCC) ekrana yazdırılır
    graph[idx].visited = true;
    cout << graph[idx].name << " ";
    for (Edge* e = graph[idx].adj; e != nullptr; e = e->next) {
        if (!graph[e->to].visited) {
            dfs2(graph, e->to); //finishStack’teki bitiş sırasına göre DFS yapılır ve her SCC’nin elemanları yazdırılır
        }
    }
}

void kosaraju() { //kosaraju algoritması bu fonksiyonda çalıştırılır
    for (int i = 0; i < MAX; ++i) { // ilk DFS
        if (graph[i].name != '0' && !graph[i].visited) {
            dfs1(graph, i); // graph üzerinde DFS yapılır ve bitiş sırası finishStack’e eklenir
        }
    }

    for (int i = 0; i < MAX; ++i) { 
        transpose[i].visited = false;//tüm düğümlerin visited durumu sıfırlanır 
    }

    cout << "\nStrongly Connected Components:\n"; //transpose üzerinde DFS 
    while (!finishStack.empty()) {
        int idx = finishStack.top();
        finishStack.pop();
        if (!transpose[idx].visited) {
            cout << "SCC: ";
            dfs2(transpose, idx);//finishStack’teki sıralamaya göre ters graf üzerinde DFS yapılır ve her bir SCC ekrana yazdırılır 
            cout << endl;
        }
    }
}

int main() { //bu fonksiyon grafı başlatır, düğümleri ve kenarları ekler ve kosaraju algoritmasını çalıştırır 
    for (int i = 0; i < MAX; ++i) {
        graph[i].name = '0';
        transpose[i].name = '0';
    }

    for (char c = 'a'; c <= 'h'; ++c) { //"a" ve "h" arasında hem normal hem ters grafa düğümler eklenir
        addVertex(graph, c);
        addVertex(transpose, c);
    }

    // Normal grafik bağlantıları
    addEdge(graph, 'a', 'b');
    addEdge(graph, 'b', 'e');
    addEdge(graph, 'e', 'a');
    addEdge(graph, 'b', 'f');
    addEdge(graph, 'b', 'c');
    addEdge(graph, 'c', 'd');
    addEdge(graph, 'd', 'c');
    addEdge(graph, 'd', 'h');
    addEdge(graph, 'h', 'h');
    addEdge(graph, 'g', 'h');
    addEdge(graph, 'f', 'g');
    addEdge(graph, 'g', 'f');
    addEdge(graph, 'e', 'f');

    // Transpoz grafik bağlantıları
    addEdge(transpose, 'b', 'a');
    addEdge(transpose, 'e', 'b');
    addEdge(transpose, 'a', 'e');
    addEdge(transpose, 'f', 'b');
    addEdge(transpose, 'c', 'b');
    addEdge(transpose, 'd', 'c');
    addEdge(transpose, 'c', 'd');
    addEdge(transpose, 'h', 'd');
    addEdge(transpose, 'h', 'h');
    addEdge(transpose, 'h', 'g');
    addEdge(transpose, 'g', 'f');
    addEdge(transpose, 'f', 'g');
    addEdge(transpose, 'f', 'e');

    kosaraju(); //kod çalıştırılır
    return 0;
}
