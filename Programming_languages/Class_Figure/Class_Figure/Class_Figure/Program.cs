using System;

// Абстрактный класс Figure (фигура)
abstract class Figure
{
    public abstract double square(); // Абстрактный метод для вычисления площади
    public abstract double perimeter(); // Абстрактный метод для вычисления периметра
}

// Производный класс Triangle (треугольник), который наследует от Figure
class Triangle : Figure
{
    protected double side_1, side_2, side_3;

    public Triangle(double side1, double side2, double side3)
    {
        side_1 = side1;
        side_2 = side2;
        side_3 = side3;
    }

    public override double square()
    {
        // Формула Герона для вычисления площади треугольника
        double p = (side_1 + side_2 + side_3) / 2;
        return Math.Sqrt(p * (p - side_1) * (p - side_2) * (p - side_3));
    }

    public override double perimeter()
    {
        return side_1 + side_2 + side_3;
    }
}

// Производный класс Right_Triangle (прямоугольный треугольник), который наследует от Triangle
class Right_Triangle : Triangle
{
    public Right_Triangle(double side1, double side2) : base(side1, side2, Math.Sqrt(side1 * side1 + side2 * side2))
    {
        // Сторона side_3 вычисляется по теореме Пифагора
    }
    // Конструктор класса с тремя параметрами
    public Right_Triangle(double side1, double side2, double hypotenuse) : base(side1, side2, hypotenuse)
    {
        if (!IsRightTriangle(side1, side2, hypotenuse))
        {
            Console.WriteLine("Неверные параметры для прямоугольного треугольника.");
            hypotenuse = Math.Sqrt(side1 * side1 + side2 * side2);
        }
    }

    // Метод для проверки является ли треугольник прямоугольным
    private bool IsRightTriangle(double a, double b, double c)
    {
        // Треугольник является прямоугольным, если сумма квадратов двух меньших сторон равна квадрату большей стороны
        double maxSide = Math.Max(Math.Max(a, b), c);
        double sumOfSquares = (a == maxSide ? b * b + c * c : (b == maxSide ? a * a + c * c : a * a + b * b));
        return maxSide * maxSide == sumOfSquares;
    }
}

// Производный класс Circle (круг), который наследует от Figure
class Circle : Figure
{
    double radius;

    public Circle(double r)
    {
        radius = r;
    }

    public override double square()
    {
        return Math.PI * radius * radius; // Формула для площади круга
    }

    public override double perimeter()
    {
        return 2 * Math.PI * radius; // Формула для периметра круга
    }
}

class Program
{
    static double ReadDouble(string message)
    {
        double result;
        while (true)
        {
            Console.Write(message);
            if (double.TryParse(Console.ReadLine(), out result) && result > 0)
                return result;
            Console.WriteLine("Ошибка ввода. Пожалуйста, введите положительное число.");
        }
    }

    static void Main(string[] args)
    {
        double side1, side2, side3, radius;

        Console.WriteLine("Введите параметры треугольника:");
        side1 = ReadDouble("Сторона 1: ");
        side2 = ReadDouble("Сторона 2: ");
        side3 = ReadDouble("Сторона 3: ");

        // Проверка на существование треугольника
        if (side1 + side2 <= side3 || side1 + side3 <= side2 || side2 + side3 <= side1)
        {
            Console.WriteLine("Треугольник с такими сторонами не существует.");
        }
        else
        {
            Triangle triangle = new Triangle(side1, side2, side3);
            Console.WriteLine("Площадь треугольника: " + triangle.square());
            Console.WriteLine("Периметр треугольника: " + triangle.perimeter());
        }

        Console.WriteLine("\nВведите параметры прямоугольного треугольника:");
        side1 = ReadDouble("Катет 1: ");
        side2 = ReadDouble("Катет 2: ");
        double hypotenuse = ReadDouble("Гипотенуза: ");

        Right_Triangle rightTriangle = new Right_Triangle(side1, side2, hypotenuse);
        Console.WriteLine("Площадь прямоугольного треугольника: " + rightTriangle.square());
        Console.WriteLine("Периметр прямоугольного треугольника: " + rightTriangle.perimeter());

        Console.WriteLine("\nВведите радиус круга:");
        radius = ReadDouble("Радиус: ");
        Circle circle = new Circle(radius);
        Console.WriteLine("Площадь круга: " + circle.square());
        Console.WriteLine("Периметр круга: " + circle.perimeter());
    }
}
