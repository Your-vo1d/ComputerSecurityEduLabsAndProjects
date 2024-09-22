SELECT DISTINCT
    manager_id
FROM
    employees
WHERE
    hire_date <= '1997-01-01'
    AND phone_number IS NULL
ORDER BY
    manager_id DESC