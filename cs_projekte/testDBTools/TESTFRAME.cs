using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace testDBTools
{
	static class TESTFRAME
	{
		static uint TEST_COUNT = 0;
		static uint TEST_FAILURES = 0;
		static string TEST_CURRENT_GROUP = "";
		static uint TEST_COUNT_CURRENT_GROUP = 0;
		static uint TEST_FAILURES_CURRENT_GROUP = 0;

		public static void TESTSUITE_START()
		{
			TEST_COUNT = 0;
			TEST_FAILURES = 0;
			ConsoleColor old_color = System.Console.ForegroundColor;
			System.Console.ForegroundColor = ConsoleColor.Yellow;
			System.Console.WriteLine("SUITE START");
			System.Console.ForegroundColor = old_color;
		}
		public static void TESTSUITE_END()
		{
			ConsoleColor old_color = System.Console.ForegroundColor;
			System.Console.ForegroundColor = ConsoleColor.Yellow;
			System.Console.WriteLine("SUITE END");
			if (TEST_FAILURES_CURRENT_GROUP > 0) System.Console.ForegroundColor = ConsoleColor.Red;
			else System.Console.ForegroundColor = ConsoleColor.Green;
			System.Console.WriteLine("RESULT: Tests: {0}, Passed: {1}, Failures: {2}", TEST_COUNT, TEST_COUNT - TEST_FAILURES, TEST_FAILURES);
			System.Console.ForegroundColor = old_color;
		}

		public static void TESTGROUP_START(string testinfo)
		{
			TEST_CURRENT_GROUP = testinfo;
			TEST_COUNT_CURRENT_GROUP = 0;
			TEST_FAILURES_CURRENT_GROUP = 0;

			ConsoleColor old_color = System.Console.ForegroundColor;
			System.Console.ForegroundColor = ConsoleColor.Yellow;
			System.Console.WriteLine("GROUP START: " + TEST_CURRENT_GROUP);
			System.Console.ForegroundColor = old_color;
		}

		public static void TESTGROUP_END()
		{
			ConsoleColor old_color = System.Console.ForegroundColor;
			System.Console.ForegroundColor = ConsoleColor.Yellow;
			System.Console.WriteLine("GROUP END: " + TEST_CURRENT_GROUP);
			if (TEST_FAILURES_CURRENT_GROUP > 0) System.Console.ForegroundColor = ConsoleColor.Red;
			else System.Console.ForegroundColor = ConsoleColor.Green;
			System.Console.WriteLine("RESULT: Tests: {0}, Passed: {1}, Failures: {2}", TEST_COUNT_CURRENT_GROUP, TEST_COUNT_CURRENT_GROUP - TEST_FAILURES_CURRENT_GROUP, TEST_FAILURES_CURRENT_GROUP);
			System.Console.ForegroundColor = old_color;
		}

		public static void TEST(string info, object soll, object ist)
		{
			TEST_COUNT++;
			TEST_COUNT_CURRENT_GROUP++;
			bool ok = soll.ToString().Equals(ist.ToString());
			ConsoleColor old_color = System.Console.ForegroundColor;
			if (!ok)
			{
				System.Console.ForegroundColor = ConsoleColor.Red;
				TEST_FAILURES++;
				TEST_FAILURES_CURRENT_GROUP++;
			}

			System.Console.WriteLine("[{0}({1})] {2}: SOLL={3} IST={4} -> {5}",
				TEST_CURRENT_GROUP, TEST_COUNT_CURRENT_GROUP, info, soll, ist, ok);

			System.Console.ForegroundColor = old_color;
		}
	}
}
