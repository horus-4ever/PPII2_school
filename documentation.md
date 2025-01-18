# Simple Language Built-in Functions Documentation

This document provides detailed documentation for the built-in functions of a simple interpreted language. Each function performs specific operations, often related to game or dialog management.

## `line`

### Description
Displays a line of dialog from a character.

### Usage
```simple
line(character, text)
```

### Parameters
- `character`: The character object who is speaking.
- `text`: The text string to be displayed.

### Example
```simple
line(narrator, "Once upon a time...")
```

### Behavior
Displays a dialog box with the specified text from the specified character. The dialog is animated and waits for user input before proceeding.

---

## `ask`

### Description
Prompts the user with a question and waits for a text input.

### Usage
```simple
ask(text)
```

### Parameters
- `text`: The question string to be displayed.

### Example
```simple
ask("What is your name?")
```

### Behavior
Displays a dialog box with an input field where the user can type a response. The dialog is animated and waits for user input before proceeding.

---

## `askYesNo`

### Description
Prompts the user with a yes/no question.

### Usage
```simple
askYesNo(text)
```

### Parameters
- `text`: The yes/no question string to be displayed.

### Example
```simple
askYesNo("Do you want to continue?")
```

### Behavior
Displays a dialog box with yes and no buttons for the user to choose from. The dialog is animated and waits for user input before proceeding.

---

## `askChoices`

### Description
Prompts the user with a question and multiple choice options.

### Usage
```simple
askChoices(text, choice1, choice2, ..., choiceN, number)
```

### Parameters
- `text`: The question string to be displayed.
- `number`: The number of choices.
- `choice1, choice2, ..., choiceN`: The choice options as strings.

### Example
```simple
askChoices("Choose your path:", "Forest", "Mountain", "River", 3)
```

### Behavior
Displays a dialog box with the specified question and choice options. The dialog is animated and waits for user input before proceeding.

---

## `give`

### Description
Gives an item to the player.

### Usage
```simple
give(itemName)
```

### Parameters
- `itemName`: The name of the item to be given.

### Example
```simple
give("Magic Sword")
```

### Behavior
Adds the specified item to the player's inventory, displays an animation of the item being received, and plays a sound effect.

---

## `finishSubquest`

### Description
Marks the current subquest as completed and proceeds to the next subquest.

### Usage
```simple
finishSubquest()
```

### Example
```simple
finishSubquest()
```

### Behavior
Advances the game state to the next subquest and returns a result indicating the operation was successful.

---

## `print`

### Description
Prints a text message to the console.

### Usage
```simple
print(text)
```

### Parameters
- `text`: The text string to be printed.

### Example
```simple
print("Hello, world!")
```

### Behavior
Prints the specified text to the console output.

---

## `goto`

### Description
Moves a character to a specified location on the map.

### Usage
```simple
goto(character, xcoord, ycoord)
```

### Parameters
- `character`: The character object to be moved.
- `xcoord`: The x-coordinate of the destination.
- `ycoord`: The y-coordinate of the destination.

### Example
```simple
goto(hero, 10, 15)
```

### Behavior
Calculates the closest path to the destination and animates the character walking to the specified coordinates on the map.

---

## Summary

These built-in functions provide essential capabilities for dialog management, user interaction, item handling, and character movement in the simple language interpreter. Each function is designed to handle specific tasks, enabling the creation of interactive and dynamic scripts.
