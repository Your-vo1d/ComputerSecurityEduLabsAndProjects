using System;

// Интерфейс для геометрических фигур
interface IFigure
{
    double Area();               // Площадь фигуры
    double CircumcircleRadius(); // Радиус описанной окружности
}

// Класс прямоугольника, реализующий интерфейс IFigure
class Rectangle : IFigure
{
    // Координаты верхнего левого угла прямоугольника (x, y)
    private double x;
    private double y;

    // Длины сторон прямоугольника
    private double a;
    private double b;

    // Конструктор для инициализации полей
    public Rectangle(double x = 0, double y = 0, double a = 1, double b = 1)
    {
        this.x = x;
        this.y = y;
        this.a = a;
        this.b = b;
    }

    // Реализация метода Area интерфейса IFigure
    public double Area()
    {
        return a * b;
    }

    // Реализация метода CircumcircleRadius интерфейса IFigure
    public double CircumcircleRadius()
    {
        return Math.Sqrt(a * a + b * b) / 2;
    }

    // Проверка, является ли прямоугольник квадратом
    public bool IsSquare()
    {
        return a == b;
    }

    // Перегрузка оператора умножения для прямоугольника
    public static Rectangle operator *(Rectangle rect, double number)
    {
        return new Rectangle(rect.x, rect.y, rect.a * number, rect.b * number);
    }

    // Проверка, находится ли прямоугольник в первой четверти координатной плоскости
    public static bool InPositiveQuarter(Rectangle rect)
    {
        return rect.x >= 0 && rect.y >= 0;
    }

    // Проверка равенства двух прямоугольников
    public static bool operator ==(Rectangle rect1, Rectangle rect2)
    {
        if (ReferenceEquals(rect1, null) || ReferenceEquals(rect2, null))
        {
            return false;
        }

        return rect1.a == rect2.a && rect1.b == rect2.b;
    }

    // Проверка неравенства двух прямоугольников
    public static bool operator !=(Rectangle rect1, Rectangle rect2)
    {
        return !(rect1 == rect2);
    }

    // Проверка пересечения прямоугольника с другим прямоугольником
    public bool Intersects(Rectangle other)
    {
        return !(x + a <= other.x || other.x + other.a <= x || y + b <= other.y || other.y + other.b <= y);
    }

    // Проверка пересечения прямоугольника с прямой заданными уравнением y = mx + c
    public bool IntersectsLine(double m, double c)
    {
        double top = y + b;
        double bottom = y;
        double left = x;
        double right = x + a;

        double topIntersection = m * left + c;
        double bottomIntersection = m * right + c;

        if ((topIntersection >= bottom && topIntersection <= top) || (bottomIntersection >= bottom && bottomIntersection <= top))
        {
            return true;
        }

        double leftIntersection = (top - c) / m;
        double rightIntersection = (bottom - c) / m;

        return (leftIntersection >= left && leftIntersection <= right) || (rightIntersection >= left && rightIntersection <= right);
    }

    // Ввод данных прямоугольника с консоли
    public void Input()
    {
        Console.WriteLine("Enter coordinates of point A:");
        x = ReadDouble("x: ");
        y = ReadDouble("y: ");

        a = ReadPositiveDouble("Enter side a: ");
        b = ReadPositiveDouble("Enter side b: ");
    }

    // Вывод информации о прямоугольнике
    public void Output()
    {
        Console.WriteLine(this);
        Console.WriteLine($"Area: {Area()}");
        Console.WriteLine($"Circumcircle radius: {CircumcircleRadius()}");
        Console.WriteLine($"Is square: {IsSquare()}");
        Console.WriteLine();
    }

    // Статический метод для чтения положительного числа с консоли
    private static double ReadPositiveDouble(string prompt)
    {
        double result;
        bool isValid;

        do
        {
            result = ReadDouble(prompt);
            isValid = result > 0;

            if (!isValid)
            {
                Console.WriteLine("Invalid input. Please enter a positive number.");
            }
        } while (!isValid);

        return result;
    }

    // Статический метод для чтения числа с консоли
    private static double ReadDouble(string prompt)
    {
        double result;
        bool isValid;

        do
        {
            Console.Write(prompt);
            isValid = double.TryParse(Console.ReadLine(), out result);

            if (!isValid)
            {
                Console.WriteLine("Invalid input. Please enter a valid number.");
            }
        } while (!isValid);

        return result;
    }

    // Переопределение метода ToString для прямоугольника
    public override string ToString()
    {
        return $"Rectangle: (x={x}, y={y}), a={a}, b={b}";
    }
}

// Основной класс программы
class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Rectangle 0:");
        Rectangle rect0 = new Rectangle();
        rect0.Output();

        Console.WriteLine("Rectangle 1:");
        Rectangle rect1 = new Rectangle();
        rect1.Input();
        rect1.Output();

        Console.WriteLine("\nRectangle 2:");
        Rectangle rect2 = new Rectangle();
        rect2.Input();
        rect2.Output();

        Console.WriteLine("\nRectangle 1 == Rectangle 2: " + (rect1 == rect2));
        Console.WriteLine("Rectangle 1 != Rectangle 2: " + (rect1 != rect2));
        Console.WriteLine("Rectangle 1 is square: " + rect1.IsSquare());
        Console.WriteLine("Rectangle 2 is square: " + rect2.IsSquare());
        Console.WriteLine("Rectangle 1 Intersects Rectangle 2: " + rect1.Intersects(rect2));

        Console.WriteLine("\nEnter line parameters for intersection test:");
        double m = ReadDouble("Enter slope (m): ");
        double c = ReadDouble("Enter intercept (c): ");

        Console.WriteLine("Rectangle 1 intersects the line: " + rect1.IntersectsLine(m, c));
        Console.WriteLine("Rectangle 2 intersects the line: " + rect2.IntersectsLine(m, c));

        Console.WriteLine("\nEnter a number to multiply Rectangle 1:");
        double multiplier = ReadDouble("Enter number: ");
        Rectangle rect1TimesMultiplier = rect1 * multiplier;
        rect1TimesMultiplier.Output();

        Console.WriteLine("\nRectangle 1 is in positive quarter: " + (Rectangle.InPositiveQuarter(rect1) ? "Yes" : "No"));
        Console.ReadLine();
    }

    // Статический метод для чтения числа с консоли
    private static double ReadDouble(string prompt)
    {
        double result;
        bool isValid;

        do
        {
            Console.Write(prompt);
            isValid = double.TryParse(Console.ReadLine(), out result);

            if (!isValid)
            {
                Console.WriteLine("Invalid input. Please enter a valid number.");
            }
        } while (!isValid);

        return result;
    }
}
