import whisper
import os
os.chdir(r"C:\Users\LeMeS\PycharmProjects\AI")
print("Current working directory:", os.getcwd())
# Загружаем модель (можно заменить на 'tiny', 'base', 'small', 'medium', 'large')
model = whisper.load_model("base")
os.environ["PATH"] += os.pathsep + r"C:\Users\LeMeS\Desktop\ypprpo\YourMusic\backend\lib\ffmpeg-n7.1-latest-win64-gpl-shared-7.1\bin"
path = "C:/Users/LeMeS/PycharmProjects/AI/maksim-znaesh-li-ty.mp3"
# Загружаем и распознаем аудио
result = model.transcribe("maksim-znaesh-li-ty.mp3", verbose=False)  # verbose=True для подробностей

for segment in result["segments"]:
    start = segment["start"]   # время начала сегмента (в секундах)
    end = segment["end"]       # время конца сегмента
    text = segment["text"]     # текст сегмента
    print(f"[{start:.2f} - {end:.2f}] {text.strip()}")
