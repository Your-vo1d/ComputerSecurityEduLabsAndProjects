SELECT
    m.employee_id AS manager_id,
    m.first_name || ' ' || m.last_name AS manager_name,
    SUM(e.salary) AS total_salary,
    round(AVG(e.salary), 2) AS avg_salary
FROM
    employees e
JOIN
    employees m ON e.manager_id = m.employee_id
GROUP BY
    m.employee_id, m.first_name, m.last_name
ORDER BY
    avg_salary DESC;
