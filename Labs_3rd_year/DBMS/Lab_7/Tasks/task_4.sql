SELECT
	E.FIRST_NAME,
	E.LAST_NAME
FROM
	EMPLOYEES E
	JOIN (
		SELECT
			DEPARTMENT_ID,
			AVG(CURRENT_DATE - HIRE_DATE) AS AVG_DAYS_OF_SERVICE
		FROM
			EMPLOYEES
		GROUP BY
			DEPARTMENT_ID
	) AS DEPT_AVG ON E.DEPARTMENT_ID = DEPT_AVG.DEPARTMENT_ID
WHERE
	(CURRENT_DATE - E.HIRE_DATE) > DEPT_AVG.AVG_DAYS_OF_SERVICE