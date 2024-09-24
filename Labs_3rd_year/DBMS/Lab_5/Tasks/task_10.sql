SELECT
    departments.department_id,
    departments.department_name,
    regions.region_name,
    MAX(employees.salary) AS max_salary,
    STRING_AGG(
        employees.last_name || SUBSTRING(
            employees.first_name
            FROM
                1 FOR 1
        ) || '.',
        ','
    ) AS employee_names
FROM
    departments
    left JOIN locations ON locations.location_id = departments.location_id
    JOIN countries ON countries.country_id = locations.country_id
    JOIN regions ON regions.region_id = countries.region_id
    LEFT JOIN employees ON employees.department_id = departments.department_id
GROUP BY
    departments.department_id,
    regions.region_name