CREATE INDEX idx_movies_rating ON project_cinema.movies(rating)
CREATE INDEX idx_movies_release_year ON project_cinema.movies(relsease_year)
CREATE INDEX idx_watchlist_user_movie ON project_cinema.watchlists(user_id, movie_id)