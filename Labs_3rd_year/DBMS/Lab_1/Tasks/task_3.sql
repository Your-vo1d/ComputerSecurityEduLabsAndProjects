SELECT
    last_name,
    first_name
FROM
    employees
WHERE
    (
        first_name LIKE '%l%l%'
        AND first_name LIKE '%a%'
    )
    OR email SIMILAR TO '%!_%' ESCAPE '!'