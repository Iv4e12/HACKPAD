THERAPRISM_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXY"
FORDESE_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
BILL_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


def theraprism_encode(text):
    result = ""

    for character in text.upper():
        if character in THERAPRISM_ALPHABET:
            result += character
        elif character == " ":
            result += " "
        else:
            result += "?"

    return result


def theraprism_decode(text):
    result = ""

    for character in text.upper():
        if character in THERAPRISM_ALPHABET:
            result += character
        elif character == " ":
            result += " "
        else:
            result += "?"

    return result


def fordese_encode(text):
    result = ""

    for character in text.upper():
        if character in FORDESE_ALPHABET:
            result += character
        elif character == " ":
            result += " "
        else:
            result += "?"

    return result


def fordese_decode(text):
    result = ""

    for character in text.upper():
        if character in FORDESE_ALPHABET:
            result += character
        elif character == " ":
            result += " "
        else:
            result += "?"

    return result


def bill_encode(text):
    result = ""

    for character in text.upper():
        if character in BILL_ALPHABET:
            result += character
        elif character == " ":
            result += " "
        else:
            result += "?"

    return result


def bill_decode(text):
    result = ""

    for character in text.upper():
        if character in BILL_ALPHABET:
            result += character
        elif character == " ":
            result += " "
        else:
            result += "?"

    return result
