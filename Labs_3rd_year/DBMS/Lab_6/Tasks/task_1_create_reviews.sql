CREATE TABLE project_cinema.Reviews(
	review_id integer constraint reviews_review_id_pk PRIMARY KEY,
	user_id integer,
	movie_id integer,
	rating integer constraint reviews_rating_cc CHECK((rating >= 0) and(rating <= 10)) default 0,
	review_text varchar,
	review_date timestamp constraint reviews_review_date not null,
	constraint reviews_user_id_fk foreign key (user_id) references project_cinema.Users(user_id),
	constraint reviews_movie_id_fk foreign key (movie_id) references project_cinema.Movies(movie_id)
)