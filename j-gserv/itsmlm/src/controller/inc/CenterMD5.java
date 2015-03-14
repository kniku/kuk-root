package controller.inc;

import java.sql.Connection;
import java.sql.SQLException;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.zkoss.zk.ui.Sessions;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zk.ui.util.GenericForwardComposer;
import org.zkoss.zul.Label;
import org.zkoss.zul.ListModelList;
import org.zkoss.zul.Listbox;
import org.zkoss.zul.Progressmeter;
import org.zkoss.zul.Timer;

import shared.AppConstants;
import shared.AppContext;
import bo.Translation;
import bo.wartung.Checker_MD5;
import controller.ListModelTranslations;

public class CenterMD5 extends GenericForwardComposer {
	private static final long serialVersionUID = 1L;
	private static final Log log = LogFactory.getLog(CenterMD5.class);
	private Timer timer_progress;				// auto wired
	private Progressmeter progress_check_trans;	// auto wired
	private Label lab_progress_check_trans;		// auto wired
	private Progressmeter progress_check_text;	// auto wired
	private Label lab_progress_check_text;		// auto wired
	private Listbox lb_translations;			// auto wired

	private Checker_MD5 checker_trans = null;
	private ListModelTranslations ModelListOfTranslations;

	public void onCreate(Event event) {
		ModelListOfTranslations = new ListModelTranslations();
		ModelListOfTranslations.initListbox(lb_translations);
	}
	
	public void onClick$but_ok(Event event) throws SQLException {
		log.debug("MD5 Check started");
		AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
		DataSource ds = ac.getDataSource();
		progress_check_trans.setValue(0);
//		Panel p = (Panel )event.getPage().getFellowIfAny("win_CenterMD5").getFellowIfAny("panel_list");
//		p.setVisible(true);
		Connection conn;
		try {
			conn = ds.getConnection();
			checker_trans = new Checker_MD5(conn);
			checker_trans.start();
			timer_progress.start();
			lab_progress_check_trans.setValue("ermittle Anzahl...");
			lab_progress_check_text.setValue("not impl. yet");
		} catch (SQLException e) {
			log.error(e);
			throw e;
		}
		((ListModelList)lb_translations.getModel()).clear();
		event.getPage().invalidate();
//		log.debug("MD5 Check beendet");
	}
	public void onTimer$timer_progress(Event event) {
		if (checker_trans != null) {
			double max = checker_trans.getMaxTranslations();
			double cnt = checker_trans.getNumOfScannedTranslations();
			if (max > 0 && cnt >= 0) {
				if (max > cnt) {
					progress_check_trans.setValue((int)((cnt / max) * 100));
					lab_progress_check_trans.setValue("scanning "+cnt+"/"+max+" ..."+(int)((cnt / max) * 100));
				}
				else {
					progress_check_trans.setValue(100);
					lab_progress_check_trans.setValue("Fertig, Probleme: " + checker_trans.getNumOfProblems());
					timer_progress.stop();
					ListModelList x = (ListModelList)lb_translations.getModel();
					for (Translation T : checker_trans.getListOfProblems()) {
						x.add(T);			
					}
//					lb_translations.invalidate();
				}
			}
		} else {
			log.error("checker NOT found/initialized!");
			timer_progress.stop();
		}
//		event.getPage().invalidate();
	}	
}
