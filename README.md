# Kosaraju Algoritması

**Kosaraju algoritması**, yönlendirilmiş bir grafikteki **güçlü bağlantı bileşenleri (SCC)** bulmak için kullanılan doğrusal zamanlı bir algoritmadır. İki ana aşamadan oluşur:

## Aşamalar

1. **Orijinal grafikte DFS yapılması:**  
   Orijinal grafikte bir DFS (derinlik öncelikli arama) gerçekleştirilir ve düğümlerin bitiş zamanları kaydedilir.

2. **Transpoze grafikte DFS yapılması:**  
   Grafikteki tüm kenarların yönü ters çevrilerek transpoze grafik oluşturulur. İlk aşamada kaydedilen bitiş sürelerine göre azalan sıradaki düğümlerle transpoze grafikte DFS yapılır.  
   Bu aşamadaki her DFS geçişi bir SCC (güçlü bağlantılı bileşen) kazandırır.

---

## Avantajları

- Basit ve uygulaması kolaydır.
- Lineer zaman karmaşıklığına sahiptir: **O(V + E)**
- Transpoz grafiği kullanılarak karmaşık grafiklerde  bileşenlerin yapısını detaylı analiz eder.
- Uygulama alanı geniştir.
- Büyük/kapsamlı sistemlerde analiz kolaylığı sağlar.
- Hem zaman hem bellek açısından verimlidir.

## Dezavantajları

- Grafiği iki kez DFS ile tarar (büyük grafiklerde ek işlem yükü).
- Transpoz oluşturma ek işlem ve hafıza gerektirir.
- İki DFS geçişi ve transpoze grafikte yüksek bellek kullanımı oluşturabilir.
- Online (gerçek zamanlı) çalışmaya uygun değildir.

---

## Adımlar

1. Tüm köşeleri ziyaret edilmemiş olarak başlat.
2. Rastgele bir köşeden DFS yap. Eğer tüm köşeler ziyaret edilmemişse `false` döndür.
3. Tüm kenarların yönünü tersine çevir (transpoz grafiği oluştur).
4. Tüm köşeleri yeniden ziyaret edilmemiş olarak işaretle.
5. Aynı köşeden transpoz grafikte DFS yap. Tüm düğümler ziyaret edilmemişse `false`, aksi halde `true` döndür.

---

## Kullanım Alanları

- Sosyal ağ analizi  
- Elektrik devre analizi  
- Web sayfası ve bağlantı analizi  
- Program analizi ve derleyici tasarımı  
- Oyun geliştirme ve yapay zekâ

---

## Ne Zaman Kullanılır?

- Grafik çok büyükse ve hızlı çözüm gerekiyorsa
- Bir sistemde döngüsel yapı kontrolü isteniyorsa
- Birbirine bağlı bileşen grupları tespiti gerekiyorsa
- Yönlü grafikte SCC bulunmak istendiğinde
   
---

## Zaman Karmaşıklığı

Kosaraju algoritması iki aşamalı DFS yapar:

- İlk DFS geçişi: Orijinal grafikte DFS sırasında her kenar bir kez ziyaret edilir.  
  **Zaman karmaşıklığı:** `O(V + E)`

```cpp
for (i = 0; i < MAX; i++) { if (!visited) dfs1(i);}
```

- İkinci geçişte, DFS bu kez ters (transpoz) grafikte yapılır. Her düğüm ve kenar sadece bir kez ziyaret edilir.
  **Zaman karmaşıklığı:** `O(V + E)`

```cpp
while (!finishStack.empty()) {
    if (!visited)
        dfs2(i);
}
```

---

## Uzay karmaşıklığı

Kosaraju algoritmasında düğümler,kenarlar ve stack(yığın) bellekte yer kaplar bunlar :

- düğümler; graph[MAX] ve transpose[MAX] dizileri ile toplam V adet düğüm saklanır.
  **Uzay karmaşıklığı:** `O(V)`

```cpp
Vertex graph[MAX];      // O(V)
Vertex transpose[MAX];  // O(V)
```

- Kenarlar; her bir kenar nesnesi new ile bellekte dinamik olarak oluşturulur.
  **Uzay karmaşılığı:** `O(E)`

```cpp
Edge* edge = new Edge;  // O(E)
```

- Stack, yapısında ilk DFS sonucunda düğüm sırası stack da tutulur.
**Uzay karmaşıklığı:** `O(V)`

```cpp
stack<int> finishStack;  // O(V)
```

---
