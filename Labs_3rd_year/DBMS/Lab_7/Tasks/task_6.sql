SELECT
    E.EMPLOYEE_ID,
    E.FIRST_NAME || ' ' || E.LAST_NAME AS FULL_NAME,
    E.SALARY,
    DEPT_INFO.DEPARTMENT_NAME,
    DEPT_INFO.CITY
FROM
    EMPLOYEES E
    LEFT JOIN (
        SELECT
            D.DEPARTMENT_ID,
            D.DEPARTMENT_NAME,
            L.CITY
        FROM
            DEPARTMENTS D
            JOIN LOCATIONS L ON D.LOCATION_ID = L.LOCATION_ID
    ) AS DEPT_INFO ON E.DEPARTMENT_ID = DEPT_INFO.DEPARTMENT_ID