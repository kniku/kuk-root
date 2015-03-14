package controller.std.list;

import org.zkoss.zul.ListModelList;
import org.zkoss.zul.Listbox;
import org.zkoss.zul.Listhead;
import org.zkoss.zul.Listheader;

import bo.std.types.AbstractStdTypeStructure;

final public class StdListboxUtils {
	public static void initializeStdListbox(Listbox iListbox, AbstractStdTypeStructure iType) {
		ListModelList Model = new ListModelList();
		Listhead hh = new Listhead();
		hh.setSizable(true);

		int last_index = iType.getLastMemberIndex();
		for (int i = 0; i <= last_index; i++) {
			Listheader h = new Listheader(iType.getMemberLabelByIndex(i));
//				h.setSort("auto");
			hh.appendChild(h);				
		}
		
		iListbox.appendChild(hh);
		iListbox.setModel(Model);
		iListbox.setItemRenderer(new StdListRenderer());
		
	}
}
