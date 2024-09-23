SELECT
    last_name || ' ' || SUBSTRING(
        first_name
        FROM
            1 FOR 1
    ) AS last_name_with_first_char,
    TO_CHAR (hire_date, 'J') AS julian_day
FROM
    employees
ORDER BY
    last_name DESC