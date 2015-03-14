
CREATE TABLE konten
(
   Id int PRIMARY KEY NOT NULL,
   Blz varchar(10) NOT NULL,
   KtoNr varchar(35) NOT NULL,
   CurCode varchar(3) NOT NULL,
   KtoName varchar(35),
   KtoDescr text
);

CREATE TABLE positionen
(
   Id varchar(32) PRIMARY KEY NOT NULL,
   EffDt timestamp NOT NULL,
   Amt numeric NOT NULL,
   Memo text NOT NULL,
   CurCode varchar(3) NOT NULL,
   PostedDt timestamp NOT NULL,
   KtoId int NOT NULL
);
ALTER TABLE positionen ADD CONSTRAINT fk_positionen_konten FOREIGN KEY (KtoId) REFERENCES konten(Id);
CREATE INDEX idx_fk_positionen_konten ON positionen(KtoId);

CREATE OR REPLACE FUNCTION positionen_create_md5id() RETURNS trigger AS '
--DECLARE
--changed boolean;
BEGIN
IF tg_op = ''INSERT'' THEN
	new.id=md5(new.memo||to_char(new.amt,''9999999.99'')||to_char(new.effdt,''YYYYMMDD'')||to_char(new.ktoid,''9999''));
END IF;
RETURN new;
END
' LANGUAGE plpgsql;

CREATE TRIGGER trg_create_md5 before INSERT ON positionen FOR EACH ROW EXECUTE PROCEDURE positionen_create_md5id();

/* eof */