SELECT
    m.employee_id AS manager_id,
    m.first_name || ' ' || m.last_name AS manager_name,
    (SELECT SUM(e.salary)
     FROM employees e
     WHERE e.manager_id = m.employee_id) AS total_salary,
    (SELECT round(AVG(e.salary), 2)
     FROM employees e
     WHERE e.manager_id = m.employee_id) AS avg_salary
FROM
    employees m
WHERE
    EXISTS (SELECT 1 FROM employees e WHERE e.manager_id = m.employee_id)
ORDER BY
    avg_salary DESC;
