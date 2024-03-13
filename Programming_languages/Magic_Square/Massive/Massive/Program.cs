using System;

class Program
{
    static void Main()
    {
        // Считываем размер массива с клавиатуры
        Console.Write("Введите размер массива: ");
        if (int.TryParse(Console.ReadLine(), out int arraySize) && arraySize > 0)
        {
            // Генерируем массив с заданным размером
            int[] array = GenerateArray(arraySize);

            // Выводим сгенерированный массив на экран
            Console.WriteLine("Сгенерированный массив:");
            foreach (var item in array)
            {
                Console.Write(item + " ");
            }

            // Находим самую длинную подпоследовательность значений одного знака
            int[] longestSequence = FindLongestSameSignSequence(array);

            // Выводим результат на экран
            Console.WriteLine("\nСамая длинная подпоследовательность значений одного знака:");
            foreach (var item in longestSequence)
            {
                Console.Write(item + " ");
            }
        }
        else
        {
            Console.WriteLine("Некорректный ввод. Размер массива должен быть положительным целым числом.");
        }
    }

    static int[] GenerateArray(int size)
    {
        Random random = new Random();
        int[] array = new int[size];

        // Заполняем массив случайными значениями (включая отрицательные), исключая 0
        for (int i = 0; i < size; i++)
        {
            // Генерируем случайное число в диапазоне (-50, 0) или (1, 50)
            array[i] = (random.Next(2) == 0 ? -1 : 1) * random.Next(1, 51);
        }

        return array;
    }

    static int[] FindLongestSameSignSequence(int[] array)
    {
        int[] currentSequence = { array[0] };
        int[] longestSequence = { array[0] };

        for (int i = 1; i < array.Length; i++)
        {
            if (HasSameSign(array[i], currentSequence[0]))
            {
                currentSequence = ConcatArrays(currentSequence, array[i]);
            }
            else
            {
                if (currentSequence.Length > longestSequence.Length)
                {
                    longestSequence = currentSequence;
                }

                currentSequence = new int[] { array[i] };
            }
        }

        // Проверяем последнюю последовательность
        if (currentSequence.Length > longestSequence.Length)
        {
            longestSequence = currentSequence;
        }

        return longestSequence;
    }

    static bool HasSameSign(int a, int b)
    {
        return (a >= 0 && b >= 0) || (a < 0 && b < 0);
    }

    static int[] ConcatArrays(int[] arr, int element)
    {
        int[] result = new int[arr.Length + 1];
        Array.Copy(arr, result, arr.Length);
        result[arr.Length] = element;
        return result;
    }
}
