SELECT 
	hire_date,
	TO_DATE((TO_DATE(DATE_PART('year', hire_date + interval '1 year')::text, 'YYYY-MM-DD') + interval'24 day')::text,'YYYY-MM-DD') AS next_25_jan,
	TO_CHAR((TO_DATE(DATE_PART('year', hire_date + interval '1 year')::text, 'YYYY-MM-DD') + interval'24 day'),'DAY') AS day_next_25_jan
FROM employees
