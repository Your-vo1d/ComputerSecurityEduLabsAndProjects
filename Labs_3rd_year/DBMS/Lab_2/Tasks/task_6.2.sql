SELECT
    last_name || ' ' || first_name AS full_name,
    salary AS actual_salary,
    50000 AS planned_salary,
    ABS(salary - 50000) AS salary_difference
FROM
    employees
ORDER BY
    salary_difference