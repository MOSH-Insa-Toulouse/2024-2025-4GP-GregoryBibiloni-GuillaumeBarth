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
- [Banc de test](#banc-de-test)
- [Résultats](#résultats)
- [Datasheet du capteur graphite](#datasheet-du-capteur-graphite)
- [Conclusion](#conclusion)

---

# Contexte

Ce projet est réalisé dans le cadre de l’UF « du capteur au banc de test » pour les étudiants en quatrième année Génie Physique. Nous nous sommes inspirées de l’article scientifique Pencil Drawn Strain Gauges and Chemiresistors on Paper (Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim & Jiaxing Huang). Cet article explique comment faire un capteur résistif à partir de crayon et de papier.
Quand on écrit avec un crayon gris sur du papier, on dépose des couches de graphites qui sont des conducteurs et laisse passer le courant mais avec une résistance. En déformant ce papier, les chaines de graphique sont plus ou moins éloignés et le courant circule plus ou moins facilement ce qui implique une variation de résistance. Ce phénomène fait qu’on peut faire de ce capteur une jauge de contrainte.
L’objectif de ce projet est donc de pouvoir réaliser une jauge de contrainte « low-tech » 
