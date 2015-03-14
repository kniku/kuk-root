package controller.std.list;

import org.zkoss.zul.Listcell;
import org.zkoss.zul.Listitem;
import org.zkoss.zul.ListitemRenderer;

import bo.std.types.AbstractStdObjectStructure;
import bo.std.types.AbstractStdTypeStructure;

public class StdListRenderer implements ListitemRenderer{
	@Override
	public void render(Listitem item, Object data) throws Exception {
		AbstractStdObjectStructure so = (AbstractStdObjectStructure )data;
		int last_index = ((AbstractStdTypeStructure )so.getType()).getLastMemberIndex();
		for (int i = 0; i <= last_index; i++) {
			String value = so.getValueStringByIndex(i);
			if (value == null) value = "";
            new Listcell(value).setParent(item);
		}

	}
}
