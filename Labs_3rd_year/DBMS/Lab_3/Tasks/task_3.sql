SELECT
    last_name || '-' || phone_number as last_name_with_phone_number,
    to_char (hire_date, 'DD MON YYYY') AS formated_date,
    EXTRACT(
        YEAR
        FROM
            AGE (NOW (), hire_date)
    ) * 12 + EXTRACT(
        MONTH
        FROM
            AGE (NOW (), hire_date)
    ) as months_difference
FROM
    employees