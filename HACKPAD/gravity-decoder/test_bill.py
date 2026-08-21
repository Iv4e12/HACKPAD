import tkinter as tk
import tkinter.font as tkfont

window = tk.Tk()
window.title("Bill Cipher Font Test")
window.geometry("900x500")

bill_font = tkfont.Font(
    family="Bill_s_Cipher",
    size=32
)

title = tk.Label(
    window,
    text="BILL CIPHER ALPHABET",
    font=("Arial", 22)
)
title.pack(pady=20)

alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

alphabet_label = tk.Label(
    window,
    text=alphabet,
    font=bill_font,
    wraplength=800
)
alphabet_label.pack(pady=30)

normal_label = tk.Label(
    window,
    text="Normal: " + alphabet,
    font=("Arial", 16),
    wraplength=800
)
normal_label.pack(pady=20)

window.mainloop()
