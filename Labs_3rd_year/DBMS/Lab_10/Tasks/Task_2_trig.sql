CREATE OR REPLACE FUNCTION project_cinema.log_user_changes()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO project_cinema.log_table (table_name, operation_type, old_data, new_data)
    VALUES (
        'users',                          
        TG_OP,                            
        TO_JSONB(OLD),                    
        TO_JSONB(NEW)                     
    );
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_log_user_changes
AFTER INSERT OR UPDATE OR DELETE ON project_cinema.users
FOR EACH ROW
EXECUTE FUNCTION project_cinema.log_user_changes();
