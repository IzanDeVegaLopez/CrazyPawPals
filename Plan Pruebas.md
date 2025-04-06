**Hipótesis:**  
El sistema de cartas transmite correctamente las mecánicas del mazo.

**Objetivo Principal:**  
Saber si el jugador reconoce todos los efectos que tienen sus cartas sobre el mazo

**Objetivo Secundario:**  
Saber en qué componentes del juego los jugadores piensan activamente.

**Plan de Pruebas:**  
	**Metodología:**  
	Observación, entrevista

**Desarrollo:**  
En ambos institutos disponemos de una sala con ordenadores. Recibiremos a los jugadores en grupos de 4\. Les haremos pasar hacia un puesto con un ordenador cada uno. Y para cada niño dispondremos de 2 observadores.  
	Los niños tendrán el juego en el menú inicial, y se les pedirá que jueguen al juego desde este punto sin información adicional dada por parte de los observadores. La prueba terminará después de 4 (02/04/2025) minutos. Al acabar el tiempo se pasará a la fase de la entrevista.  
Los testers serán informados del flujo del plan de pruebas de antemano. Es decir, se les hará saber que la *ronda consiste de 4 minutos de juego, seguidos de entrevista de 4 minutos*.  
Durante la fase de entrevista se harán varias preguntas a los jugadores. Esta fase tiene una duración aproximada de otros 4 minutos.

**Procedimiento para la observación:**  
	Se esperará a que los *4 ordenadores* estén ocupados por los *4 testers de cada ronda*. Entonces, se les abrirá una instancia nueva de la aplicación. *que inicia* *en el menú principal*. Jugarán al juego principal.

Después de esto da comienzo la partida y la observación. Durante la observación tan solo nos dirigimos a los tester en las siguientes circunstancias y proveyendo la siguiente información:

- si se buggea y pierden el control o salen fuera de límites, los observadores  reiniciará la aplicación, comenzando de nuevo  
- si no encuentran a todos los enemigos de la oleada y no saben qué hacer, *no se les dará indicaciones adicionales*; habrá flechas in-game que indican la dirección de los enemigos restantes (02/04/2025)

	Durante la observación cada *2 o más observadores* observarán a *1 tester*. Las ambigüedades detalladas arriba serán resueltas por quienes se han asignado a cada tester. Todos los observadores estarán durante el resto de casos situados detrás de los tester prestando atención al gameplay para apuntar la respuesta a las preguntas formuladas en el *Anexo 1*.

**Procedimiento para la entrevista:**  
	Una vez finalice el tiempo de juego. Se pedirá a los testers que dejen de jugar.  
Las preguntas a hacer están en el anexo 2\.  
	Tras responder a una pregunta, si el contenido de la respuesta contesta a esta y el tester quiere seguir expresando su opinión, se le permitirá agradecidamente y el “entrevistador secundario” prestará especial atención al contenido que pueda ser de utilidad y no esté listado en los *temas* del *anexo 2*.  
Una vez finalizadas las preguntas y recogidas las respuestas, se les agradecerá y se les pedirá que vuelvan a clase.  

**ANEXO 1: Cuestiones de Observación:**  
0\.   Anotar lo que hace el tester en el menú principal:
	1\) Darle a play directamente    2\) inspeccionar los botones de selección y darle a play con las por defecto		3\) ir escogiendo todas las armas y elegir unas distintas a las por defecto	4\) escoger de forma que parece random o sin saber lo que está haciendo armas distintas a las por defecto


1\. Primera acción que realiza el tester:  
	1\) Nada	2\) Desplazarse	3\) Disparar	4\) Usar	5\) Recargar 

   
2\. Tiempo que tarda el tester en realizar su primera acción:
	X segundos

   
3\. Primera acción que realiza el tester al encontrarse con un enemigo:  
	1\) Nada	2\) Huir	3\) Acercarse	4\) Disparar


4\.  Tiempo que tarda *desde que aparece en pantalla el primer enemigo*  en darse cuenta que es un enemigo:  
   X segundos  

   
5\.   Si el jugador llega a disparar o usar por primera vez, describir que procede a hacer con el sistema de cartas. Posibles reacciones incluyen pero no están limitadas a:
	1\) Nada	2\) Seguir disparando (sin pensar mucho)	3\) Examinar los tipos de disparo	4\) Examinar el slot de la carta 


6\.   El jugador intenta esquivar los proyectiles enemigos
	1\) Sí.	2\) No


7\.   Si la ha tenido ¿que carta ha causado una reacción emocional perceptible al usarla?
	1) Nada	2\) X carta


8\.   Ataque más usado
	1\) M1	2\) M2


9\. ¿Qué hace el tester cuando se ha quedado sin maná?
	1\) Nada, no ocurre	2\) Intenta seguir disparando y arriesgándose pero no puede	3\) Se da cuenta y cambia de estrategia (huir)



**ANEXO 2: Preguntas de la entrevista:**  
P1. ¿Te has sentido abrumado durante el gameplay? ¿O aburrido porque lo tenías todo controlado? Queremos saber si lo que tenías a tu disposición es adecuado al número de amenazas presentes.  
P1. Cuando disparabas con el clic izquierdo, y cuando con el clic con derecho?  
P2. ¿Ha habido alguna carta que te llamara la atención? ¿Cuál más?  
P3. Describe qué ocurrió con las cartas cuando disparaste por primera vez.  
P4. Pq escogiste la carta de recompensa que cogiste?  
P5. Sabes decirnos pq a veces el M2 no hace nada (debería responder lo del maná si lo entendió)  
P6: (Preguntar solo si cogió alguna carta de mill). Pq crees q a veces salía una 2a carta a la izquierda?

**Supuestos de este plan de prueba:**

* El juego dispone de un tutorial opcional al que se puede acceder a través del menú principal. El tutorial explica:  
  * El funcionamiento del mazo (descartar con M1, y usar cartas con M2)  
  * Eliminar a todos los enemigos para superar una oleada  
  * Ganar recompensa entre oleadas  
  * Superar todas las recompensas para ganar  
* Durante el juego principal hay una barra de progreso que avanza al eliminar enemigos, y se completa cuando la oleada finaliza  
* El juego tiene un flujo que permite ir desde el menú principal hasta el menú de selección de arma y mazo. Desde este hasta la gameScene, y cada vez que se acabe una oleada, sale el menú de recompensas. Se escoge una y después se vuelve a la siguiente oleada del main game.  
* La HUD tiene pistas visuales de qué botón hace las siguientes acciones:  
  * Disparo Principal (M1)  
  * Usar carta (M2)
