SELECT
    DEPARTMENTS.DEPARTMENT_NAME,
    COUNTRIES.COUNTRY_NAME,
    LOCATIONS.CITY
FROM
    DEPARTMENTS
    LEFT JOIN LOCATIONS ON LOCATIONS.LOCATION_ID = DEPARTMENTS.LOCATION_ID
    LEFT JOIN COUNTRIES ON COUNTRIES.COUNTRY_ID = LOCATIONS.COUNTRY_ID
    LEFT JOIN EMPLOYEES ON DEPARTMENTS.DEPARTMENT_ID = EMPLOYEES.DEPARTMENT_ID
GROUP BY
    COUNTRIES.COUNTRY_NAME,
    DEPARTMENTS.DEPARTMENT_NAME,
    LOCATIONS.CITY
HAVING
    COUNT(EMPLOYEE_ID) > 2