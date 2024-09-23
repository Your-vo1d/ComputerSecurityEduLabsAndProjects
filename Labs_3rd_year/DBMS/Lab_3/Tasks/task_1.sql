SELECT
    last_name || ' ' || first_name as full_name,
    job_id
FROM
    employees
WHERE
    DATE_PART (
        'day',
        (
            DATE_TRUNC ('month', hire_date + INTERVAL '1 month') - hire_date
        )
    ) < 10