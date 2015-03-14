/*
 * DBD-GenSql auto generated sql script (create table)
 * Mode = oracle9
 */

/* Auszugspositionen*/
/* implements structure Positionen*/
create table Positionen (
 Id varchar2(32) NOT NULL,
 EffDt date NOT NULL,
 Amt numeric(13,2) NOT NULL,
 Memo varchar2(512) NOT NULL,
 CurCode varchar2(3) NOT NULL,
 PostedDt date NOT NULL,
 KtoId numeric(9,0) NOT NULL,
 primary key (Id))
;

/* Kontentabelle*/
/* implements structure Konten*/
create table Konten (
 Id numeric(9,0) NOT NULL,
 Blz varchar2(10) NOT NULL,
 KtoNr varchar2(35) NOT NULL,
 CurCode varchar2(3) NOT NULL,
 KtoName varchar2(35),
 KtoDescr varchar2(512),
 primary key (Id))
;

/*
 * constraints and indices
 */
alter table Positionen add constraint FK_Positionen_Konten_1 foreign key (KtoId) references Konten (Id);

/* eof */