# 🎮 Simplified Tetris Engine in C++17

Progetto accademico per il corso di **Programmazione e Laboratorio** (A.A. 2024/2025).

Questa libreria implementa un motore di gioco per una versione semplificata del classico **Tetris** in C++, focalizzandosi su due operazioni fondamentali:

1. **Parsing ricorsivo** dei pezzi da uno stream di input.
2. **Inserimento e simulazione della fisica** dei pezzi nella griglia di gioco (gestione della gravità, eliminazione delle righe e scorrimento dei pezzi).

---

## 🧩 Panoramica delle Classi

La libreria si articola principalmente su due componenti strutturali:

### 1. La classe `piece`

Rappresenta un singolo pezzo di Tetris memorizzato come una matrice quadrata dinamica di valori booleani (`bool** m_grid`), dove la dimensione del lato $s$ deve essere obbligatoriamente una potenza di 2 (es. 1, 2, 4, 8, ...).

* **Proprietà:**
  * `m_side`: Dimensione del lato ($s = 2^k$).
  * `m_color`: Codice colore ASCII/ANSI (`uint8_t`, da 1 a 255).
  * `m_grid`: Griglia booleana 2D allocata dinamicamente.

* **Funzionalità chiave:**
  * **Rule of 5:** Costruttori di copia/spostamento, distruttore e operatori di assegnamento per la gestione sicura della memoria dinamica.
  * `rotate()`: Ruota il pezzo di $90^\circ$ in senso orario (trasforma la cella $(i, j)$ nella cella $(j, \text{m\_side} - i - 1)$).
  * `cut_row(i)`: Rimuove la riga $i$ dal pezzo e fa scorrere le righe superiori verso il basso di una posizione.
  * Operatori I/O (`operator>>`, `operator<<`) e di confronto (`==`, `!=`).

### 2. La classe `tetris`

Rappresenta il campo da gioco di dimensioni $W \times H$ e gestisce la posizione dei pezzi e la logica del punteggio.

* **Struttura Dati:** Gestisce una lista semplicemente concatenata (`m_field`) di nodi, dove ciascun nodo contiene un oggetto `tetris_piece`:

  ```cpp
  struct tetris_piece {
      piece p;
      int x; // Coordinata X dell'angolo in basso a sinistra
      int y; // Coordinata Y dell'angolo in basso a sinistra (y >= 0)
  };
  ```

* **Funzionalità chiave:**
  * `insert(p, x)`: Calcola la caduta del pezzo alla coordinata $x$ posizionandolo alla massima coordinata $y$ valida, gestisce il completamento delle righe, incrementa lo score di $W$ per ogni riga completata, elimina le righe e fa scorrere i pezzi rimanenti.
  * `containment(p, x, y)`: Verifica se un pezzo può essere contenuto nel campo senza sovrapporsi ad altri pezzi o uscire dai bordi.
  * **Iteratori custom:** Supporta iteratori di tipo Forward (`begin()`, `end()`, `const_iterator`) per scorrere i pezzi presenti nel campo.

---

## 🌲 Formato Ricorsivo dei Pezzi

I pezzi vengono serializzati/deserializzati utilizzando una rappresentazione sintetica basata su una struttura gerarchica a 4 quadranti ordinati in senso orario/lettura: **Top-Left (TL)**, **Top-Right (TR)**, **Bottom-Left (BL)**, **Bottom-Right (BR)**.

### Sintassi dello Stream di Input:

```text
<dimensione_lato> <colore> <griglia_ricorsiva>
```

Sintassi dei quadranti:
* `()`: Quadrante completamente pieno.
* `[]`: Quadrante completamente vuoto.
* `(TL TR BL BR)`: Ricorsione su sotto-quadranti.

### Esempio:

```text
4 75 (([]()[]())(()[]()[])([]()()())(()[]()()))
```

Rappresenta un pezzo $4 \times 4$ con colore `75` e la seguente griglia booleana:

$$
\begin{pmatrix}
- & X & X & - \\
- & X & X & - \\
- & X & X & - \\
X & X & X & X
\end{pmatrix}
$$

---

## 📊 Struttura della Griglia di Gioco `tetris`

Il campo di gioco viene stampato/letto nello stream con il seguente formato:

```text
<score> <width> <height>
<piece_1> <x1> <y1>
<piece_2> <x2> <y2>
...
```

Dove ciascun pezzo viene stampato nell'ordine della lista interna (dalla testa alla coda).

---

## 🛠️ Compilazione ed Esecuzione

Il progetto utilizza un `Makefile` per la gestione della compilazione con C++17.

### Compilazione Standard

Per compilare normalmente il progetto eseguendo l'ottimizzazione del codice (`-O3`) e disabilitando le assunzioni di debug (`-DNDEBUG`), esegui semplicemente:

```bash
make
```

Il comando compila la sorgente `src/tetris.cpp` assieme all'entry point `tools/main.cpp` includendo l'header `include/tetris.hpp`, generando l'eseguibile all'interno della cartella `build/`.

### Esecuzione dell'Eseguibile

Una volta completata la compilazione, puoi avviare l'applicazione con:

```bash
./build/main
```
