/*
 * DBD-GenSql auto generated sql script (create table)
 * Mode = mysql5
 */

/* Testtabelle 2*/
/* implements structure S2*/
create table T2 (
 S2C1 varchar(10) default 'Hi' NOT NULL comment 'Spalte 1',
 S2C2 datetime default '2003-12-13 00:00:00' comment 'Spalte 2',
 S2C3 datetime default '2001-05-12 14:15:16' NOT NULL comment 'Spalte 3',
 primary key (S2C1))
 ENGINE=InnoDB
 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
 COMMENT='Testtabelle 2'
;

/* implements structure S1*/
create table TX (
 S1C0 int unsigned auto_increment NOT NULL comment 'automatic id test',
 S1C1 varchar(10) default 'Hi' NOT NULL comment 'Spalte 1',
 S1C2 double default -9.12 NOT NULL comment 'Spalte 2',
 S1C3 datetime default '0001-02-01 00:00:00' NOT NULL comment 'Spalte 3',
 S1C4 varchar(35) default '                                   ' NOT NULL comment 'Spalte 4',
 S1C5 varchar(35) default 'test overloading' NOT NULL comment 'Spalte 5',
 S1C6 varchar(35) default 'xxx' NOT NULL comment 'Spalte 6',
 primary key (S1C2))
 ENGINE=InnoDB
 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
;

/* Testtabelle 1*/
/* implements structure S1*/
create table T1 (
 S1C0 int unsigned auto_increment NOT NULL comment 'automatic id test',
 S1C1 varchar(10) default 'Hi' NOT NULL comment 'Spalte 1',
 S1C2 double default -9.12 NOT NULL comment 'Spalte 2',
 S1C3 datetime default '0001-02-01 00:00:00' NOT NULL comment 'Spalte 3',
 S1C4 varchar(35) default '                                   ' NOT NULL comment 'Spalte 4',
 S1C5 varchar(35) default 'test overloading' NOT NULL comment 'Spalte 5',
 S1C6 varchar(35) default 'xxx' NOT NULL comment 'Spalte 6',
 primary key (S1C1,S1C2))
 ENGINE=InnoDB
 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
 COMMENT='Testtabelle 1'
;

/*
 * constraints and indices
 */
alter table T2 add constraint fk01_T2_T1 foreign key (S2C2) references T1 (S1C3);
alter table TX add index IDX_TX_1 (S1C2,S1C3);
alter table T1 add constraint unique index idxT1 (S1C2,S1C3);

/* eof */