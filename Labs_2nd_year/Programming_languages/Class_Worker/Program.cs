using System;

class Worker
{
    // Поля класса
    private string name;
    private int age;
    private int experience;

    // Конструктор класса
    public Worker(string name, int age, int experience)
    {
        this.name = name;
        this.age = age;
        this.experience = experience;
    }

    public Worker(int age, int experience)
    {
        if (age < 14 || age > 100 || experience < 0 || experience > 100)
        {
            this.name = "";
            this.age = 0;
            this.experience = 0;
        }
        else
        {
            this.name = "";
            this.age = age;
            this.experience = experience;
        }
    }
    public Worker()
    {
        this.name = "";
        this.age = 0;
        this.experience = 0;
    }

    // Свойства для доступа к полям
    public string Name
    {
        get { return name; }
        set { name = value; }
    }

    public int Age
    {
        get { return age; }
        set { age = value; }
    }
    public int Experience
    {
        get { return experience; }
        set { experience = value; }
    }
    // Метод для вывода информации о работнике
    public void DisplayInfo()
    {
        Console.WriteLine($"Имя: {name}, Возраст: {age}, Стаж: {experience}");
    }
}

class Program
{
    static void Main()
    {
        // Пример использования класса
        Worker worker1 = new Worker("Иван", 25, 23);
        worker1.DisplayInfo();

        // Можно также использовать свойства для доступа и изменения значений полей
        Worker worker2 = new Worker("Мария", 30, 32);
        worker2.Age = 31;
        worker2.Experience = 11;
        worker2.DisplayInfo();
    }
}