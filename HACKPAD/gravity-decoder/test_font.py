import tkinter as tk
import tkinter.font as tkfont


window = tk.Tk()
window.title("Theraprism Font Test")
window.geometry("800x500")

theraprism_font = tkfont.Font(
  family="The Book of Bill",
  size=32
)

title = tk.Label(
  window,
  text="THERAPRISM ALPHABET",
  font=("Arial", 22)
)
title.pack(pady=20)

alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXY"

alphabet_label = tk.Label(
 window,
 text=alphabet,
 font=theraprism_font,
 wraplength=700
)
alphabet_label.pack(pady=30)

normal_label = tk.Label(
 window,
 text="Normal: " + alphabet,
 font=("Arial", 16),
 wraplength=700
)
normal_label.pack(pady=20)

window.mainloop()
