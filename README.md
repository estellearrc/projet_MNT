# projet_MNT

Projet C++ de 2A filière ROB à l'ENSTA BRETAGNE de rendu de Modèle Numérique de Terrain (MNT).

Sujet du projet : ```/doc/cpp_06_projet_carte.pdf```

## Pré-requis

* C99 compiler
* C++11 compiler
* SQLite3 >= 3.11 (headers, library and executable)
* libtiff >= 4.0 (headers and library)
* optional (but recommended): curl >= 7.29.0
* GNU make for autotools build or CMake >= 3.9
* Firefox

Pour installer la bibliothèque proj, suivre les instructions au lien suivant : https://proj.org/install.html#installation

## Exécution

Se placer à la racine du dossier.

Lancer ```./build.sh``` dans un terminal.

Si besoin, rendre le script build.sh exécutable avec la commande : 
```
chmod +x build.sh
```

## Sorties

* Documentation du code dans ```/doc/html/index.html``` à ouvrir avec un navigateur web.
* Génération des images de MNT dans le dossier ```/images```.

