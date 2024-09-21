using System;
using System.Numerics;

class Vec
{
    int x, y; // Поля класса для хранения координат

    // Конструктор класса с двумя параметрами
    public Vec(int x, int y)
    {
        this.x = x;
        this.y = y;
    }

    // Конструктор класса с одним параметром (по умолчанию y=0)
    public Vec(int x)
    {
        this.x = x;
        this.y = 0;
    }

    // Конструктор класса без параметров (координаты по умолчанию (0,0))
    public Vec()
    {
        this.x = 0;
        this.y = 0;
    }

    // Перегрузка оператора "-" для вычитания векторов
    public static Vec operator -(Vec a, Vec b)
    {
        Vec result = new Vec(a.x - b.x, a.y - b.y);
        return result;
    }

    // Перегрузка оператора "==" для сравнения векторов на равенство
    public static bool operator ==(Vec a, Vec b)
    {
        return a.x == b.x && a.y == b.y;
    }

    // Перегрузка оператора "!=" для сравнения векторов на неравенство
    public static bool operator !=(Vec a, Vec b)
    {
        return !(a == b);
    }

    // Перегрузка оператора "*" для умножения вектора на скаляр
    public static Vec operator *(Vec a, int n)
    {
        return new Vec(a.x * n, a.y * n);
    }
    // Перегрузка оператора "*" для скалярного произведения векторов
    public static int operator *(Vec a, Vec b)
    {
        return (a.x * b.x) + (a.y * b.y);
    }

    // Переопределение метода ToString для корректного вывода вектора в виде строки
    public override string ToString()
    {
        return $"({x}, {y})";
    }

    public static Vec InputVector()
    {
        Console.WriteLine("Enter x:");
        int x = int.Parse(Console.ReadLine());

        Console.WriteLine("Enter y:");
        int y = int.Parse(Console.ReadLine());

        return new Vec(x, y);
    }
}

class Solution
{
    static void Main(string[] args)
    {
        Vec v1 = new Vec(); // Создаем объект класса Vec
        Console.WriteLine("Vector №1: " + v1); // Выводим объект класса Vec

        Vec v2 = new Vec(1);
        Console.WriteLine("Vector №2: " + v2);

        Vec v3 = new Vec(-2, 3);
        Console.WriteLine("Vector №3: " + v3);

        Vec v4 = new Vec(1, 0);
        Console.WriteLine("Vector №4: " + v4);

        Vec v5 = new Vec(2, 3);
        Console.WriteLine("Vector №5: " + v5);

        Vec inpVec1 = Vec.InputVector();
        Console.WriteLine("The introduced vector: " + inpVec1);

        Vec inpVec2 = Vec.InputVector();
        Console.WriteLine("The introduced vector: " + inpVec2);

        if (inpVec1 == inpVec2)
        {
            Console.WriteLine("The introdused vector 1 and 2 are equal.");
        }
        else
        {
            Console.WriteLine("The introdused vector 1 and 2 aren't equal.");
        }

        if (v2 != v3)
        {
            Console.WriteLine("The introdused vector 1 and 2 aren't equal.");
        }
        else
        {
            Console.WriteLine("The introdused vector 1 and 2 are equal.");
        }

        Vec result = new Vec();
        int mult = 0;
        while (!(int.TryParse(Console.ReadLine(), out mult)))
        {
            Console.WriteLine("Error. Try again.");
        }
        result = inpVec1 * mult;
        Console.WriteLine("The result is the multiplication of a introdused vector 1 by a number : " + result);
        int resultVV = inpVec1 * inpVec2;
        Console.WriteLine("The result is the multiplication of a introdused vector 1 and 2: " + resultVV);

    }
}
