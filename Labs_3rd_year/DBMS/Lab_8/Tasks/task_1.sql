WITH RECURSIVE ManagerHierarchy AS (
    -- Выбираем сотрудника и его непосредственного менеджера (1 уровень)
    SELECT 
        e.employee_id,
        e.manager_id,
        e.last_name || ' ' || SUBSTRING(e.first_name, 1, 1) || '. - '  || d.department_name AS employee_name,
        m.last_name || ' ' || SUBSTRING(m.first_name, 1, 1) || '.' AS manager_name,
        1 AS level
    FROM 
        employees e
    JOIN 
        employees m ON e.manager_id = m.employee_id
	JOIN departments d ON d.department_id = e.department_id

    UNION ALL

    -- Рекурсивно поднимаемся на второй уровень, чтобы найти менеджера менеджера
    SELECT 
        mh.employee_id,
        m2.manager_id,
        mh.employee_name,
        mh.manager_name || ', ' || M3.LAST_NAME || ' ' ||  SUBSTRING(m3.first_name, 1, 1) || '.' AS manager_name,
        mh.level + 1 AS level
    FROM 
        ManagerHierarchy mh
	JOIN EMPLOYEES M2 ON MH.MANAGER_ID = M2.EMPLOYEE_ID
	JOIN EMPLOYEES M3 ON M2.MANAGER_ID = M3.EMPLOYEE_ID
    WHERE 
        mh.level = 1 -- Берем только тех, у кого есть второй менеджер
)

SELECT 
    employee_name,
    manager_name
FROM 
    ManagerHierarchy
WHERE 
    level = 2 -- Выводим только сотрудников, у которых два менеджера