CREATE TABLE project_cinema.Movies (
	movie_id integer constraint movies_movie_id_pk PRIMARY KEY,
	title varchar constraint movies_title_nn NOT NULL,
	description varchar,
	release_year date constraint movies_release_year NOT NULL,
	rating integer default 0,
	duration interval default '0'
	constraint movies_rating_cc check((rating >= 0) and(rating <= 10)) 
)