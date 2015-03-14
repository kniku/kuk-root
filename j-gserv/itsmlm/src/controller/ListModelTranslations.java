package controller;

import org.zkoss.zul.ListModel;
import org.zkoss.zul.ListModelList;
import org.zkoss.zul.Listbox;
import org.zkoss.zul.Listcell;
import org.zkoss.zul.Listhead;
import org.zkoss.zul.Listheader;
import org.zkoss.zul.Listitem;
import org.zkoss.zul.ListitemRenderer;

import bo.Translation;

public class ListModelTranslations extends ListModelList {
	private static final long serialVersionUID = 1L;

	public static boolean isSelectable(Translation iTrans) {
		boolean r = true;
		if (iTrans != null) {
			String l = iTrans.getTr_look();
			if (l != null && !l.isEmpty() && !l.equals("*")) {
				r = false;
			}
		}
		return r;
	}
	
	class RendererListOfTranslations implements ListitemRenderer{
		@Override
		public void render(Listitem item, Object data) throws Exception{
			Translation T = (Translation )data;
			if (!isSelectable(T)) item.setStyle("color:#999999;");
			new Listcell(T.getText()).setParent(item);
            new Listcell(T.getTranslation()).setParent(item);
            new Listcell(T.getVersion()).setParent(item);
            new Listcell(T.getTr_look()).setParent(item);
		}
	}

//	public ListModelTranslations(Listbox iList) {
//		super();
	public void initListbox(Listbox iList) {
		ListModel ModelListOfTranslations = new ListModelList();
		Listhead hh = new Listhead();
		hh.setSizable(true);
		Listheader h = new Listheader("Originaltext");
		h.setSort("auto");
		hh.appendChild(h);
		hh.appendChild(new Listheader("Übersetzung"));
		h = new Listheader("Version");
		h.setWidth("65px");
		hh.appendChild(h);
		h = new Listheader("L&F");
		h.setWidth("35px");
		hh.appendChild(h);
		iList.appendChild(hh);
		iList.setModel(ModelListOfTranslations);
		iList.setItemRenderer(new RendererListOfTranslations());
		
	}

	
}
