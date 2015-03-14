
/* MYSQL */

create database bankinfo;
create user bankinfo identified by "bankinfo";
grant all on bankinfo.* to bankinfo;

insert into konten (id,blz,ktonr,curcode) values (1,'12000','08835436001','EUR');

/*
create table Positionen (
	Id int unsigned not null auto_increment unique comment 'Position Id',
	KtoId int unsigned not null comment 'Konto Id',
	CurCode varchar(3) not null comment 'Positionswährung',
	Memo varchar(512) not null comment 'Auzugstext',
	Amt double not null comment 'Positionsbetrag',
	EffDt datetime not null comment 'Valutadatum',
	PostedDt datetime not null comment 'Buchungsdatum',
	key (Id),
	constraint fk_Konten foreign key (KtoId) references Konten (Id));
*/