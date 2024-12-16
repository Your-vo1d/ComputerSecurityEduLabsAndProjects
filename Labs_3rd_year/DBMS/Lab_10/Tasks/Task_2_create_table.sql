CREATE TABLE project_cinema.log_table (
    log_id SERIAL PRIMARY KEY,
    table_name VARCHAR(50),
    operation_type VARCHAR(10),
    changed_by TEXT DEFAULT CURRENT_USER,
    change_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    old_data JSONB,
    new_data JSONB
);
