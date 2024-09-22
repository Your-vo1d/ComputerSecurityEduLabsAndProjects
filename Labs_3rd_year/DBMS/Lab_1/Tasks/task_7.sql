SELECT
    last_name,
    employee_id,
    manager_id,
    department_id,
    hire_date AS "Дата найма"
FROM
    employees
WHERE
    manager_id IN (100, 114, 108)
    AND department_id NOT IN (5, 8)
ORDER BY
    last_name