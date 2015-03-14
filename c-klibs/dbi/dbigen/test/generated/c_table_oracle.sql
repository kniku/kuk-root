/*
 * DBD-GenSql auto generated sql script (create table)
 * Mode = oracle9
 */

/* Testtabelle 2*/
/* implements structure S2*/
create table T2 (
 S2C1 varchar2(10) default 'Hi' NOT NULL,
 S2C2 date default to_date('2003.12.13 00:00:00','YYYY.MM.DD HH24:MI:SS'),
 S2C3 date default to_date('2001.05.12 14:15:16','YYYY.MM.DD HH24:MI:SS') NOT NULL,
 primary key (S2C1))
 ENGINE=InnoDB
 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
 COMMENT='Testtabelle 2'
;

/* implements structure S1*/
create table TX (
 S1C0 numeric(9,0) NOT NULL,
 S1C1 varchar2(10) default 'Hi' NOT NULL,
 S1C2 numeric(10,2) default -9.12 NOT NULL,
 S1C3 date default to_date('0001.02.01 00:00:00','YYYY.MM.DD HH24:MI:SS') NOT NULL,
 S1C4 varchar2(35) default '                                   ' NOT NULL,
 S1C5 varchar2(35) default 'test overloading' NOT NULL,
 S1C6 varchar2(35) default 'xxx' NOT NULL,
 primary key (S1C2))
 ENGINE=InnoDB
 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
;
/* simulating auto increment columns for table */
create sequence seq_TX_S1C0 start with 1 increment by 1 maxvalue 999999999;
create trigger trg_TX_S1C0 before insert on TX for each row begin select seq_TX_S1C0.nextval into :new.S1C0 from dual; end;
/


/* Testtabelle 1*/
/* implements structure S1*/
create table T1 (
 S1C0 numeric(9,0) NOT NULL,
 S1C1 varchar2(10) default 'Hi' NOT NULL,
 S1C2 numeric(10,2) default -9.12 NOT NULL,
 S1C3 date default to_date('0001.02.01 00:00:00','YYYY.MM.DD HH24:MI:SS') NOT NULL,
 S1C4 varchar2(35) default '                                   ' NOT NULL,
 S1C5 varchar2(35) default 'test overloading' NOT NULL,
 S1C6 varchar2(35) default 'xxx' NOT NULL,
 primary key (S1C1,S1C2))
 ENGINE=InnoDB
 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci
 COMMENT='Testtabelle 1'
;
/* simulating auto increment columns for table */
create sequence seq_T1_S1C0 start with 1 increment by 1 maxvalue 999999999;
create trigger trg_T1_S1C0 before insert on T1 for each row begin select seq_T1_S1C0.nextval into :new.S1C0 from dual; end;
/


/*
 * constraints and indices
 */
alter table T2 add constraint fk01_T2_T1 foreign key (S2C2) references T1 (S1C3);
create index IDX_TX_1 on TX (S1C2,S1C3);
create unique index idxT1 on T1 (S1C2,S1C3);

/* eof */