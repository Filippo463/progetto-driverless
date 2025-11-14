# Progetto Controllo Veicolo Driverless (E-Agle FSD Task)

Questa repository contiene la mia sottomissione per il task di controllo E-Agle Formula Student Driverless.

Il progetto è stato sviluppato in C++ e CMake. Implementa un modello cinematico di veicolo (Livello 1 + Bonus) e un **Stanley Controller (Livello 5)** per il tracciamento di un percorso circolare (Bonus 3), con visualizzazione 2D tramite la libreria `raylib`.

---

## 🎥 Risultato Finale (Showcase)

Simulazione dello Stanley Controller (Livello 5) che segue un percorso circolare. Si può notare che il controller è **stabile anche ad alte velocità** (fino a V=50.0) e **corregge l'errore laterale**, mantenendo `e_y` e `e_th` vicini allo zero per tutto il percorso.

[Video della Simulazione (Google Drive)](https://drive.google.com/file/d/1fuXtPzxGQkqCO9IX-o4ftW9oWkyOof2j/view?usp=sharing)

*(Per un'analisi dettagliata di tutti i passaggi, dei fallimenti del P-controller e delle scelte di design, si veda il report completo linkato in fondo).*

---

## 🚀 Istruzioni di Compilazione e Avvio

Il progetto utilizza CMake e `FetchContent` per gestire automaticamente le dipendenze (come `raylib`).

### Prerequisiti

1.  Un compilatore C++ (es. MSVC su Windows tramite **Visual Studio Build Tools**, GCC su Linux).
2.  **CMake** (versione 3.10+).
3.  **Git**.

### Passaggi per la Compilazione

1.  **Clonare la repository:**
    ```bash
    git clone [https://github.com/Filippo463/progetto-driverless.git](https://github.com/Filippo463/progetto-driverless.git)
    cd progetto-driverless
    ```

2.  **Creare la cartella di build e configurare CMake:**
    *(CMake scaricherà automaticamente raylib in questo passaggio)*
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Compilare il progetto:**
    ```bash
    cmake --build .
    ```

4.  **Avviare la simulazione:**
    ```bash
    # Su Windows
    ./Debug/my_car_simulator.exe
    
    # Su macOS / Linux
    ./my_car_simulator
    ```

---

## 🔧 Approccio Tecnico e Scelte di Design

Il mio percorso di sviluppo è stato iterativo e mi ha portato ad implementare una soluzione avanzata (Livello 5) dopo aver scoperto i limiti fondamentali dei controller più semplici.

1.  **Livello 1-3 (Analisi):** Ho implementato un **P-Controller** (Livello 3) su una sinusoide. Ho scoperto che questo controller fallisce catastroficamente in due modi:
    * **Stallo Circolare:** Se l'errore è troppo grande, il raggio di sterzata minimo ($R = L/\tan(\delta_{max})$) rende fisicamente impossibile raggiungere il percorso.
    * **Instabilità ad Alta Velocità:** Il controller è "cieco" alla velocità, ma l'effetto dello sterzo è amplificato da $v$ nella fisica del veicolo, portando a oscillazioni violente ("sminchiamento").

2.  **La Scelta (Livello 5):** Di fronte a questi limiti, ho determinato che "rattoppare" il P-controller con un PID (Livello 4) sarebbe stato solo un curare il sintomo. Ho deciso di saltare direttamente al **Livello 5 (Stanley Controller)** per risolvere il problema alla radice.

3.  **Implementazione (Stanley):** Lo Stanley Controller è una soluzione ibrida "proattiva" che risolve entrambi i problemi:
    * $\delta = \theta_e + \arctan \left( \frac{K \cdot e_y}{v + \epsilon} \right)$
    * Il termine **$\theta_e$ (errore di angolo)** si occupa di "raddrizzare" l'auto.
    * Il termine **$e_y$ (errore laterale)** si occupa di tornare sulla linea.
    * Il termine **$/v$ (velocità)** rende il controller **adattivo**: si "calma" da solo all'aumentare della velocità, risolvendo l'instabilità.

---

## 📄 Report Completo

Per un'analisi dettagliata di tutti i passaggi, le sfide di tuning (come il "Bug di $\pi$" e l'Anti-Windup), la matematica e la bibliografia, si prega di consultare il report tecnico completo.

[Leggi qui il Report Tecnico Completo (.pdf)](https://drive.google.com/file/d/14LujzI2T7pzfmWPGVKIPr4X_BtUh_zZ7/view?usp=sharing)
