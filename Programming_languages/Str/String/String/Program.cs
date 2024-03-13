using System;
using System.Text;
using System.Text.RegularExpressions;

class Program
{
    static void Main()
    {

        Console.Write("Введите строку: ");
        string inputString = Console.ReadLine();

        Console.Write("Введите символ: ");
        char searchChar = Console.ReadLine()[0];
        Console.WriteLine(searchChar);
        int count = CountOccurrences(inputString, searchChar);
        Console.WriteLine($"\nСимвол '{searchChar}' встречается в строке {count} раз.");

        string tempText = RemoveDigits(inputString);
        Console.WriteLine("Результат удаления цифр: " + tempText);

        if (CheckKey(inputString))
        {
            Console.WriteLine("Строка содержит все символы слова key.");
        }
        else
        {
            Console.WriteLine("Строка не содержит все символы слова key.");
        }
        if (CheckParentheses(inputString))
        {
            Console.WriteLine("Скобки стоят корректно.");
        }
        else
        {
            Console.WriteLine("Скобки стоят некорректно.");
        }


    }

    static int CountOccurrences(string str, char searchChar)
    {
        int count = 0;
        foreach (char c in str)
        {
            if (c == searchChar)
            {
                count++;
            }
        }
        return count;
    }

    static bool CheckKey(string str)
    {
        string temp = str.ToLower();
        foreach (char item in "key")
        {
            if (temp.IndexOf(item) == -1)
                return false;
        }
        return true;
    }

    static bool CheckParentheses(string text)
    {
        Stack<char> stack = new Stack<char>();

        foreach (char symbol in text)
        {
            if (symbol == '(')
            {
                stack.Push(symbol);
            }
            else if (symbol == ')')
            {
                if (stack.Count == 0 || stack.Pop() != '(')
                {
                    return false;
                }
            }
        }
        return true;
    }

    static string RemoveDigits(string input)
    {
        string result = Regex.Replace(input, "[0-9]", ""); ;

        return result;
    }

}
