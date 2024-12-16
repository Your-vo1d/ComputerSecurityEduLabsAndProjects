CREATE OR REPLACE FUNCTION project_cinema.update_movie_rating()
RETURNS TRIGGER AS $$
BEGIN
    UPDATE project_cinema.movies
    SET rating = (
        SELECT COALESCE(AVG(rating), 0)  
        FROM project_cinema.reviews
        WHERE movie_id = NEW.movie_id
    )
    WHERE movie_id = NEW.movie_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_update_movie_rating
AFTER INSERT OR UPDATE ON project_cinema.reviews
FOR EACH ROW
EXECUTE FUNCTION project_cinema.update_movie_rating();
