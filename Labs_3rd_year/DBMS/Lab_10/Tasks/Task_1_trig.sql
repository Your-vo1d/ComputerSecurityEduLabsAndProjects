CREATE OR REPLACE FUNCTION project_cinema.set_user_id()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.user_id IS NULL THEN
        NEW.user_id := NEXTVAL('project_cinema.users_user_id_seq');
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_set_user_id
BEFORE INSERT ON project_cinema.users
FOR EACH ROW
EXECUTE FUNCTION project_cinema.set_user_id();
