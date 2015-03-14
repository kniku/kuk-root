
/* trigger fuer den slave mode */

create or replace TRIGGER TRG_ES_LOGS_AI AFTER INSERT ON ES_LOGS
FOR EACH ROW
BEGIN
	dbms_alert.signal('ECOLOGALERT', '<'||
		case :new.entry_type
			when 'I' then 'in'
			else 'out'
		end||
		' l="'||:new.login||
		'" i="'||nvl(:new.info,'')||
		'" t="'||to_char(:new.entry_ts,'yyyymmddHH24MISS')||
		'"/>'
		);
END;
/

