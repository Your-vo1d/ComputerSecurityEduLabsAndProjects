SELECT
    last_name,
    first_name
FROM
    employees
WHERE
    first_name like '%oo%'
    OR last_name SIMILAR TO '%o{2}%'