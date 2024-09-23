SELECT
    UPPER(last_name) || ' ' || hire_date as last_name_with_hire_date,
    to_char (hire_date, 'DAY - MONTH') AS formated_date
FROM
    employees
WHERE
    job_id BETWEEN 14
    AND 19
    OR salary BETWEEN 9000
    AND 25000