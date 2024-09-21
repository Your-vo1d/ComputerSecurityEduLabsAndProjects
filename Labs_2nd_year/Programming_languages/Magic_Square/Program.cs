using System;

class MyClass
{
    static void Main()
    {
        int size_matrix;
        string input;
        do
        {
            Console.WriteLine("Enter size square matrix: ");
            input = Console.ReadLine();
            if (int.TryParse(input, out size_matrix) && size_matrix > 0 )
            {
                break;
            }
            else
            { 
                Console.WriteLine("Input error.");
            }
        } while (true);


        int[,] matrix = null;

        try
        {
            matrix = new int[size_matrix, size_matrix];
        }
        catch (OutOfMemoryException)
        {
            Console.WriteLine("Ошибка выделения памяти. Программа завершена.");
            return;
        }
        int[,] myMatrix = GenerateMatrix(size_matrix);
        Console.WriteLine("Матрица");
        for (int i = 0; i < size_matrix; i++)
        {
            for (int j = 0; j < size_matrix; j++)
            {
                Console.Write(myMatrix[i, j] + " ");
            }
            Console.WriteLine();
        }
        if (CheckIsMagicSquare(myMatrix))
        {
            Console.WriteLine("Является магическим квадратом.") ;
        }
        else
        {
            Console.WriteLine("Не является магическим квадратом");
        }
        int[,] matrix2 = InputMatrix(size_matrix);
        if (CheckIsMagicSquare(matrix2))
        {
            Console.WriteLine("Является магическим квадратом.");
        }
        else
        {
            Console.WriteLine("Не является магическим квадратом");
        }
    }

    static int[,] InputMatrix(int size_matrix)
    {
        int[,] matrix = new int[size_matrix, size_matrix];
        for (int i = 0; i < size_matrix; i++)
        {
            for (int j = 0; j < size_matrix; j++)
            {
                do
                {
                    Console.WriteLine("Строка " + i + ". Столбец " + j);
                    string input = Console.ReadLine();
                    int temp = 0;
                    if (int.TryParse(input, out temp))
                    {
                        matrix[i, j] = temp;
                        break; // Выйти из цикла, если парсинг прошел успешно
                    }
                    else
                    {
                        Console.WriteLine("Ошибка! Некорректный ввод. Пожалуйста, введите число заново.");
                    }
                } while (true); // Зациклить ввод до тех пор, пока не будет введено корректное число
            }
        }
        return matrix;
    }

    static int[,] GenerateMatrix(int size_matrix)
    {
        int[,] matrix = new int[size_matrix, size_matrix];
        Random random = new Random();
        for (int i = 0; i < size_matrix; i++)
        {
            for (int j = 0; j < size_matrix; j++)
            {
                matrix[i, j] = random.Next(50);
            }
        }
        return matrix;
    }
    static bool CheckIsMagicSquare(int[,] matrix)
    {
        int sumIdeal = 0;

        for (int i = 0; i < matrix.GetLength(1); i++)
        {
            sumIdeal += matrix[0
                , i];
        }

        int sumHoriz = 0;
        int sumVert = 0;
        int sumLeftDiag = 0;
        int sumRightDiag = 0;
        int size = matrix.GetLength(0);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                sumHoriz += matrix[i, j];
                sumVert += matrix[j, i];
                if (i + j == size - 1)
                {
                    sumLeftDiag += matrix[i, i];
                    sumRightDiag += matrix[i, j];
                }
            }
            if (sumHoriz != sumIdeal || sumVert != sumIdeal)
            {
                Console.WriteLine(sumIdeal);
                return false;
            }
            sumHoriz = 0;
            sumVert = 0;
        }
        if (sumLeftDiag != sumIdeal || sumRightDiag != sumIdeal)
        {
            return false;
        }
        return true;
    }
}