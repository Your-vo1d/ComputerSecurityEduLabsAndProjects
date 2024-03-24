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
            throw new ArgumentException("Неверные параметры для прямоугольного треугольника.");
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
    static void Main(string[] args)
    {
        // Пример использования классов
        Triangle triangle = new Triangle(3, 4, 5);
        Console.WriteLine("Площадь треугольника: " + triangle.square());
        Console.WriteLine("Периметр треугольника: " + triangle.perimeter());

        Right_Triangle rightTriangle = new Right_Triangle(3, 4);
        Console.WriteLine("Площадь прямоугольного треугольника: " + rightTriangle.square());
        Console.WriteLine("Периметр прямоугольного треугольника: " + rightTriangle.perimeter());

        Circle circle = new Circle(5);
        Console.WriteLine("Площадь круга: " + circle.square());
        Console.WriteLine("Периметр круга: " + circle.perimeter());
    }
}
