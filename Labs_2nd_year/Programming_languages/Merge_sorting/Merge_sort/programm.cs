using System;
using System.IO;

class NaturalMergeSort
{
	static void Main()
	{
		string inputFileName = "input.txt";
		string tempFileName1 = "temp1.txt";
		string tempFileName2 = "temp2.txt";

		// Генерация тестового файла
		GenerateTestData(inputFileName, 100);

		// Сортировка естественным слиянием
		NaturalMergeSortFiles(inputFileName, tempFileName1, tempFileName2);

		Console.WriteLine("Сортировка завершена.");
		Console.WriteLine();
	}

	static void NaturalMergeSortFiles(string inputFile, string tempFile1, string tempFile2)
	{
		// Пока не достигнем конца файла
		while (true)
		{
			if (!SplitFile(inputFile, tempFile1, tempFile2))
				break;
			MergeFiles(tempFile1, tempFile2, inputFile);
		}

	}

	static bool SplitFile(string inputFile, string outputFile, string tempFile)
	{
		using (var reader = new StreamReader(inputFile))
		using (var writer = new StreamWriter(outputFile, false))
		using (var writer2 = new StreamWriter(tempFile, false))
		{
			string line = reader.ReadLine();
			if (line == null)
				return false;
			string lastLine = line;
			bool flag = true;
			while (line != null)
			{
				int numberLine = int.Parse(line);
				int numberLastLine = int.Parse(lastLine);
				if (numberLine < numberLastLine)
					flag = !flag;
				if (flag)
				{
					writer.WriteLine(line);
				}
				else
				{
					writer2.WriteLine(line);
				}
				lastLine = line;
				line = reader.ReadLine();
			}
			writer.Close();
			writer2.Close();
			FileInfo fileInfo = new FileInfo(tempFile);
        	return !(fileInfo.Length == 0);
			
		}
	}

	static void MergeFiles(string inputFile1, string inputFile2, string outputFile)
	{
		using (var reader1 = new StreamReader(inputFile1))
		using (var reader2 = new StreamReader(inputFile2))
		using (var writer = new StreamWriter(outputFile))
		{
			string line1 = reader1.ReadLine();
			string line2 = reader2.ReadLine();
			while (line1 != null && line2 != null)
			{
				int numberLine1 = int.Parse(line1);
				int numberLine2 = int.Parse(line2);
				if (numberLine1 < numberLine2)
				{
					writer.WriteLine(line1);
					line1 = reader1.ReadLine();
				}
				else
				{
					writer.WriteLine(line2);
					line2 = reader2.ReadLine();
				}
			}

			// Дописываем оставшиеся элементы
			while (line1 != null)
			{
				writer.WriteLine(line1);
				line1 = reader1.ReadLine();
			}

			while (line2 != null)
			{
				writer.WriteLine(line2);
				line2 = reader2.ReadLine();
			}
		writer.Close();
		}
	}

	static void GenerateTestData(string fileName, int count)
	{
		Random random = new Random();
		using (var writer = new StreamWriter(fileName))
		{
			for (int i = 0; i < count; i++)
			{
				writer.WriteLine(random.Next(1000));
			}
		}
	}
}
