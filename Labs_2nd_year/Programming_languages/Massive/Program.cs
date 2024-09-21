using System;
using System.Linq;

class Program
{
    static void Main()
    {
        int[] array = InputArray();

        int[] longestSubsequence = FindLongestSubsequence(array);

        Console.WriteLine("Самая длинная подпоследовательность значений одного знака:");
        Console.WriteLine(string.Join(", ", longestSubsequence));
    }

    static int[] InputArray()
    {
        int[] myArray = null;

        do
        {
            Console.Write("Введите элементы массива через пробел: ");
            string input = Console.ReadLine();
            if (input.Contains("0"))
            {
                Console.Write("Массив содержит 0.");
                continue;
            }
            string[] inputArray = input.Split(' ');
            myArray = new int[inputArray.Length];

            for (int i = 0; i < inputArray.Length; i++)
            {
                if (!int.TryParse(inputArray[i], out myArray[i]))
                {
                    Console.WriteLine("Некорректный ввод");
                    myArray = null;
                    break;
                }
            }
        } while (myArray == null || myArray.Length == 0);
        return myArray;
    }

    static int[] FindLongestSubsequence(int[] array)
    {
        int[] currentSubsequence = { array[0] };
        int[] longestSubsequence = { array[0] };

        for (int i = 1; i < array.Length; i++)
        {
            if (Math.Sign(array[i]) == Math.Sign(currentSubsequence.LastOrDefault()))
            {
                currentSubsequence = currentSubsequence.Concat(new[] { array[i] }).ToArray();
            }
            else
            {
                currentSubsequence = new[] { array[i] };
            }

            if (currentSubsequence.Length > longestSubsequence.Length)
            {
                longestSubsequence = currentSubsequence.ToArray();
            }
        }

        return longestSubsequence;
    }
}
