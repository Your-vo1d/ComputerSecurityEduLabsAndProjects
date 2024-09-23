SELECT
	manager_id,
	COUNT(employee_id) AS count_employees,
	STRING_AGG(
		first_name || ' ' || last_name || '-' || salary,
		', '
	)
FROM
	employees
WHERE
	manager_id is not null
GROUP BY
	manager_id