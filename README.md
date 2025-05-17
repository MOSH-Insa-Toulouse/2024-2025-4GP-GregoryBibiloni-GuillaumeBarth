# 2024-2025-4GP-GregoryBibiloni-GuillaumeBarth

## Projet Capteur Graphite

# Sommaire

- [Contexte](#contexte)
- [Livrables](#livrables)
- [Matériel necéssaire](#matériel-nécessaire)
- [Simulation LTSpice](#simulation-ltspice)
- [Design du PCB via KiCad](#design-du-pcb-via-kicad)
- [Réalisation du Shield](#réalisation-du-shield)
- [Code Arduino](#code-arduino)
- [Application mobile Android via MIT App Inventor](#application-mobile-android-via-mit-app-inventor)
- [Banc de test et analyse](#banc-de-test-et-analyse)
- [Datasheet du capteur graphite](#datasheet-du-capteur-graphite)
- [Conclusion](#conclusion)

---

# Contexte

Ce projet est réalisé dans le cadre de l’UF « du capteur au banc de test » pour les étudiants en quatrième année Génie Physique. Nous nous sommes inspirées de l’article scientifique _Pencil Drawn Strain Gauges and Chemiresistors on Paper_ (Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim & Jiaxing Huang).
Cet article explique comment faire un capteur résistif à partir de crayon et de papier.

Quand on écrit avec un crayon gris sur du papier, on dépose des couches de graphites qui sont des conducteurs et laisse passer le courant mais avec une résistance. En déformant ce papier, les chaines de graphique sont plus ou moins éloignés et le courant circule plus ou moins facilement ce qui implique une variation de résistance. Ce phénomène fait qu’on peut faire de ce capteur une jauge de contrainte.

![Schéma 1](ReadMe/Image/Compression%20déformation.png)
_Cette figure vient de l'article cité ci dessus, et montre les chaines de carbone si on effectue une compression ou une flexion du papier. Une compression fait qu'on a des chaines plus proche donc une plus faible valeur de résistance. Pour une flexion on a l'effet contraire_

L’objectif de ce projet est donc de pouvoir réaliser une jauge de contrainte « low-tech » 

---

# Livrables

Voici la liste des livrables :

•	Le Shield PCB branché sur une carte Arduino UNO, intégrant le capteur graphite, un module Bluetooth, un encodeur rotatoire, un potentiomètre numérique, un écran OLED, et un capteur de flexion industriel.

•	Le code Arduino qui permet de contrôler tous les composants et d’acquérir les mesures. (Mesure de résistance, Connection Bluetooth, Affichage de l’OLED, l’encodeur rotatoire et le potentiomètre numérique).

•	L’application Android qui permet d’afficher la valeur de la résistance directement sur son téléphone.

•	La datasheet du capteur en graphite (avec différentes valeurs de crayon).

---

# Matériel nécessaire

Pour concevoir notre dispositif, on a besoin de ces composants :

•	Des résistances : 1 résistance de 1 kΩ, 1 résistance de 10 kΩ, 2 résistances de 100 kΩ, une résistance R2 qui correspond au potentiomètre numérique (le MCP41050) et une résistance de 47 kΩ pour la jauge de contrainte industrielle.

•	Des capacités : 2 capacités de 100 nF et 1 de 1 µF.

•	D’un amplificateur opérationnel : le LTC1050.

•	Un module Bluetooth HC-05.

•	Un encodeur rotatoire ESP32.

•	Un écran OLED 128 * 64.

•	Un flex sensor.

•	Le capteur graphite (connecté avec des pinces croco) fait avec un papier et un crayon.

•	La carte Arduino UNO.

---

# Simulation LTSpice

La résistance interne du capteur graphite est de l’ordre du GΩ donc le courant généré après application de la tension de 5V (Valeur de l’arduino) est faible (de l’ordre du nA). On a besoin de l’amplifier grâce à un montage transimpédance composé d’un amplificateur opérationnel (AOP), le LTC1050, qui peut capter les signaux de faible intensité.

![Schéma 2](ReadMe/Image/Schéma%20electrique%20LTSpice.png)

Ce montage est composé de 3 filtres afin de limiter le bruit :

•   En rouge, on filtre le 50 Hz du réseau d’électricité

•	En bleu, on filtre le bruit du capteur

•	En vert, on filtre le bruit de l’acquisition de l’Arduino

Voici une simulation sur LTSpice qui montre l'atténuation du bruit en fonction de la fréquence :
![Schéma 2bis](ReadMe/Image/LTSpiceReducBruit.png)

On réduit le 50 Hz de 72 dB environ.

Pour calculer la valeur de la résistance du capteur graphite, la formule est la suivante :

![Schéma 3](ReadMe/Image/Formule%20résistance.png)

Où R2 correspond à la valeur du potentiomètre numérique.



---

# Design du PCB

On a conçu notre PCB via le logiciel KiCad, cela s’est fait en 2 étapes :

La première étape consiste en la réalisation du schématique, donc de créer tous les composants qu’on utilise en leur attribuant des pins, un nom et un modèle 3D. Une fois tous nos composants créés, on rajoute le montage de notre amplificateur transimpédance et on affecte chaque pin de l’Arduino à nos composants et on effectue le câblage entre les différents composants.

![Schéma 4](ReadMe/Image/Face%20schématique.png)
Ceci est le schéma électrique de l'ensemble de nos composants.

Une fois cela fait, on doit maintenant positionner l'ensemble de nos composants sur le PCB et les relier par la suite. Il faut placer judicieusement les composants sur le PCB afin qu'ils ne se chevauchent pas les uns sur les autres, et qu'ils n'empêchent pas le câblage de l'ensemble des composants. Cette étape nécessite une certaine rigueur afin d'éviter le plus que possible de faire des VIA (trou effectué afin de faire des connexions par-dessus certains routages).

Voici le résultat de notre routage sur notre PCB :

![Schéma 5](ReadMe/Image/PCB%20sur%20kicad.png)
U6 correspond à l’encodeur rotatoire (et le servor a été mit en plus si on avait besoin d’un servo motor pendant les manipulations).

👉 [Voici le lien pour le KiCad](KiCad/)


---

# Réalisation du Shield

Une fois le PCB réalisé sur KiCaD, on l'a envoyé à Cathy afin qu'elle puisse vérifier que le PCB est correct et prêt à être imprimé. Pour cela, on lui a envoyé notre Gerber, le schéma contenant que le routage de notre carte. Le Gerber sert de masque afin de faire une impression UV sur une couche en cuivre sur la carte en époxy. Une fois cette impression faite, elle a plongé la plaque dans un révélateur qui a retiré les zones non exposées. Enfin un lavage à l'acétone permet d'enlever les dernières impuretés.

Le Gerber :
![Schéma 5bis](ReadMe/Image/Gerber.jpg)

Il faut par la suite percer des trous sur notre plaque avec un fôret (0.8mm pour les composants et 1mm pour les pins de l’Arduino).

Une fois cette étape effectuée, on place tous nos composants sur notre Shield en les soudants, puis on raccorde le Shield avec la carte Arduino Uno :

![Schéma 6](ReadMe/Image/Shield%20et%20composant.png)
---

# Code Arduino

On code maintenant le programme qui permet de faire fonctionner notre système. Dans ce code on peut contrôler un menu qui s’affiche sur l’écran OLED à l’aide de l’encodeur rotatoire. On peut rentrer manuellement la valeur du potentiomètre numérique à l’aide de ce menu, on peut contrôler la connexion Bluetooth et on peut lire la valeur de la résistance liée à la déformation du capteur.

Le code est présent dans notre Git.
👉 [Voici le lien pour le code Arduino](Code%20Arduino/)

---

# Application mobile Android via MIT App Inventor

On a créé l’application grâce au logiciel MIT App Inventor. C’est un APK et il est composé d’une face avant et d’une face arrière : 

![Schéma 7](ReadMe/Image/Face%20avant%20app.png)

Ceci est la face avant.


![Schéma 8](ReadMe/Image/Face%20arrière%20app.png)

Et ceci est la face arrière.

Grâce à cette application, on peut se connecter sur le module Bluetooth et récupérer la valeur de la résistance en fonction de la flexion.

👉 [Voici le lien pour l'application Android (sous forme .aia et .apk)](Application%20Android/)

---

# Banc de test

On veut faire un banc de test qui permet de calculer la valeur de la résistance du capteur graphite pour différentes valeurs de déformation.
On utilise donc un support qui possèdent différents emplacement qui ont chacun leur propre rayon de courbure.

Le support ressemble à :

![Schéma 9](ReadMe/Image/Banc.jpg)

De haut en bas :

•   Rayon de courbure de 0.045m pour une déformation de 2,11E-3. 

•   Rayon de courbure de 0.04m pour une déformation de 2,38E-3.

•   Rayon de courbure de 0.035m pour une déformation de 2,71E-3.

•   Rayon de courbure de 0.03m pour une déformation de 3,17E-3.

•   Rayon de courbure de 0.025m pour une déformation de 3,80E-3.

•   Rayon de courbure de 0.02m pour une déformation de 4,75E-3.

Voici les résultats obtenus avec une résistance du potentiomètre de 40906 ohms :  

![Schéma 10](ReadMe/Image/Flexion.png)

Valeur obtenue pour trois types de crayons en effectuant une flexion : HB, 3B et 6B.
On remarque donc une augmentation de la résistance relative plus on déforme le papier, ce qui était le résultat attendu.

![Schéma 10](ReadMe/Image/Compression.png)

Valeur obtenue pour trois types de crayons en effectuant une compression : HB, 3B et 6B.
On remarque donc une diminution de la résistance relative plus on déforme le papier, ce qui était le résultat attendu.

On a remarqué un défaut pendant nos manipulations, et c'est que le papier s'use très vite et on a souvent besoin de le changer fréquemment afin qu'il soit de nouveau opérationnel. 

---

# Datasheet du capteur graphite

👉 [Voici le lien pour la data sheet](DataSheet/DataSheet.pdf)

---

# Conclusion

En conclusion, ce capteur permet de mesurer des variations de résistance et donner un ordre de grandeur de cette variation. Mais il n'est pas très précis et il s'use facilement. Il ne permet donc pas de remplacer le flex sensor industriel