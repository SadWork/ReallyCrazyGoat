from PIL import Image

def reconstruct_image(width, height, file_red, file_green, file_blue, output_path):
    # Создаем пустое изображение
    reconstructed_image = Image.new('RGB', (width, height))
    # Читаем данные из файлов, пропуская первую строку
    with open(file_red, 'r') as f_red, open(file_green, 'r') as f_green, open(file_blue, 'r') as f_blue:
        # Пропускаем первую строку в каждом файле
        f_red.readline()
        f_green.readline()
        f_blue.readline()

        # Читаем остальные строки
        data_red = [list(map(float, line.split())) for line in f_red.readlines()]
        data_green = [list(map(float, line.split())) for line in f_green.readlines()]
        data_blue = [list(map(float, line.split())) for line in f_blue.readlines()]
        
    # Перебираем данные и устанавливаем цвет для каждого пикселя
    for i in range(len(data_red)):
        x, y = int((data_red[i][0]) * width), int((data_red[i][1] )  * height)
        color = (
            int(data_red[i][2] * 255),
            int(data_green[i][2] * 255),
            int(data_blue[i][2] * 255)
        )
        reconstructed_image.putpixel((x, y), color)

    # Сохраняем восстановленное изображение
    reconstructed_image.save(output_path)
    print(f"Восстановление завершено. Изображение сохранено в {output_path}")

# Пример использования
reconstruct_image(64, 64, 'points_red.txt', 'points_green.txt', 'points_blue.txt', 'reconstructed_image.png')
