import os
import subprocess

steps = 1000
size = 50
height = 38

# 1. Установить текущую директорию как рабочую
script_dir = os.path.dirname(os.path.realpath(__file__))
os.chdir(script_dir)

# 2. Проверить наличие файла img.png
if not os.path.exists("img.png"):
    print("Ошибка: файл 'img.png' отсутствует в директории.")
    exit(1)

# 3. Скомпилировать main_img.cpp
executable_name = "main_img"
if os.name == "nt":
    executable_name += ".exe"

if not os.path.exists(executable_name):
    if not os.path.exists("main_img.cpp"):
        print("Ошибка: файл 'main_img.cpp' отсутствует в директории.")
        exit(1)
    try:
        print("Компиляция main_img.cpp...")
        compile_command = ["g++", "-o", executable_name, "main_img.cpp"]
        if os.name == "nt":
            compile_command.append("-std=c++17")
        compile_command.append("-lm")
        subprocess.run(compile_command, check=True)
        print("Компиляция main_img.cpp успешно завершена.")
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при компиляции main_img.cpp: {e}")
        exit(1)

# 4. Запустить img.py
try:
    print("Запуск img.py...")
    subprocess.run(["python3" if os.name != "nt" else "python", "img.py"], check=True)
    print("img.py успешно выполнен.")
except subprocess.CalledProcessError as e:
    print(f"Ошибка при выполнении img.py: {e}")
    exit(1)

# 5. Запустить main_img для каждого цветового канала
channels = ["red", "green", "blue"]
for channel in channels:
    points_file = f"points_{channel}.txt"
    output_file = f"approximation_{channel}.txt"

    if not os.path.exists(points_file):
        print(f"Ошибка: входной файл {points_file} отсутствует.")
        exit(1)

    command = f"./{executable_name} {steps} {size} {output_file} {height} < {points_file}" if os.name != "nt" else f"{executable_name} {steps} {size} {output_file} {height} < {points_file}"
    try:
        print(f"Запуск команды: {command}")
        subprocess.run(command, shell=True, check=True)
        print(f"{executable_name} успешно обработал {points_file}.")
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при выполнении команды для {channel}: {e}")
        exit(1)

# 6. Запустить restorve.py
try:
    print("Запуск restorve.py...")
    subprocess.run(["python3" if os.name != "nt" else "python", "restorve.py"], check=True)
    print("restorve.py успешно выполнен.")
except subprocess.CalledProcessError as e:
    print(f"Ошибка при выполнении restorve.py: {e}")
    exit(1)

print("Все шаги успешно выполнены.")
