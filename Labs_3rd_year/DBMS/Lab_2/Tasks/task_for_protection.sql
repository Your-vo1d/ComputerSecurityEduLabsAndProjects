SELECT
    department_id,
    SUBSTRING(
        first_name
        FROM
            2
    ) AS name_without_first_char,
    last_name || ' ' || ABS(LENGTH (first_name) - LENGTH (last_name)) || ' # ' || UPPER(email) AS last_name_with_len_f_name
FROM
    employees
WHERE
    email like '%b%'