SELECT
    manager_id,
    COUNT(employee_id) AS employee_count,
    ROUND(AVG(salary), 2) AS avg_salary,
    MAX(salary) - MIN(salary) AS salary_diff
FROM
    employees
GROUP BY
    manager_id
HAVING
    AVG(salary) BETWEEN 3000
    AND 7000
ORDER BY
    avg_salary