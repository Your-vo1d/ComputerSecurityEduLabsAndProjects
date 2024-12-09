CREATE VIEW user_movie_reviews AS
SELECT 
    u.user_id, 
    u.nickname AS user_nickname,                -- Ник пользователя
    m.title AS movie_title,                     -- Название фильма
    g.genre_name AS genre,                      -- Жанр фильма
    r.rating AS user_rating,                    -- Оценка пользователя
    r.review_date                               -- Дата написания отзыва
FROM users u
JOIN reviews r ON u.user_id = r.user_id         -- Соединение пользователей с их отзывами
JOIN movies m ON r.movie_id = m.movie_id        -- Соединение отзывов с фильмами
JOIN movies_genres mg ON m.movie_id = mg.movie_id -- Соединение фильмов с их жанрами
JOIN genres g ON mg.genre_id = g.genre_id       -- Соединение для получения названия жанра
ORDER BY u.user_id, r.review_date DESC;         -- Сортировка по пользователю и дате отзыва


CREATE VIEW user_subscription_watchlist AS
SELECT 
    u.user_id, 
    u.user_name AS user_nickname,                 
    s.description AS subscription_type,          
    m.title AS movie_title,                      
    w.added_date AS date_added_to_watchlist      
FROM project_cinema.users u
LEFT JOIN project_cinema.subscriptions s 
    ON u.subscription_id = s.subscription_id      
LEFT JOIN project_cinema.watchlists w 
    ON u.user_id = w.user_id                     
LEFT JOIN project_cinema.movies m 
    ON w.movie_id = m.movie_id                   
ORDER BY u.user_id, w.added_date DESC;           
