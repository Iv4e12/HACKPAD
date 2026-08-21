import os

modes = ["ENCODE", "DECODE"]
ciphers = ["TERRAPRISM", "AUTHOR CODE", "BILL CODE"]

characters = list("ABCDEFGHIJKLMNOPQRSTUVWXYZ")

terraprism_symbols = [
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z"
]


def menu(title, options):
    selected = 0

    while True:
        os.system("clear")

        print("╔════════════════════════════╗")
        print(f"║ {title:^26} ║")
        print("╚════════════════════════════╝")
        print()

        for i, option in enumerate(options):
            if i == selected:
                print("> " + option)
            else:
                print("  " + option)

        print()
        print("W/S = wheel    ENTER = select")

        key = input("> ").lower()

        if key == "w":
            selected = (selected - 1) % len(options)

        elif key == "s":
            selected = (selected + 1) % len(options)

        elif key == "":
            return options[selected]


def enter_message():
    message = ""
    selected = 0
    cursor = 0

    if cipher == "TERRAPRISM":
        current_characters = terraprism_symbols
    else:
        current_characters = characters

    while True:
        os.system("clear")

        print("╔════════════════════════════╗")
        print("║      MYSTERY DECODER       ║")
        print("╠════════════════════════════╣")
        print("║                            ║")
        print("║  Message: " + message[:cursor] + "|" + message[cursor:])
        print("║                            ║")
        print("║  Selected: " + current_characters[selected])
        print("║                            ║")
        print("╚════════════════════════════╝")

        print()
        print("W/S = wheel")
        print("A/D = left/right")
        print("SPACE = space")
        print("X = delete")
        print("ENTER = select")
        print("Q = finish")

        key = input("> ").lower()

        if key == "w":
            selected = (selected - 1) % len(current_characters)

        elif key == "s":
            selected = (selected + 1) % len(current_characters)

        elif key == "":
            message = (
                message[:cursor]
                + current_characters[selected]
                + message[cursor:]
            )
            cursor += 1

        elif key == "space":
            message = message[:cursor] + " " + message[cursor:]
            cursor += 1

        elif key == "x":
            if cursor > 0:
                message = message[:cursor - 1] + message[cursor:]
                cursor -= 1

        elif key == "a":
            if cursor > 0:
                cursor -= 1

        elif key == "d":
            if cursor < len(message):
                cursor += 1

        elif key == "q":
            return message


def atbash(text):
    result = ""

    for letter in text:
        if letter in characters:
            position = characters.index(letter)
            result += characters[25 - position]
        else:
            result += letter

    return result


mode = menu("SELECT MODE", modes)

cipher = menu("SELECT CIPHER", ciphers)

message = enter_message()

print()
print("Final message:", message)
