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
        Console.WriteLine("Введите координату x:");
        int x = int.Parse(Console.ReadLine());

        Console.WriteLine("Введите координату y:");
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

        if (v2 == v4)
        {
            Console.WriteLine("The vector 2 and 4 are equal.");
        }
        else
        {
            Console.WriteLine("The vector 2 and 4 aren't equal.");
        }
        
        if (v2 != v3)
        {
            Console.WriteLine("The vector 2 and 3 aren't equal.");
        }
        else
        {
            Console.WriteLine("The vector 2 and 3 are equal.");
        }
        Vec result = new Vec();
        result = v3 * 5;
        Console.WriteLine("The result is the multiplication of a vector №3 by a number (5): " + result);
        int resultVV = v5 * v3;
        Console.WriteLine("The result is the multiplication of a vector №5 by a vector №3: " + resultVV);

        Vec inpVec = Vec.InputVector();
        Console.WriteLine("The introduced vector: " + inpVec);
    }
}
