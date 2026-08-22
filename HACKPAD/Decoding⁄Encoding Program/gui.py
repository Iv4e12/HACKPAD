import tkinter as tk
import tkinter.font as tkfont

from ciphers import (
    theraprism_encode,
    theraprism_decode,
    fordese_encode,
    fordese_decode,
    bill_encode,
    bill_decode
)


def show_main_menu():

    for widget in window.winfo_children():
        widget.destroy()

    title = tk.Label(
        window,
        text="MYSTERY SHACK DECODER",
        font=("Arial", 22)
    )
    title.pack(pady=25)

    mode_label = tk.Label(
        window,
        text="SELECT MODE",
        font=("Arial", 16)
    )
    mode_label.pack(pady=10)

    encode_button = tk.Button(
        window,
        text="ENCODE",
        font=("Arial", 14),
        width=15,
        command=lambda: choose_cipher("ENCODE")
    )
    encode_button.pack(pady=5)

    decode_button = tk.Button(
        window,
        text="DECODE",
        font=("Arial", 14),
        width=15,
        command=lambda: choose_cipher("DECODE")
    )
    decode_button.pack(pady=5)


def choose_cipher(mode):

    for widget in window.winfo_children():
        widget.destroy()

    title = tk.Label(
        window,
        text="MYSTERY SHACK DECODER",
        font=("Arial", 22)
    )
    title.pack(pady=25)

    label = tk.Label(
        window,
        text="SELECT CIPHER",
        font=("Arial", 16)
    )
    label.pack(pady=10)

    ciphers = [
        "THERAPRISM",
        "AUTHOR CODE",
        "BILL CODE"
    ]

    for cipher in ciphers:

        button = tk.Button(
            window,
            text=cipher,
            font=("Arial", 14),
            width=20,
            command=lambda c=cipher: select_cipher(mode, c)
        )

        button.pack(pady=5)

    back_button = tk.Button(
        window,
        text="← BACK",
        font=("Arial", 14),
        width=20,
        command=show_main_menu
    )
    back_button.pack(pady=20)


