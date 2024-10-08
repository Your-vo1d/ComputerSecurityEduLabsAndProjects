SELECT
	E.FIRST_NAME || ' ' || E.LAST_NAME AS FULL_NAME,
	E.SALARY,
	ROUND(M.AVG_SALARY, 2),
	E.MANAGER_ID
FROM
	EMPLOYEES E
	JOIN (
		SELECT
			MANAGER_ID,
			AVG(SALARY) AS AVG_SALARY
		FROM
			EMPLOYEES
		WHERE
			MANAGER_ID IS NOT NULL
		GROUP BY
			MANAGER_ID
	) M ON E.MANAGER_ID = M.MANAGER_ID
WHERE
	E.SALARY > M.AVG_SALARY
