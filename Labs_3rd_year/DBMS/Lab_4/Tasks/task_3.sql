SELECT
    STRING_AGG(
        last_name || '//' || job_id || '//' || salary,
        '//'
    ) AS employee_info,
    MIN(salary) AS min_salary,
    MAX(salary) AS max_salary,
    AVG(salary) AS avg_salary
FROM
    employees
GROUP BY
    department_id