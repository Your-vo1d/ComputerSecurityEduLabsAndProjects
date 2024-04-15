import math


def N(t):
    result = 172/45*(math.pi/2-math.atan((2000-t)/45))
    return result


years = [1000, 1750, 1800, 1850, 1900, 1950, 1955, 
         1960, 1965, 1970, 1975, 1980, 1985, 1990, 1995, 
         2000, 2005, 2010, 2011, 2012, 2013, 2014, 2015,
         2016, 2017, 2018, 2019]

population =[0.400, 0.791, 1.000, 1.262, 1.650, 2.519,
             2.756, 3.021, 3.335, 3.692, 4.068, 4.435, 4.831,
             5.264, 5.674, 6.071, 6.344, 6.933, 7.015, 7.100,
             7.162, 7.271, 7.358, 7.444, 7.530, 7.669, 7.763]  

population_computing = [N(i) for i in years]

error_list = [
    abs((population[i] - population_computing[i]) / population[i])
    for i in range(len(population))
]
start = int(input())
end = int(input())
min_error = min(error_list)

index_min_error = error_list.index(min_error)

error_list = error_list[start:end + 1]
years = years[start:end + 1]
max_error = max(error_list)
index_max_error = error_list.index(max_error)

min_error = min(error_list)

index_min_error = error_list.index(min_error)

avg_error = sum(error_list) / len(error_list)

print("Погрешность - минимальная, год: %4d, максимальная, год: %4d, средняя, процент: %6.3f"%(years[index_min_error], years[index_max_error], avg_error * 100))