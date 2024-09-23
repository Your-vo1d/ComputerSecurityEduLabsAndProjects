SELECT
    last_name || ' ' || first_name AS full_name,
    salary,
    ROUND(LOG (salary) * POWER(salary, 0.5), 2) AS complex_bonus
FROM
    employees
WHERE
    salary > 5000
ORDER BY
    complex_bonus DESC