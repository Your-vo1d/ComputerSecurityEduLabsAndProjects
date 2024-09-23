SELECT
    department_id,
    ROUND(AVG(salary), 2) AS avg_salary
FROM
    employees
GROUP BY
    department_id
HAVING
    COUNT(employee_id) > 5
ORDER BY
    avg_salary DESC