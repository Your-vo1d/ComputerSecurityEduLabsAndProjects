SELECT 
	COUNT (employee_id), 
	job_id,
	AVG(salary) AS avarage_salary
FROM 
	employees 
GROUP BY 
	job_id 
HAVING
	AVG(salary) BETWEEN 500 AND 4000

