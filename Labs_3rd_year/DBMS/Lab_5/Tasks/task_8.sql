SELECT
    REGIONS.REGION_NAME
FROM
    REGIONS
    LEFT JOIN COUNTRIES ON COUNTRIES.REGION_ID = REGIONS.REGION_ID
    LEFT JOIN LOCATIONS ON LOCATIONS.COUNTRY_ID = COUNTRIES.COUNTRY_ID
    LEFT JOIN DEPARTMENTS ON DEPARTMENTS.LOCATION_ID = LOCATIONS.LOCATION_ID
GROUP BY
    REGIONS.REGION_NAME
HAVING
    COUNT(DEPARTMENT_ID) = 0