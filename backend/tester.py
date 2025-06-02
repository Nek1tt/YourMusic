import tkinter as tk
from tkinter import ttk
import requests

SERVER_URL = "http://localhost:8080/"

# Жёстко прописанный путь к аудиофайлу
AUDIO_PATH = "/Users/stanislavmalarcuk/Desktop/уппрпо/data/shaman-ya-russkij-mp3.mp3"

def send_command(params):
    try:
        response = requests.post(SERVER_URL, json=params)
        print(f"Response: {response.status_code}, {response.text}")
    except Exception as e:
        print(f"Ошибка отправки команды: {e}")

def load_file():
    # Отправляем команду загрузки с жёстким путём
    send_command({"command": "load", "path": AUDIO_PATH})

def play():
    send_command({"command": "resume"})

def pause():
    send_command({"command": "pause"})

def stop():
    send_command({"command": "quit"})

def set_volume(val):
    send_command({"command": "volume", "level": float(val)})

def seek(val):
    send_command({"command": "seeking", "position": float(val)})

root = tk.Tk()
root.title("Audio Player Client")

ttk.Button(root, text="Load & Play", command=load_file).grid(row=0, column=0, columnspan=3, padx=10, pady=10)

ttk.Button(root, text="▶ Play", command=play).grid(row=1, column=0, padx=10, pady=10)
ttk.Button(root, text="⏸ Pause", command=pause).grid(row=1, column=1, padx=10)
ttk.Button(root, text="⏹ Stop", command=stop).grid(row=1, column=2, padx=10)

ttk.Label(root, text="🔊 Volume").grid(row=2, column=0, padx=10, sticky="w")
volume = ttk.Scale(root, from_=0.01, to=1.0, value=1.0, orient="horizontal", command=set_volume)
volume.grid(row=2, column=1, columnspan=2, sticky="ew", padx=10)

ttk.Label(root, text="📍 Seek").grid(row=3, column=0, padx=10, sticky="w")
seek_bar = ttk.Scale(root, from_=0.01, to=1.0, value=0.01, orient="horizontal", command=seek)
seek_bar.grid(row=3, column=1, columnspan=2, sticky="ew", padx=10)

root.mainloop()
