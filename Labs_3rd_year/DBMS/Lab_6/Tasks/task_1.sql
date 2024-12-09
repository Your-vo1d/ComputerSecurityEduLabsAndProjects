

CREATE TABLE Users(
	user_id integer constraint user_id_pk PRIMARY KEY,
	username varchar constraint username_nn NOT NULL,
	email varchar,
	date_of_birth date,
	registartion_date timestamp constraint users_registration_date_nn NOT NULL,
	type_subscription varchar,
	date_start_sub date,
	date_end_sub date,
	is_sub bool constraint users_is_sub_nn NOT NULL
)
CREATE TABLE Reviews(
	review_id integer constraint reviews_review_id_pk PRIMARY KEY,
	user_id integer,
	movie_id integer,
	rating integer constraint reviews_rating_cc CHECK((rating >= 0) and(rating <= 10)) default 0,
	review_text text,
	review_date timestamp constraint reviews_review_date not null,
	constraint reviews_user_id_fk foreign key (user_id) references Users(user_id),
	constraint reviews_movie_id_fk foreign key (movie_id) references Movies(movie_id)
)