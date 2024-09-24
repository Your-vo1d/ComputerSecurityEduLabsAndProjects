SELECT
    employees.first_name,
    employees.last_name,
    departments.department_name,
    locations.city
FROM
    employees,
    departments,
    locations
WHERE
    employees.department_id = departments.department_id
    AND departments.location_id = locations.location_id
    AND departments.department_name = 'Sales'
    AND (
        locations.city = 'Seatle'
        OR locations.city = 'Oxford'
        OR locations.city = 'Toronto'
    )