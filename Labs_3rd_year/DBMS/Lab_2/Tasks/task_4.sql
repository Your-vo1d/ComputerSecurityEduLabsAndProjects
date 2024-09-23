SELECT
    first_name,
    last_name,
    salary * 12 AS annual_salary,
    ROUND(salary / 20, 2) AS daily_salary,
    ROUND(salary * LOG (salary)) AS bonus
FROM
    employees
WHERE
    (
        CHAR_LENGTH(first_name) = 5
        OR CHAR_LENGTH(last_name) = 5
    )
    AND salary / 20 <= 400