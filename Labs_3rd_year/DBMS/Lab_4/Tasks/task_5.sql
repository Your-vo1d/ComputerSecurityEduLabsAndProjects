SELECT
    department_id || '-' || COUNT(employee_id) AS department_with_count,
    ROUND(AVG(salary), 2) AS average_salary,
    STRING_AGG(first_name || ' ' || last_name, ', ') AS full_name
FROM
    employees
GROUP BY
    department_id
HAVING
    AVG(salary) < 6000