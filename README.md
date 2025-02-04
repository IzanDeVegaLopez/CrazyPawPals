# 🐱✨ Crazy Paw Pals ✨🐾
***¡Únete a la gatalla!*** 🌐 **[Página web del juego](https://ucm-fdi-disia.github.io/CrazyPawPals/)**  


---
# *Game Design Document*
### Equipo de desarrollo: Curtain Reapers

**Integrantes:**  
- Izan de Vega López  
- Haoshuang Hou  
- María Eduarda Beckers  
- Bingcheng Wang  
- Denisa Juarranz Berindea  
- Óscar Melquiades Durán Narganes  
- Amiel Ramos Juez  
- Javier Fueyo López  
- José Narciso Robles Durán
  
## ÍNDICE  
1. [Resumen](#1-resumen)  
   1.1. [Descripción](#11-descripción)  
   1.2. [Género](#12-género)  
   1.3. [Setting](#13-setting)  
   1.4. [Características principales](#14-características-principales)  
2. [Gameplay](#2-gameplay)  
   2.1. [Objetivo del juego](#21-objetivo-del-juego)  
   2.2. [Core loops](#22-core-loops)  
3. [Mecánicas](#3-mecánicas)  
   3.1. [Movimiento](#31-movimiento)  
   3.2. [Maná](#32-maná)  
   3.3. [Retícula](#33-retícula)  
   3.4. [Cartas](#34-cartas)  
   3.5. [Armas](#35-armas)  
   3.6. [Oleadas](#36-oleadas)  
   3.7. [Recompensas](#37-recompensas)  
4. [Interfaz](#4-interfaz)  
   4.1. [Controles](#41-controles)  
   4.2. [Cámara](#42-cámara)  
   4.3. [HUD](#43-hud)  
   4.4. [Menús](#44-menús)  
5. [Mundo del juego](#5-mundo-del-juego)  
   5.1. [Personajes](#51-personajes)  
   5.2. [Niveles](#52-niveles)  
6. [Experiencia de juego](#6-experiencia-de-juego)  
7. [Estética y contenido](#7-estética-y-contenido)  
8. [Referencias](#8-referencias)  


## 1. Resumen  

### 1.1. Descripción  
Roguelike de disparos 2D en perspectiva cenital, donde el jugador tiene el control del disparo del personaje. Las habilidades que el jugador usa pertenecen a un mazo de cartas que podrá mejorar a lo largo de la partida. El juego se divide en oleadas de enemigos. Al completar 10 oleadas, el jugador habrá ganado; en caso de morir antes, habrá perdido y tendrá que comenzar desde cero.  

### 1.2. Género  
Roguelike, cartas, acción, top-down shooter.  

### 1.3. Setting  
En un acogedor café de gatos en plena reforma, extrañas criaturas han aparecido, amenazando con invadir el lugar. Como uno de los valientes gatos del café, la misión del jugador es proteger su hogar y asegurar que todo siga adelante.  

### 1.4. Características principales  
- Estética mona con gatos.  
- Juego de disparos.  
- Juego por oleadas.  
- Roguelite.  
- Cartas.  

## 2. Gameplay  

### 2.1. Objetivo del juego  
El objetivo del juego es completar un número concreto de oleadas (10). Este sería el objetivo a largo plazo, así como mejorar tu mazo y personaje al final de cada oleada. A corto plazo, en cada oleada, el jugador debe sobrevivir y eliminar a todos los enemigos para pasar a la siguiente.  

### 2.2. Core loops  
![Diagrama de flujo](https://github.com/UCM-FDI-DISIA/CrazyPawPals/blob/main/docs/img/Diagrama%20de%20flujo.jpg)

1. *Elección de personaje* (relacionado con arma y mazo de cartas).  
2. *Oleada de enemigos*:  
   - Si gana, obtiene recompensas.  
   - Si pierde, vuelve al inicio.  
3. *Final del juego*: Completar 10 oleadas.  


## 3. Mecánicas  

### 3.1. Movimiento  
El jugador tiene control de movimiento completo en 360 grados, permitiendo desplazarse en cualquier dirección, sin restricciones de ángulo.

**Parámetros:**  
- Velocidad de movimiento máxima.  -->  Aceleración.  
- Porcentaje de interpolación al cambiar de dirección en movimiento.  -->Deceleración.  

### 3.2. Maná  
El maná es la divisa que permite jugar cartas, al jugar una carta se pagará su coste de maná y este se restará del maná que tiene el jugador. Está siempre siempre visible con un número en pantalla, y una barrita que marca el progreso hasta el siguiente punto de maná obtenido. 
El maná se regenera pasivamente con el tiempo a un ritmo fijo, aunque algunas cartas podrían otorgar maná adicional o aumentar la tasa en la que lo ganas temporalmente.
El jugador tiene un maná máximo, la cantidad de maná del jugador nunca puede exceder esta cantidad.

**Representación interna:**  
- El maná se representa como un entero. Cada 100 puntos de maná en la representación equivalen a 1 punto de maná en el juego. El número y la barrita de progreso no son más que el resultado de las operaciones “%100”  “/100” a la representación del maná. 

**Parámetros:**  
- Obtención de maná en puntos por segundo (generalmente entre 0.5 y 1).  
- Maná máximo (entre 2 y 4).  

### 3.3. Retícula  
La retícula es un elemento en pantalla que se corresponderá con la posición del ratón o se moverá con el stick de un mando. Marca el punto hacia el que se lanzarán los hechizos o el ataque de la arma.  

### 3.4. Cartas  
El jugador dispone de un mazo de cartas inicial que podrá modificar a lo largo de la partida.  

**Anatomía de una carta**  
- Imagen identificativa.  
- Coste de maná.  
- Efecto (visible al pasar el ratón sobre la carta).  

**Espacios**  
- Mano: Carta visible para el jugador.  
- Mazo: Cartas no usadas.  
- Descartes: Cartas usadas o descartadas.  

**Funcionamiento General**

Tendrá siempre visible una única carta que tendrá en su mano y podrá usarla o descartarla. Después de cualquiera de estas dos opciones la carta se colocará en el montón de descartes dejando de ser visible, e inmediatamente se coloca la primera carta del mazo en la mano del jugador.

**Recargar**

Cuando no haya más cartas en el mazo ni en la mano tendrá que recargar. Durante este proceso las cartas de descartes vuelven al mazo y se barajan. Dura aproximadamente 2 segundos y el jugador puede hacerlo antes de quedarse sin cartas manualmente. Durante este proceso el jugador no puede descartar ni usar cartas. 

**Uso de cartas**

El jugador podrá usar la carta en su mano, consumiendo maná equivalente a su coste, ejecutando el efecto que contiene y colocándola en descartes en el proceso. Esto se puede hacer siempre y cuando el jugador pueda pagar el coste de maná de la carta. Si el hechizo tiene objetivo, esta será la retícula del jugador.

**Descartar cartas**

El jugador colocará la carta de su mano en descartes sin consumir maná ni ejecutar ninguno de sus efectos. Al hacer esto ejecutará el ataque de arma del que disponga. Esto se puede hacer siempre y cuando el ataque de arma del jugador esté disponible.Tener una velocidad de descartes.


### 3.5. Armas  
Cada vez que el jugador realice la acción de descartar una carta ejecutará el ataque del arma escogida antes de entrar en partida.
Al usar el arma equipada, esta entrará en enfriamiento haciendo irrealizable la acción de descartar una carta. El enfriamiento generalmente dura entre 0.25 y 1 segundo y el tiempo empezará a reducirse justo después de ser usado. Cuando el enfriamiento termine el arma volverá a estar disponible y con ello el descartar cartas.
Si el arma tiene objetivo este será la retícula del jugador.

*Parámetros:*  
- Daño.  
- Enfriamiento.  
- Número de disparos.  
- Duración del ataque.  
- Dispersión (en grados).  
- Efectos adicionales (explosión, veneno, etc.).  

### 3.6. Oleadas  
El juego dispondrá en total de 10 oleadas. La duración aproximada de cada una es de 1 minuto, y se terminan al eliminar a todos los enemigos aparecidos con esta. 

**Desarrollo de las oleadas**

Al comenzar una oleada, los enemigos irán apareciendo en los bordes del escenario durante los 10 primeros segundos de la oleada.

Cada oleada tendrá un cronómetro de 1 min. Cuando queden 10 segundos, el mapa comenzará a cubrirse de niebla, y al llegar a cero, estará completamente cubierto.
La niebla hará daño poco a poco (5% de vida por segundo) al tocar al jugador. Sin embargo, los enemigos no serán afectados por la niebla.

**Fin de Oleada**

Al finalizar una oleada, el mazo se recarga instantáneamente y  comienza un periodo entre oleadas de 10 segundos. Durante este el jugador tendrá una elección entre varias recompensas distintas. Y al acabar los 10 segundos comenzará la siguiente oleada.
Durante esta fase el cooldown del disparo del jugador se vuelve 0 y la recarga del mazo dura tan solo medio segundo.

**Eventos de Oleadas**

En algunas ocasiones las oleadas mostrarán antes de comenzar un sprite con un texto y una imagen que evoquen a un evento de oleadas. Esto hace que las recompensas de esta oleada sean de mayor rareza o calidad, y esta oleada además dispondrá de un modificador en el campo de batalla, el jugador y/o los enemigos.


### 3.7. Recompensas  

Al completar una oleada, aparecerá un menú de recompensas. Este menú mostrará varias opciones, cada una con una descripción que se revelará al pasar sobre ella. El jugador podrá elegir solo una de las recompensas disponibles. Al elegir cualquiera de las recompensas el resto desaparecerán. 
Si al comenzar una nueva oleada no se ha escogido recompensa, empezarán a parpadear y 5 segundos después desaparecerán.

**Distribución de recompensas**

*En una oleada normal:*
- Curación (10-20% vida).
- Nuevas cartas.

*En una oleada con Eventos:*
- Mejorar carta.
  
*En una oleada de Boss:*

Aparecerán 3 recompensas de Boss adicionales con otro color. En este caso podemos escoger una de estas recompensas adicionales y otra de las habituales y el tiempo entre oleadas se alarga a 30 segundos.

**Tipos de recompensas**
- *Curación:* Se aplica instantáneamente al jugador sin poder sobrepasar su vida máxima.
- *Nuevas cartas:* El jugador puede añadir una carta a su mazo. Sin embargo, si el mazo ha alcanzado su límite máximo, deberá elegir entre no coger la nueva carta o sustituir una carta antigua por ella.
- *Mejorar carta:*  Modifica el comportamiento de la carta en cierta medida, puede modificar solo una de las estadísticas o una combinación de varias (algunas de ellas puede considerarse nociva).
  - Coste: Reduce el coste de la carta.
  - Daño: Aumenta el daño.
  - Plano: aumenta el daño en 1 o 2, favorece a ataques multi hit.
  - Porcentual: aumenta el daño del ataque en un porcentaje del ataque actual, favorece a ataques con gran daño.
  - Número Proyectiles: aumenta el número de proyectiles. Si lo hace en gran medida puede modificar la dispersión.
  - Dispersión: en grados.
  - Lanzamiento Doble: Aumenta el coste en 1 para lanzarlo 2 veces.
- *Mejora de Boss:* Mejora de estadísticas del jugador.
  - Mejora de recarga de maná
  - Mejora de maná máximo
  - Mejora de velocidad de movimiento
  - Gana 1-2 de daño de ataque permanente
  - Mejora del tiempo de recarga



## 4. Interfaz  

### 4.1. Controles  
- Movimiento: WASD / LS.  
- Ataque de arma: M1 / RT / R2.  
- Usar carta: M2 / LT / L2.  
- Movimiento de retícula: Ratón / RS.  
- Recarga manual: Space / B / X.  
- Elegir recompensa: F / Y / Square.  

### 4.2. Cámara  
Tiene fijado al personaje en el centro, pero cuando este se acerque a las esquinas del mapa, la cámara se quedará fija antes de poder ver fuera del mapa. Como si se moviese con el jugador pero tuviese puntos máximos y mínimos hasta los que se puede mover.

### 4.3. HUD  
![Boceto HUD](https://github.com/UCM-FDI-DISIA/CrazyPawPals/blob/main/docs/img/bocetoHUD.jpg))

### 4.4. Menús  
- **Menú principal**
    - Jugar (dentro de este menú pulsar arriba o abajo alterna la opción modificada).
    - Escoger arma (pulsando izquierda o derecha).
    - Escoger mazo inicial (pulsando izquierda o derecha)/
    - Comenzar.
    - Salir.

- **Menú de pausa**
    - Restaurar.
    - Salir.

- **Menú de recompensa**
    - Recompensa de cartas.
    - Recompensa de mejorar carta cuando es una oleada de eventos.

- **Menú de Fin de Juego**
    - Volver a jugar.
    - Menú principal.
    - Salir del Juego.

## 5. Mundo del juego  

### 5.1. Personajes  
- **Personajes:** Gatos.
  
  <img src="https://github.com/UCM-FDI-DISIA/CrazyPawPals/blob/main/docs/img/bocetoPersonaje1.png" alt="Boceto personaje" width="100" />

- **Enemigos:**  
  - Tipo 1: SarnoRata (ataque en área).  
  - Tipo 2: MichiMafioso (disparo de proyectiles).  
  - Miniboss 1: Jefe de la CatKuza (patrones de ataque complejos).  

### 5.2. Niveles  
Un único nivel con un mapa de tamaño 1.5-2 veces lo visible en pantalla. Cuyo único elemento son los bordes del escenario que no permiten al jugador avanzar más allá de estos.

## 6. Experiencia de juego  
**Dinámicas Buscadas**

Buscamos dinámicas de roguelike, de disparos en visión cenital, de deckbuilding, de juego por oleadas, de juego arcade y contrarreloj. Buscamos también que los jugadores dependan más de sus habilidades de esquivar y de aprenderse los patrones enemigos que de mejorar el personaje hasta que se pueda pasar el juego solo.

**Descripción de Partida**

*Menú Inicial*

El juego comienza en el **menú principal**, donde el jugador puede:
- Iniciar una nueva partida.
- Ajustar configuraciones.
- Salir del juego.

Al seleccionar **"Jugar"**, se abre una pantalla donde el jugador elige su **arma** y **mazo inicial**. Una vez elegidos, el jugador pulsa **"Comenzar"** para iniciar la partida.

La música se intensifica y comenzamos a ver enemigos acercarse por los bordes de la pantalla.

*Inicio de la Partida*

El personaje del jugador aparece en el **campo de batalla** sin nada a su alrededor, un escenario temático que refleja el entorno del café de gatos. En la interfaz de usuario (HUD), se muestran:
- **Carta en mano**: La primera carta del mazo, lista para ser usada.
- **Contador de maná**: Un indicador numérico y una barra de progreso que muestra el maná disponible.
- **Barra de vida**: Representa la salud del jugador.
- **Temporizador de oleada**: Un contador regresivo que indica **"Tiempo hasta la siguiente oleada: 10 segundos"**.

La música cambia a un tono más intenso, señalando el inicio de la acción. Los enemigos comienzan a aparecer en los bordes del mapa y se acercan al jugador.

*Acciones del Jugador*

- **Disparar con clic izquierdo**: Usa el arma equipada para atacar a los enemigos. Cada disparo consume un breve tiempo de enfriamiento.
- **Usar cartas con clic derecho**: Activa la carta actual en la mano, consumiendo maná. Si no hay suficiente maná, la carta se muestra en colores apagados y se reproduce un sonido de error.

Después de usar o descartar una carta, el mazo avanza, mostrando una nueva carta en la mano. El maná se regenera pasivamente con el tiempo, permitiendo al jugador usar cartas más poderosas más adelante.

*Combate y Estrategia*

Al atacar a los enemigos, estos cambian de color (por ejemplo, se ponen rojos) para indicar que han recibido daño. Después de varios impactos, los enemigos son eliminados y desaparecen del campo de batalla. El jugador debe:
- Moverse constantemente para evitar los ataques enemigos.
- Gestionar su maná para usar las cartas de manera efectiva.

*Fin de la Oleada*

Una vez que todos los enemigos son derrotados, aparecen un **menú de recompensas**. El jugador puede:
- Seleccionar una recompensa presionando la tecla **F**.

Al elegir una recompensa, las demás desaparecen, y la selección se aplica inmediatamente (por ejemplo, una nueva carta se añade al mazo, se mejora una estadística o se recupera salud).

*Eventos Especiales*

En algunas oleadas, puede ocurrir un **evento especial** (por ejemplo, una oleada de jefe o un modificador de dificultad). Estos eventos se anuncian con un mensaje y un icono en pantalla, y ofrecen recompensas más poderosas al completarse.

*Derrota y Menú de Fin de Juego*

Si el jugador pierde toda su salud, aparece un **menú de derrota** que muestra:
- **Puntuación**: Basada en el progreso y las oleadas completadas.
- **Cartas del mazo**: Un resumen de las cartas que se usaron durante la partida.
- **Mejoras obtenidas**: Lista de mejoras o recompensas adquiridas.

El jugador puede elegir entre:
- **Volver a jugar**: Reinicia la partida con la misma configuración inicial.
- **Menú principal**: Regresa al menú inicial para ajustar opciones o seleccionar un nuevo mazo y arma.


## 7. Estética y contenido  
- Música tranquila entre oleadas y frenética durante las oleadas.  
- SFX para disparos, cartas, impactos y daño.  

**Contenido:**  
- Cartas, armas, eventos de oleadas, efectos de estado y mazos iniciales.  
- [Listado de ideas](https://docs.google.com/spreadsheets/d/1PGo0GbLELP-OOfftK2E9CeL0m9wJTdOLV3VXZhlMbZc/edit?usp=sharing).
  

# 8. Referencias  
- **RoR 2:** Tipos de enemigos y patrones de ataque.  
- **One Step From Eden:** Cartas como hechizos + ataque básico y recompensas.Recargar el mazo.
- **MtG:** Mecánica Mill.  
- **PvZ GW 2:** Eventos de oleadas.  
