
insert into mltrans_history (mcode,Look,Lang,App,Modul,Dialog,TextKey,Translation,isActive,TextMD5,UpdKz,UpdIP,UpdTs)
select mcode,Look,Lang,App,Modul,Dialog,TextKey,Translation,isActive,TextMD5,'N','INIT',sysdate from mltrans;
