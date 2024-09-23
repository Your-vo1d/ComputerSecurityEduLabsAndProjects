SELECT
    department_id,
    COUNT(employee_id) AS "Number of Employees",
    AVG(salary) AS "Average Salary",
    ROUND(MAX(salary), 2) AS "Max Salary",
    MIN(hire_date) AS "Earliest Hire Date"
FROM
    employees
GROUP BY
    department_id
HAVING
    AVG(salary) > 5000
ORDER BY
    "Average Salary" DESC