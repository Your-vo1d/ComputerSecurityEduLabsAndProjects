SELECT
    last_name || ' ' || UPPER(
        SUBSTRING(
            first_name
            FROM
                1 FOR 1
        )
    ) || '.' AS surname_initial,
    salary
FROM
    employees
WHERE
    salary * 6 > 50000
ORDER BY
    last_name