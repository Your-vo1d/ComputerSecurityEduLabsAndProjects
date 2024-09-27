SELECT
	E.EMPLOYEE_ID,
	E.FIRST_NAME,
	E.LAST_NAME,
	E.HIRE_DATE,
	(CURRENT_DATE - E.HIRE_DATE) AS DAYS_OF_SERVICE,
	ROUND(DEPT_AVG.AVG_DAYS_OF_SERVICE),
	ROUND(
		(
			(CURRENT_DATE - E.HIRE_DATE) - DEPT_AVG.AVG_DAYS_OF_SERVICE
		)
	) AS DIFFERENCE_IN_DAYS
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
	(CURRENT_DATE - E.HIRE_DATE) > DEPT_AVG.AVG_DAYS_OF_SERVICE;