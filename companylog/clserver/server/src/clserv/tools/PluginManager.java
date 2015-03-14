package clserv.tools;

import java.io.File;
import java.io.FileInputStream;
import java.util.HashMap;

import org.apache.log4j.Logger;

import clserv.shared.I_Plugin;


/**
 * @author kuk
 * load and manage plugins
 * SINGLETON
 */
final public class PluginManager {
	private final Logger logger = Logger.getLogger(this.getClass());
	private static PluginManager m_Instance = null;
	private String m_PluginDir = ".";
	private String m_PluginPackage = "plugins";
	private MyFileClassLoader m_ClassLoader;
	private HashMap<String, PluginEntry> m_Plugins = new HashMap<String, PluginEntry>();

	private class PluginEntry {
		Class<?> clazz = null;
		I_Plugin clazzinstance = null;
		long last_modified = 0;
	}
	
	private class MyFileClassLoader extends ClassLoader {

		private String rootPath;

		MyFileClassLoader(String rootPath) {
			this.rootPath = rootPath;
		}

		@SuppressWarnings("unchecked")
		protected Class findClass(String name) throws ClassNotFoundException {
			byte[] b = loadClassData(name);
			if (b == null)
				throw new ClassNotFoundException(name);
			return defineClass(name, b, 0, b.length);
		}

		/**
		 * Liefert die gefundene Klasse als Byte Array zur�ck ... 
		 * @param name
		 * @return byte[] theClassBytes
		 */
		private byte[] loadClassData(String name) {
			try {
				name = rootPath + name.replace('.', File.separatorChar) + ".class";
				File f = new File(name);
				FileInputStream fis = new FileInputStream(f);
				byte cByte[] = new byte[(int) f.length()];
				fis.read(cByte);
				fis.close();
				return cByte;
			} catch (Exception e) {
				e.printStackTrace();
			}
			return null;
		}
	}
	
	/**
	 * @return the plugin manager (SINGLETON)
	 */
	public static PluginManager getInstance() {
		if (m_Instance == null) {
			m_Instance = new PluginManager();
		}
		return m_Instance;
	}
	private PluginManager() {
		m_ClassLoader = new MyFileClassLoader(m_PluginDir + "/");
	}

	public Object[] getListOfPlugins() {
		return m_Plugins.keySet().toArray();
	}
	
	/**
	 * @param iClassName class of plugin
	 * @return the instance of plugin class or null if not found
	 */
	public I_Plugin getInstanceOfPlugin(String iClassName) {
		I_Plugin r = null;
		PluginEntry pe = m_Plugins.get(iClassName);
		if (pe != null) r = pe.clazzinstance;
		
/* start - for plugin development ... */

		if (r ==  null) {
			Class<?> clazz;
			try {
				clazz = Class.forName("plugins."+iClassName);
				if (clazz != null) {
					Object clazzinstance = clazz.newInstance();
					if (clazzinstance instanceof I_Plugin) {
						pe = new PluginEntry();
						m_Plugins.put(iClassName, pe);
						pe.clazz = clazz;
						pe.clazzinstance = (I_Plugin)clazzinstance;
						pe.last_modified = 0;
						r = pe.clazzinstance;
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
/* end - for plugin development ... */
		
		return r;
	}
	
	public void reloadPlugins() {
		logger.info("Neuladen der Plugins, Verzeichnis: "+m_PluginDir+"/"+m_PluginPackage);
		File d = new File(m_PluginDir+"/"+m_PluginPackage);
		String[] flist = d.list();
		if (flist != null) {
			I_Plugin plugininstance = null;
			for (int i = 0; i<flist.length; i++) {
				logger.info("Datei: "+flist[i]);
				if (flist[i].endsWith(".class")) {
					String clazzname = flist[i].substring(0, flist[i].length() - 6);
					String clazz_internalname;
					int idx_postfix = clazzname.lastIndexOf("_");
					if (idx_postfix > 2) {
						clazz_internalname = clazzname.substring(0, idx_postfix);
					} else {
						clazz_internalname = clazzname;
					}
					logger.info("Klasse: "+clazzname+", internal: "+clazz_internalname);
					File dd = new File(m_PluginDir+"/"+m_PluginPackage+"/"+flist[i]);
					long last_modified = dd.lastModified();
					PluginEntry aPluginEntry;
					aPluginEntry = m_Plugins.get(clazz_internalname);
					if (aPluginEntry != null) {
						logger.info("Klasse bereits registriert: "+aPluginEntry.clazz.getSimpleName());
						if (aPluginEntry.last_modified >= last_modified) continue;
						logger.info("Datei ist aktueller, ueberlade Klasse...");
					} else {
						logger.info("Klasse noch nicht registriert.");
						aPluginEntry = null;
					}
					boolean plugin_failed = true;
					try {
						Class<?> clazz = m_ClassLoader.loadClass(m_PluginPackage + "." + clazzname);
						Object clazzinstance = clazz.newInstance();
						if (clazzinstance instanceof I_Plugin) {
							plugininstance = (I_Plugin)clazzinstance;
							plugininstance.onNewInstance();
							plugin_failed = false;
							if (aPluginEntry == null) {
								aPluginEntry = new PluginEntry();
								m_Plugins.put(clazz_internalname, aPluginEntry);
							}
							aPluginEntry.clazz = clazz;
							aPluginEntry.clazzinstance = plugininstance;
							aPluginEntry.last_modified = last_modified;
							logger.info("Klasse registriert: "+clazz_internalname+"->"+clazz.getSimpleName());
						} else {
							logger.warn("Klasse implementiert nicht das Plugin Interface ("+I_Plugin.class.getSimpleName()+")!");							
						}
					} catch (Throwable e) {
						e.printStackTrace();
					}
					if (plugin_failed) {
						logger.error("Ungueltiges Plugin: "+flist[i]);
					}
				}
			}
		}
		logger.info("Neuladen der Plugins fertig.");
	}	
	
}
