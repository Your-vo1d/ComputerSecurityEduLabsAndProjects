SELECT
    employees.first_name,
    employees.last_name,
    departments.department_name,
    locations.city,
    regions.region_name
FROM
    departments
    LEFT JOIN employees ON departments.department_id = employees.department_id
    RIGHT JOIN locations ON departments.location_id = locations.location_id
    LEFT JOIN countries ON locations.country_id = countries.country_id
    LEFT JOIN regions ON countries.region_id = regions.region_id
WHERE
    departments.department_name LIKE 'S%'
GROUP BY
    departments.department_id,
    locations.city,
    regions.region_name,
    employees.first_name,
    employees.last_name
HAVING
    AVG(employees.salary) > 5400