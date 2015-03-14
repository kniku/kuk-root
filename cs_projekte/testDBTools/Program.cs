using System;
using System.Collections;
using knk.DBTools;
using System.Data;
using testDBTools.Properties;
//using System.Collections.Specialized;
//using System.Configuration;

//using Npgsql;

namespace testDBTools
{

	class Program
	{

/*		[SQLTABLE(name="konten")]
		class CKonten : ATableStruct
		{
			[SQLCOLUMN(name="blz")] public string BLZ { get; set; }
			public string str2 { get; set; }
			[SQLCOLUMN(name="id",auto=true)] public int ID { get; set; }
			[SQLCOLUMN(name = "ktonr")]
			public string KTONR { get; set; }
			[SQLCOLUMN(name = "curcode")]
			public string CURRENCY { get; set; }
		}
*/
		[SQLTABLE(name = "knk_test")]
		class testStruct : ATableStruct
		{
			[SQLCOLUMN(name = "id")]
			public Int32 id { get; set; }
			[SQLCOLUMN(name = "txt1")]
			public string str1 { get; set; }
			[SQLCOLUMN(name = "txt2")]
			public string str2 { get; set; }
			[SQLCOLUMN]
			public DateTime DT1 { get; set; }
			[SQLCOLUMN]
			public Double NUM1 { get; set; }
			[SQLCOLUMN(name = "terminal")]
			public string TERMINAL { get; set; }	// Terminal/Arbeitsplatz; len.:256

		}

		static void create_schema(Connection conn)
		{
			string rv_str;
			
			try
			{
				rv_str = conn.execSQL("drop table knk_test");
				System.Console.WriteLine("drop table: rv={0}", rv_str);
			}
			catch (Exception)
			{
				// just catch it...
			}

			if (conn.Provider == ConnectionPool.ProviderType.SqlServer)
			{
				rv_str = conn.execSQL("create table knk_test (ID integer, TXT1 varchar(256), TXT2 varchar(256), DT1 datetime, NUM1 numeric(15,2), TERMINAL nvarchar(256))");
			}
			else
			{
				rv_str = conn.execSQL("create table knk_test (ID integer, TXT1 varchar(256), TXT2 varchar(256), DT1 timestamp, NUM1 numeric(15,2), TERMINAL varchar(256))");
			}
			System.Console.WriteLine("create table: rv={0}", rv_str);

		}
		
		static void create_testdata(Connection conn, int nr)
		{
			string rv_str = conn.execSQL("insert into knk_test (ID, TXT1, TXT2, DT1, NUM1) values (@1,@2,@3,@4,@5)",
				nr * 10 + 1, nr.ToString() + "1-1", nr.ToString() + "1-2", DateTime.Now, nr * 10 + 1.1);
			TESTFRAME.TEST("insert " + nr, 1, rv_str);
			rv_str = conn.execSQL("insert into knk_test (ID, TXT1, TXT2, DT1, NUM1) values (@1,@2,@3,@4,@5)",
				nr * 10 + 2, nr.ToString() + "2-1", nr.ToString() + "2-2", DateTime.Now, nr * 10 + 2.1);
			TESTFRAME.TEST("insert " + nr, 1, rv_str);
			rv_str = conn.execSQL("insert into knk_test (ID, TXT1, TXT2, DT1, NUM1) values (@1,@2,@3,@4,@5)",
				nr * 10 + 3, nr.ToString() + "3-1", nr.ToString() + "3-2", DateTime.Now, nr * 10 + 3.1);
			TESTFRAME.TEST("insert " + nr, 1, rv_str);
			if (nr > 1)
			{
				rv_str = conn.execSQL("insert into knk_test (ID, TXT1, TXT2, DT1, NUM1) values (@1,@2,@3,@4,@5)",
					nr * 10 + 4, nr.ToString() + "4-1", nr.ToString() + "4-2", DateTime.Now, nr * 10 + 4.1);
				TESTFRAME.TEST("insert " + nr, 1, rv_str);
			}
		}

