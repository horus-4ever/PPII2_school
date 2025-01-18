# Lost in TELECOM Nancy

Vous êtes un élève à TELECOM Nancy.
Perdu dans les locaux de l'école, vous allez devoir résoudre les mystères
et énigmes de vos protagonistes préférés pour retrouver les objets perdus.

## Table des matières

- [Aperçu](#aperçu)
- [Captures d'Écran](#captures-décran)
- [Fonctionnalités](#fonctionnalités)
- [Installation](#installation)
- [Compilation](#compilation)
- [Utilisation](#utilisation)
- [Dépendances](#dépendances)
- [Détails techniques](#détails-techniques)
- [Crédits](#crédits)

## Aperçu

Détailler le jeu ici

## Captures d'Écran

Mettre des captures d'écran du jeu ici

## Fonctionnalités

### Menus

- Premier menu avec présentation du jeu, des touches, choix du volume et bouton pour quitter
- Deuxième menu permettant de personnaliser son personnage

### Dans le jeu

- Déplacer le personnage avec z q d s ou ↑ ← → ↓
- Accéder au premier menu avec echap
- Accéder au deuxième menu avec m
- Parler à un personnage avec i
- Avancer dans les dialogues avec enter
- Passer une porte avec p

### Développement

- Le monde (la map) est décrit dans un fichier texte. Le monde est vu comme une matrice d'entiers avec un type de bout de map et d'objets associés à chaque numéro. On peut donc ainsi personnaliser très facilement le monde du jeu. On peut ainsi ajouter facilement de nouveaux designs (sols et objets).
- L'histoire du jeu est décrite dans un fichier texte aussi. Le dialogue des personnes est contenu dedans. On peut donc ainsi très facilement modifier l'histoire, les quêtes, objets en récompenses, ect.

## Installation

### Pré-requis

Avant de compiler le jeu, assurez-vous d'avoir les outils suivants installés :
- Un compilateur C (Un vrai télécomien utilise clang, mais gcc marche très bien aussi)
- SDL2 (bibliothèque utilisée pour développer le jeu)
- SDL2_image
- SDL2_mixer
- SDL2_ttf

### Installation de SDL2 sur Ubuntu

Commande bash:\
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

## Compilation

Récupérer le projet avec la commande suivante : \
git clone https://gitlab.telecomnancy.univ-lorraine.fr/ppii-2k24/video-game-07.git \
Puis compiler le jeu avec la commande suivante au niveau de la racine du projet : \
make

## Utilisation

Il suffit d'exécuter le jeu avec la commande suivante : \
./main

## Dépendances

Les bibliothèques et dépendances du projet sont les suivantes :
- SDL2
- SDL2_mixer
- SDL2_image
- SDL2_ttf


## Détails Techniques du langage
### Motivation
In order to easily change the gameplay and the story of the game, without recompiling each time, we decided to store the gameplay in separate files.
Since we need to ask for user input and advance in the story conditionally, we decided to create a simple language for the gameplay.

Théophile Pichard (horus-4ever) was in charge to implement the language.

### Implementation
No library has been used to create the language: the files are tokenized and parsed by hand. The source code is converted into an intermediate bytecode representation that is inspired heavily from the Python bytecode.

The interpreter itself is also heavily inspired by CPython (the official Python interpreter), and it interfaces with the game. Objects defined in our original game logic are accessible directly into the language.

Objects are all stored on the heap (as in CPython) and their lifetimes are managed via reference counting (as in CPython). We assumed there was no cyclic references, eliminating the need of a custom garbage colector.

The language doesn't allow the declaration of custom data-types, eliminating the need for a complex typing system.

See `Tn_Object` for more details. Here is an extract of the file.

```c
typedef enum {
    STRING_OBJ, 
    NUMBER_OBJ,
    BOOL_OBJ,
    PLAYER_OBJ,
    QUEST_OBJ,
    CHARACTER_OBJ,
    FUNCTION_OBJ
} Tn_ObjectType;


typedef struct Tn_Object {
    Tn_ObjectType type;
    union {
        UTF8_String* string;
        int64_t number;
        bool boolean;
        Perso_SDL* player;
        Character* character;
        Quest* quest;
        struct Tn_Object*(*function)();
    };
    __getattr__f getattr;
    __setattr__f setattr;
    __eq__f equal;
    __neq__f nequal;
    __ge__f gequal;
    __le__f lequal;
    __gt__f gthan;
    __lt__f lthan;
    __add__f add;
    __sub__f sub;
    __mul__f mul;
    __call__f call;
    __repr__f repr;
    int refcount;
} Tn_Object;
```

### Bytecode
The interpreter accept the following bytecode instructions:
```c
I_LOAD_STRING
I_LOAD_VARIABLE
I_BINARY_ADD
I_BINARY_SUB
I_BINARY_MUL
I_CMP_EQUAL
I_CMP_NEQUAL
I_CMP_GE
I_CMP_LE
I_CMP_GT
I_CMP_LT
I_JUMP_IF_FALSE
I_JUMP
I_CALL
I_METHOD_CALL
I_STORE
I_POP_TOP
I_DUP_TOP
I_LOAD_ATTR
I_STORE_ATTR
I_LOAD_NUMBER
I_LOAD_METHOD
```

### Interpreter
The interpreter interpret the bytecode. The interpreter is not run on a seperate thread, which means that the interpreter is a blocking part of the game. We assume that the code that need to be executed is not too big and doesn't contain heavy computations.

Some built-in functions are asking for user input and put the interpreter in a wainting state. See `interpreter.h` for more details.

### Usage Example
Below is an example of gameplay file that is parsed by the parser.
```js
@quest 1
name: "Introduction"
map: "map1"
nextquest: "quest2"

@subquest 1
description: "Parler à Tomczak"
#init {

}
#loop {
    
}

@subquest 2
description: "Parler à Michèle"
#init {

}
#loop {

}

@subquest 3
description: "Suivre Michèle à l'aile Nord"
#init {
    goto(michele, 106, 160)
}
#loop {

}

@subquest 4
description: "Suivre Michèle à l'aile Sud"
#init {
    goto(michele, 209, 177)
}
#loop {
    
}

@subquest 5
description: "Suivre Michèle à l'aile Est"
#init {
    goto(michele, 176, 137)
}
#loop {
    
}

@subquest 6
description: "Manger avec Tomczak"
#init {

}
#loop {

}

@character "tomczak"
skin: "ressources/images/tomczak.png"
spawnx: "5536"
spawny: "5952"
orientation : "3"
{
    if(quest.subquest == 1) {
        line(tomczak, "Bonjour cher élève, bienvenue à TELECOM Nancy !")
        test = askChoices("Que veux-tu faire ?", "Fuir", "Jouer", "Manger", "Etudier la SIC", 4)
        if(test == "Etudier la SIC") {
            line(tomczak, "Ah ah ! C'est bien !")
            player.name = ask("Comment tu t'appelles déjà ?")
            line(tomczak, player.name + "... Je m'en souviendrai pour les SIC party !")
            line(tomczak, "Entre et va voir Michèle.")
            playsound("subquest")
            gotoSubquest(2)
        } else {
            line(tomczak, "Alors ça ne m'intérresse pas...")
        }
    } else {
        line(tomczak, "Je suis Tomczak, le seul et unique vrai prof.")
    }
}

@character "michele"
skin: "ressources/images/michele.png"
spawnx: "4128"
spawny: "4704"
orientation : "3"
{
    if(quest.subquest == 2) {
        line(michele, "Salut " + player.name + " ! Je suis Michèle !")
        line(michele, "C'est ton premier jour n'est-ce pas ?")
        line(michele, "Suis-moi, je vais te montrer l'école.")
        playsound("subquest")
        gotoSubquest(3)
    } else if(quest.subquest == 3) {
        line(michele, "Voici l'aile Nord.")
        line(michele, "Il y a un amphi ici, c'est l'amphi Nord,")
        line(michele, "des fois il y fait froid en hiver.")
        line(michele, "Suis-moi, on va dans l'aile Sud maintenant.")
        playsound("subquest")
        gotoSubquest(4)
    } else if(quest.subquest == 4) {
        line(michele, "Voici l'aile Sud. Il y a un amphi ici aussi, c'est l'amphi Sud.")
        line(michele, "Et il y a également des salles de cours à droite.")
        answer = askChoices("As-tu réussi le partiel de SD ?","Non", "Non", 2)
        if(answer == "Non") {
            line(michele, "Dommage ! Ne t'en fais pas, tu n'es pas le seul... Apparemment c'était un sujet original !")
        } else {
            line(michele, "Dommage ! Ne t'en fais pas, tu n'es pas le seul... Apparemment c'était un sujet original !")
        }
        line(michele, "Viens, je vais te montrer l'aile Est !")
        playsound("subquest")
        gotoSubquest(5)
    } else if(quest.subquest == 5) {
        line(michele, "Et pour finir l'aile Est, celle des élèves.")
        line(michele, "Sur la gauche, c'est le médialab. Tu peux y travailler.")
        line(michele, "Sur la droite tu trouveras les clubs de l'école.")
        answer = askChoices("Y-a-t-il un club qui t'intéresse ?", "Musique", "Sport", "Absoludique", "Bar", 4)
        line(michele, "Génial !")
        line(michele, "Au fond il y a la cafétéria,")
        line(michele, "J'espère que tout est clair !")
        finishQuest()
    } else {
        line(michele, "Je suis Michèle.")
    }
}

@character "heurtel"
skin: "ressources/images/heurtel.png"
spawnx: "2336"
spawny: "4128"
orientation : "3"
{
    line(heurtel, "Je suis Mme Heurtel.")
}

@character "heudiard"
skin: "ressources/images/heudiard.png"
spawnx: "6432"
spawny: "5632"
orientation : "2"
{
    line(heudiard, "Je suis Mme Heudiard.")
}

@character "bouthier"
skin: "ressources/images/bouthier.png"
spawnx: "5344"
spawny: "2464"
orientation : "3"
{
    line(bouthier, "Je suis M Bouthier.")
}

@character "marc"
skin: "ressources/images/marc.png"
spawnx: "4160"
spawny: "3296"
orientation : "3"
{
    line(marc, "Je suis Marc.")
}

@character "lucine"
skin: "ressources/images/lucine.png"
spawnx: "5536"
spawny: "2784"
orientation : "4"
{
    line(lucine, "Je suis Lucine.")
}

@character "theophile"
skin: "ressources/images/theophile.png"
spawnx: "4192"
spawny: "5600"
orientation : "3"
{
    line(theophile, "Je suis Theophile.")
}

@character "amine"
skin: "ressources/images/amine.png"
spawnx: "5280"
spawny: "3424"
orientation : "1"
{
    line(amine, "Je suis Amine.")
}

@character "bar"
skin: "ressources/images/bar.png"
spawnx: "5568"
spawny: "1728"
orientation : "1"
{
    line(bar, "Salut ! Je suis de service au bar aujourd'hui.")
    answer = askChoices("Je te sers quelque chose ?", "Une pomme", "Un oasis PCF", "Une pizza", "Un Kinder Bueno", "Des M&M's", "Un croissant", 6)
    if (answer == "Une pomme") {
        line(bar, "Healthy c'est bien ! Bon app")
    } else if (answer == "Un oasis PCF") {
        line(bar, "Le meilleur oasis, tu as du gout ;)")
    } else if (answer == "Une pizza") {
        line(bar, "Ok ! Elle sera cuite d'ici 20 min.")
    } else if (answer == "Un Kinder Bueno") {
        line(bar, "Pas de problème je te donne ça.")
    } else if (answer == "Des M&M's") {
        line(bar, "Il n'y en a plus désolé, il faut revenir demain.")
    } else {
        line(bar, "Tu as raison, ils sont du jour !")
    }
}

@item "canard"
image: "ressources/images/item1.png"

@item "joker"
image: "ressources/images/item2.png"

@item "souris"
image: "ressources/images/item3.png"

@sound "subquest"
sound: "ressources/sound/subquest.mp3"

```

### Simple Language Built-in Functions Documentation

This document provides detailed documentation for the built-in functions of a simple interpreted language. Each function performs specific operations, often related to game or dialog management.

### `line`

#### Description
Displays a line of dialog from a character.

#### Usage
```simple
line(character, text)
```

#### Parameters
- `character`: The character object who is speaking.
- `text`: The text string to be displayed.

#### Example
```simple
line(narrator, "Once upon a time...")
```

#### Behavior
Displays a dialog box with the specified text from the specified character. The dialog is animated and waits for user input before proceeding.

---

### `ask`

#### Description
Prompts the user with a question and waits for a text input.

#### Usage
```simple
ask(text)
```

#### Parameters
- `text`: The question string to be displayed.

#### Example
```simple
ask("What is your name?")
```

#### Behavior
Displays a dialog box with an input field where the user can type a response. The dialog is animated and waits for user input before proceeding.

---

### `askYesNo`

#### Description
Prompts the user with a yes/no question.

#### Usage
```simple
askYesNo(text)
```

#### Parameters
- `text`: The yes/no question string to be displayed.

#### Example
```simple
askYesNo("Do you want to continue?")
```

#### Behavior
Displays a dialog box with yes and no buttons for the user to choose from. The dialog is animated and waits for user input before proceeding.

---

### `askChoices`

#### Description
Prompts the user with a question and multiple choice options.

#### Usage
```simple
askChoices(text, choice1, choice2, ..., choiceN, number)
```

#### Parameters
- `text`: The question string to be displayed.
- `number`: The number of choices.
- `choice1, choice2, ..., choiceN`: The choice options as strings.

#### Example
```simple
askChoices("Choose your path:", "Forest", "Mountain", "River", 3)
```

#### Behavior
Displays a dialog box with the specified question and choice options. The dialog is animated and waits for user input before proceeding.

---

### `give`

#### Description
Gives an item to the player.

#### Usage
```simple
give(itemName)
```

#### Parameters
- `itemName`: The name of the item to be given.

#### Example
```simple
give("Magic Sword")
```

#### Behavior
Adds the specified item to the player's inventory, displays an animation of the item being received, and plays a sound effect.

---

### `finishSubquest`

#### Description
Marks the current subquest as completed and proceeds to the next subquest.

#### Usage
```simple
finishSubquest()
```

#### Example
```simple
finishSubquest()
```

#### Behavior
Advances the game state to the next subquest and returns a result indicating the operation was successful.

---

### `print`

#### Description
Prints a text message to the console.

#### Usage
```simple
print(text)
```

#### Parameters
- `text`: The text string to be printed.

#### Example
```simple
print("Hello, world!")
```

#### Behavior
Prints the specified text to the console output.

---

### `goto`

#### Description
Moves a character to a specified location on the map.

#### Usage
```simple
goto(character, xcoord, ycoord)
```

#### Parameters
- `character`: The character object to be moved.
- `xcoord`: The x-coordinate of the destination.
- `ycoord`: The y-coordinate of the destination.

#### Example
```simple
goto(hero, 10, 15)
```

#### Behavior
Calculates the closest path to the destination and animates the character walking to the specified coordinates on the map.

---

### Summary

These built-in functions provide essential capabilities for dialog management, user interaction, item handling, and character movement in the simple language interpreter. Each function is designed to handle specific tasks, enabling the creation of interactive and dynamic scripts.



## Crédits

Les personnes qui ont contribué au projet sont :
- Théophile Pichard
- Amine Salhi
- Lucine Giraud
- Marc Lichtner

Les designs sont faits par Lucine avec le logiciel Aseprite \
La map a été faite sur le logiciel Tiled \
La musique et les sons ont été récupéré sur le site https://opengameart.org/ \
La musique est https://opengameart.org/content/soliloquy \
Les autres sons proviennent du pack https://opengameart.org/content/50-rpg-sound-effects