def select_cipher(mode, cipher):

    if cipher == "THERAPRISM":
        characters = list("ABCDEFGHIJKLMNOPQRSTUVWXY")
    else:
        characters = list("ABCDEFGHIJKLMNOPQRSTUVWXYZ")

    selected = 0
    message = ""
    cursor = 0

    theraprism_font = tkfont.Font(
        family="The Book of Bill",
        size=40
    )

    fordese_font = tkfont.Font(
        family="Fordese (GF J3)",
        size=40
    )

    bill_font = tkfont.Font(
        family="Bill_s_Cipher",
        size=40
    )

    if cipher == "THERAPRISM":
        cipher_font = theraprism_font

    elif cipher == "AUTHOR CODE":
        cipher_font = fordese_font

    elif cipher == "BILL CODE":
        cipher_font = bill_font

    else:
        cipher_font = ("Arial", 40)

    if mode == "ENCODE":
        selected_font = ("Arial", 40)
        message_font = ("Arial", 20)

    else:
        selected_font = cipher_font
        message_font = cipher_font

    for widget in window.winfo_children():
        widget.destroy()

    title = tk.Label(
        window,
        text="ENTER MESSAGE",
        font=("Arial", 22)
    )
    title.pack(pady=20)

    selected_label = tk.Label(
        window,
        text=characters[selected],
        font=selected_font
    )
    selected_label.pack(pady=10)

    message_label = tk.Label(
        window,
        text="|",
        font=message_font
    )
    message_label.pack(pady=10)

    def update_message():

        display = message[:cursor] + "|" + message[cursor:]

        message_label.config(
            text=display
        )

    def wheel_up():

        nonlocal selected

        selected = (selected - 1) % len(characters)

        selected_label.config(
            text=characters[selected]
        )

    def wheel_down():

        nonlocal selected

        selected = (selected + 1) % len(characters)

        selected_label.config(
            text=characters[selected]
        )

    def select_character():

        nonlocal message, cursor

        message = (
            message[:cursor]
            + characters[selected]
            + message[cursor:]
        )

        cursor += 1

        update_message()

    def add_space():

        nonlocal message, cursor

        message = (
            message[:cursor]
            + " "
            + message[cursor:]
        )

        cursor += 1

        update_message()

    def delete_character():

        nonlocal message, cursor

        if cursor > 0:

            message = (
                message[:cursor - 1]
                + message[cursor:]
            )

            cursor -= 1

            update_message()

    def go_back():

        choose_cipher(mode)

    def finish_message():

        if cipher == "THERAPRISM":

            if mode == "ENCODE":

                result = theraprism_encode(message)
                result_font = theraprism_font
                result_title_text = "ENCODED MESSAGE"

            else:

                result = theraprism_decode(message)
                result_font = ("Arial", 20)
                result_title_text = "DECODED MESSAGE"

        elif cipher == "AUTHOR CODE":

            if mode == "ENCODE":

                result = fordese_encode(message)
                result_font = fordese_font
                result_title_text = "ENCODED MESSAGE"

            else:

                result = fordese_decode(message)
                result_font = ("Arial", 20)
                result_title_text = "DECODED MESSAGE"

        elif cipher == "BILL CODE":

            if mode == "ENCODE":

                result = bill_encode(message)
                result_font = bill_font
                result_title_text = "ENCODED MESSAGE"

            else:

                result = bill_decode(message)
                result_font = ("Arial", 20)
                result_title_text = "DECODED MESSAGE"

        else:

            result = message
            result_font = ("Arial", 20)
            result_title_text = "RESULT"

        for widget in window.winfo_children():
            widget.destroy()

        result_title = tk.Label(
            window,
            text=result_title_text,
            font=("Arial", 22)
        )
        result_title.pack(pady=30)

        original_label = tk.Label(
            window,
            text="Original: " + message,
            font=("Arial", 14)
        )
        original_label.pack(pady=10)

        result_label = tk.Label(
            window,
            text=result,
            font=result_font
        )
        result_label.pack(pady=30)

        mode_label = tk.Label(
            window,
            text="Mode: " + mode,
            font=("Arial", 12)
        )
        mode_label.pack(pady=10)

        cipher_label = tk.Label(
            window,
            text="Cipher: " + cipher,
            font=("Arial", 12)
        )
        cipher_label.pack(pady=5)

        back_button = tk.Button(
            window,
            text="← BACK",
            font=("Arial", 14),
            width=20,
            command=lambda: select_cipher(mode, cipher)
        )
        back_button.pack(pady=20)

    # Keyboard controls for testing the physical controls
    window.bind(
        "<w>",
        lambda event: wheel_up()
    )

    window.bind(
        "<s>",
        lambda event: wheel_down()
    )

    window.bind(
        "<Return>",
        lambda event: select_character()
    )

    space_button = tk.Button(
        window,
        text="SPACE",
        font=("Arial", 12),
        width=10,
        command=add_space
    )
    space_button.pack(pady=5)

    delete_button = tk.Button(
        window,
        text="DELETE",
        font=("Arial", 12),
        width=10,
        command=delete_character
    )
    delete_button.pack(pady=5)

    finish_button = tk.Button(
        window,
        text="FINISH",
        font=("Arial", 12),
        width=10,
        command=finish_message
    )
    finish_button.pack(pady=5)

    back_button = tk.Button(
        window,
        text="← BACK",
        font=("Arial", 14),
        width=20,
        command=go_back
    )
    back_button.pack(pady=15)

    controls = tk.Label(
        window,
        text="W/S = wheel    ENTER = select",
        font=("Arial", 12)
    )
    controls.pack(pady=10)


window = tk.Tk()

window.title("MYSTERY SHACK DECODER")
window.geometry("600x700")

show_main_menu()

window.mainloop()