		static void Main(string[] args)
		{
			string rv_str;

			Program app = new Program();

			TESTFRAME.TESTSUITE_START();

			try
			{
				ConnectionPool pool = null;
				if (pool == null)
				{
					//pool = new ConnectionPool(ConnectionPool.ProviderType.Postgres, Settings.Default.databasePG_BTA);
					pool = new ConnectionPool(ConnectionPool.ProviderType.Postgres, Settings.Default.databaseERDE);
					//pool = new ConnectionPool(ConnectionPool.ProviderType.SqlServer, Settings.Default.databaseAxavia);
				}
				else
				{
					/*postgres WSTECKNK*/
					//pool = new ConnectionPool(ConnectionPool.ProviderType.Postgres, "localhost", "kuk_test", "kuk", "kuk", 7000);

					/*postgres ERDE*/
					pool = new ConnectionPool(ConnectionPool.ProviderType.Postgres, "localhost", "testdb", "kuk", "kuk", 0);

					/*sqlserver BTA*/
					//pool = new ConnectionPool(ConnectionPool.ProviderType.SqlServer, "axavia", "BT_ZETS", "axavia", "Axavia#2011", 0);

					/*sqlite*/
					//pool = new ConnectionPool(ConnectionPool.ProviderType.SQLite, null, "kuk_test.sqlite", null, null, 0);
				}

				Connection conn1 = pool.getConnection();
				Connection conn2 = pool.getConnection();

				LookupRegistry.registerTable(typeof(testStruct));

				foreach (TableLookupInfo tli in LookupRegistry.Tables)
				{
					System.Console.WriteLine("TABELLE:[" + tli.name_sql + "] (" + tli.name_struct + ")");
					foreach (ColumnLookupInfo cli in tli.Columns)
					{
						System.Console.WriteLine("  COLUMN:[" + cli.name_sql + "] (" + cli.name_struct + "," + cli.type_cs + ") auto="+cli.isAuto);
					}
				}

				System.Console.WriteLine(LookupRegistry.lookupTable("knk_test").name_sql);

				create_schema(conn1);
				if (pool.Provider == ConnectionPool.ProviderType.SqlServer)
				{
					rv_str = conn1.execSQL("ALTER DATABASE BT_ZETS SET ALLOW_SNAPSHOT_ISOLATION ON");
				}

#if true // simple sql test

				TESTFRAME.TESTGROUP_START("SIMPLE SQL/TSQL");
				conn1.TSQL_startTransaction();
				conn2.TSQL_startTransaction();
				create_testdata(conn1, 1);
				rv_str = conn1.execSQL("SELECT count(*) FROM knk_test");
				TESTFRAME.TEST("c1:select count", 3, rv_str);
				rv_str = conn2.execSQL("SELECT count(*) FROM knk_test");
				TESTFRAME.TEST("c2:select count", 0, rv_str);
				create_testdata(conn2, 2);
				rv_str = conn1.execSQL("SELECT count(*) FROM knk_test");
				TESTFRAME.TEST("c1:select count", 3, rv_str);
				rv_str = conn2.execSQL("SELECT count(*) FROM knk_test");
				TESTFRAME.TEST("c2:select count", 4, rv_str);
				conn1.TSQL_commit();
				conn2.TSQL_rollback();
				rv_str = conn1.execSQL("SELECT count(*) FROM knk_test");
				TESTFRAME.TEST("c1:select count", 3, rv_str);
				rv_str = conn2.execSQL("SELECT count(*) FROM knk_test");
				TESTFRAME.TEST("c2:select count", 3, rv_str);

				rv_str = conn1.execSQL("SELECT txt1 FROM knk_test where txt2=@2 and id=@1 and num1=@3", 11, "11-2", 11.1);
				TESTFRAME.TEST("c1:select", "11-1", rv_str);

				TESTFRAME.TESTGROUP_END();

#endif

//				rv_str = conn1.execSQL("drop table knk_test");
//				System.Console.WriteLine("drop table: rv={0}", rv_str);

#if true // test insert
				TESTFRAME.TESTGROUP_START("STD SQL");

				testStruct v = new testStruct();
				v.id = 500;
				v.str1 = "KTONR" + v.id;
				v.str2 = "BLZ" + v.id;
				v.NUM1 = 1.1;
				v.DT1 = DateTime.Now;
				v.TERMINAL = "XXXX";
				int cnt = conn1.execSTD_insert(v);
				TESTFRAME.TEST("insert STD:", 1, cnt);

				ArrayList arrKonten = new ArrayList();
				for (int i = 0; i < 100; i++)
				{
					v = new testStruct();
					v.id = i + 200;
					v.str1 = "KTONR" + v.id;
					v.str2 = "BLZ" + v.id;
					v.NUM1 = 1.1;
					v.DT1 = DateTime.Now;
					v.TERMINAL = "XXXX";
					arrKonten.Add(v);
				}
				cnt = conn1.execSTD_insert(arrKonten);
				TESTFRAME.TEST("insert STD:", 100, cnt);
//				Console.WriteLine("insert: array:{0} -> db-rv:{1}", arrKonten.Count, cnt);
				rv_str = conn1.execSQL("SELECT count(*) FROM knk_test");
				TESTFRAME.TEST("count after insert STD:", "104", rv_str);
//				Console.WriteLine("konten in db: {0}", rv_str);
				rv_str = conn1.execSQL("delete FROM knk_test where id>=@1", 200);
//				Console.WriteLine("delete: rv={0}", rv_str);
				TESTFRAME.TEST("delete STD:", "101", rv_str);
				TESTFRAME.TESTGROUP_END();
#endif

#if false // simple sql test
				rv_str = conn.execSQL("SELECT count(*) FROM konten");
				rv_str = conn.execSQL("SELECT ktonr FROM konten where id=:2 and curcode=:1", "EUR", 2);
#endif

#if false // execSQL_select test
				ArrayList arrStr = new ArrayList();
				ArrayList arrInt = new ArrayList();
				ArrayList myList = new ArrayList();
				ArrayList myList2 = new ArrayList();

				int cnt_select;
				while ((cnt_select = conn.execSQL_select("SELECT * FROM konten", arrStr, arrInt, myList, myList2)) > 0)
				{
					for (int i = 0; i < cnt_select; i++)
					{
						Console.WriteLine(arrStr[i] + "|" + arrInt[i] + "|" + myList[i] + "|" + myList2[i]);
					}
				}
				conn.execSQL_cleanup();

				conn = pool.getConnection();
				rv_str = conn.execSQL("SELECT * FROM konten");
				pool.releaseConnection(conn);
#endif
#if false
				int counter = 0;
				while ((cnt_select = conn.execSQL_select("SELECT * FROM positionen", 20, arrStr, arrInt)) > 0)
				{
					for (int i = 0; i < cnt_select; i++)
					{
						Console.WriteLine(arrStr[i] + "|" + arrInt[i]);
						counter++;
					}
				}
				Console.WriteLine("count=" + counter);
				conn.execSQL_cleanup();
				pool.releaseConnection(conn);
#endif

#if false // tansactional interface test
				conn = pool.getConnection();
				conn.TSQL_startTransaction();
				rv_str = conn.execSQL("SELECT * FROM konten for update");
				pool.releaseConnection(conn);
#endif

#if false // STD_select test
				conn = pool.getConnection();
				testStruct xxx = new testStruct();
				conn.execSTD_select(xxx, "select ktonr from konten where ktonr='21'");
				pool.releaseConnection(conn);

				conn = pool.getConnection();
				ArrayList arr = conn.execSTD_select("positionen", null);
				pool.releaseConnection(conn);
				foreach (testStruct2 e in arr)
				{
					Console.WriteLine("{0}|{1}", e.amt, e.memo);
				}
#endif
				if (pool.Provider == ConnectionPool.ProviderType.SqlServer)
				{
					rv_str = conn1.execSQL("ALTER DATABASE BT_ZETS SET ALLOW_SNAPSHOT_ISOLATION OFF");
				}
				pool.shutdown();
			}
			catch (Exception msg)
			{
				Console.WriteLine(msg.ToString());
				Environment.Exit(-1);
			}
			TESTFRAME.TESTSUITE_END();
		}
	}
}
