SELECT
    last_name,
    first_name,
    email,
    salary
FROM
    employees
WHERE
    (
        (
            first_name LIKE '%l%l%'
            AND first_name LIKE '%a%'
        )
        OR email SIMILAR TO '%!_%' ESCAPE '!'
    )
    and salary < 10000
ORDER BY
    first_name DESC