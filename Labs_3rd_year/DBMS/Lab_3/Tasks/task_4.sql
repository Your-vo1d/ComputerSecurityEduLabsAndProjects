SELECT
    first_name || ' ' || last_name AS full_name,
    hire_date
FROM
    employees
WHERE
    EXTRACT(
        YEAR
        FROM
            hire_date
    ) BETWEEN 1990
    AND 1995
    and EXTRACT(
        YEAR
        FROM
            current_date
    ) - EXTRACT(
        YEAR
        FROM
            hire_date
    ) > 30