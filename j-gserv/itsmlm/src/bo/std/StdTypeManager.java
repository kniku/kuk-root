package bo.std;

import java.util.HashMap;

import bo.std.types.AbstractStdType;

public final class StdTypeManager {
		HashMap<String, AbstractStdType> mMap = new HashMap<String, AbstractStdType>();
		void addType(AbstractStdType iType) {
			if (iType != null) mMap.put(iType.getName(), iType);
		}
		AbstractStdType getTypeByName(String iName) {
			return mMap.get(iName);
		}
	}
