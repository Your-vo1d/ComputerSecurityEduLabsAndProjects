    employee_id,
    first_name || ' ' || last_name AS "Full Name",  
    salary,
    ROUND(log10(salary) * 100, 2) AS bonus, 
    CURRENT_DATE - hire_date AS "Days Since Hire",  
    (hire_date + (CEIL(EXTRACT(YEAR FROM AGE(CURRENT_DATE, hire_date)) / 5.0) * 5 || ' years')::interval)::date AS "Next Milestone Anniversary" 
FROM
    employees
WHERE 
    salary > 5000  
ORDER BY 
    EXTRACT(YEAR FROM AGE(CURRENT_DATE, hire_date)) DESC
