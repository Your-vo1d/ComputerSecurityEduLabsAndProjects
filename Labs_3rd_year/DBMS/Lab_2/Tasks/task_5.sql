SELECT
    CONCAT (last_name, ' ', first_name) AS full_name_format_1,
    CONCAT (
        round(salary / 20, 0),
        ' руб. ',
        round(((salary / 20.0) - salary / 20) * 100, 0),
        ' коп. '
    ) as daily_salary_format_1,
    last_name || ' ' || first_name AS full_name_format_2,
    round(salary / 20, 0) || ' руб. ' || round(((salary / 20.0) - salary / 20) * 100, 0) || ' коп. ' AS daily_salary_format_2
FROM
    Employees