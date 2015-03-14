package plugins.RESMGR;

import java.io.File;
import java.util.HashMap;
public class ResourceManager {
	// SINGLETON
	private final String DATA_DIR = "./data/plugins/RESMGR";
	private static ResourceManager m_Instance = null;
	private static HashMap<String,Resource> m_Resources = new HashMap<String,Resource>();
	
	private ResourceManager() {
		reloadResources();
	}
	
	public void reloadResources() {
		m_Resources.clear();
		// parsing file list
		File d = new File(DATA_DIR);
		if (d.exists() == false) d.mkdirs();
		String[] flist = d.list();
		if (flist != null) {
			for (int i = 0; i < flist.length; i++) {
				if (flist[i].endsWith(".res")) {
					Resource aRes = Resource.loadResource(DATA_DIR + "/" + flist[i]);
					if (aRes != null) {
						m_Resources.put(aRes.getName(), aRes);
					}
				}
			}
		}
		// loading resources
	}
	
	public static ResourceManager getInstance() {
		if (m_Instance == null) m_Instance = new ResourceManager();
		return m_Instance;
	}

	public Resource getResource(String iName) {
		return m_Resources.get(iName);
	}
	
	public Object[] getListOfResources() {
		return m_Resources.keySet().toArray();
	}
}
