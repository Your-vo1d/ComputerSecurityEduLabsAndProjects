CREATE TABLE project_cinema.Users(
	user_id integer constraint user_id_pk PRIMARY KEY,
	username varchar constraint username_nn NOT NULL,
	email varchar,
	date_of_birth date,
	registartion_date timestamp constraint users_registration_date_nn NOT NULL,
	subscription_id integer
)
