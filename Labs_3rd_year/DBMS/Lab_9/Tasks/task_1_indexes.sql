-- Уникальный индекс в таблице users
CREATE UNIQUE INDEX idx_unique_nickname ON project_cinema.users(user_name)
-- Уникальный индекс в таблице movies
CREATE UNIQUE INDEX idx_unique_title ON project_cinema.movies(title)