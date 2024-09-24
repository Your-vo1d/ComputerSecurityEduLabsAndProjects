SELECT
    employees.first_name,
    employees.last_name,
    departments.department_name,
    locations.city
FROM
    employees
    JOIN departments ON employees.department_id = departments.department_id
    JOIN locations ON departments.location_id = locations.location_id
WHERE
    departments.department_name = 'Sales'
    AND locations.city IN ('Seatle', 'Oxford', 'Toronto')