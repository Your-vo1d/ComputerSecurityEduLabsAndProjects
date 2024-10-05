WITH RECURSIVE EmployeeHierarchy AS (
    -- Базовый уровень: выбираем всех сотрудников и их непосредственных менеджеров
    SELECT
        e.employee_id,
        e.manager_id,
        e.last_name,
        e.first_name
    FROM
        employees e
    WHERE
        e.manager_id IS NOT NULL  -- сотрудники, у которых есть менеджеры

    UNION ALL

    -- Рекурсивный шаг: добавляем подчиненных всех уровней
    SELECT
        e.employee_id,
        eh.manager_id,
        e.last_name,
        e.first_name
    FROM
        employees e
    JOIN EmployeeHierarchy eh ON e.manager_id = eh.employee_id
)
-- Основной запрос с подключением таблицы dependents
SELECT
    m.employee_id AS manager_id,
    m.last_name || ' ' || SUBSTRING(m.first_name, 1, 1) || '.' AS manager_name
FROM
    EmployeeHierarchy eh
JOIN employees m ON eh.manager_id = m.employee_id  -- соединение менеджера и подчиненных
JOIN (
    SELECT
        employee_id,
        COUNT(dependent_id) AS num_of_dependents
    FROM
        dependents
    GROUP BY
        employee_id
    HAVING
        COUNT(dependent_id) >= 2  -- сотрудники с 2 и более детьми
) d ON eh.employee_id = d.employee_id  -- соединение с сотрудниками, имеющими 2 и более детей
GROUP BY
    m.employee_id, m.last_name, m.first_name
ORDER BY
    m.manager_id;
