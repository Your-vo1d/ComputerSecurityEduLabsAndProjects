SELECT
    last_name,
    hire_date
FROM
    employees
WHERE
    hire_date BETWEEN '1997-01-01' AND '2000-12-31'
ORDER BY
    last_name