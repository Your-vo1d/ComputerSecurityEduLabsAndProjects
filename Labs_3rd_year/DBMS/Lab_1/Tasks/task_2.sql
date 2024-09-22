SELECT
    first_name,
    last_name,
    manager_id,
    phone_number,
    email,
    salary
FROM
    employees
WHERE
    salary < 25000
ORDER BY
    salary DESC