SELECT
    department_id,
    SUBSTRING(
        first_name
        FROM
            2
    ) AS name_without_first_char
FROM
    employees
WHERE
    UPPER(last_name) = 'HUNOLD'