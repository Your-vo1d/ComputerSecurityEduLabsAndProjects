SELECT 
    employee_id,
    first_name || ' ' || last_name AS "Full Name",
    salary,
    ROUND(salary * POWER(1.05, 6), 2) AS "Projected Salary in 6 Months",
    ROUND(salary * POWER(1.05, 6) - salary, 2) AS "Difference in Salary"
FROM 
    employees
WHERE 
    hire_date >= CURRENT_DATE - INTERVAL '30 years' 
    AND EXTRACT(MONTH FROM hire_date) BETWEEN 6 AND 8  
ORDER BY 
    employee_id
