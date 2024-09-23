SELECT
    LOWER(last_name) as lower_last_name,
    ROUND(salary / 4, 2) AS weekly_salary
FROM
    employees
WHERE
    UPPER(first_name) != 'STEVEN'
ORDER BY
    weekly_salary DESC