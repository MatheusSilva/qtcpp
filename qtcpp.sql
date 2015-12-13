CREATE TABLE usuarios (
	id bigint PRIMARY KEY AUTO_INCREMENT,
	nome varchar(100),
	email varchar(100),
	sexo char(1),
	idade tinyint(3),
	cidade varchar(100)
);