using System;
using System.Text;
using Knk.Base.Reflection;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace UnitTest_Base
{
    [TestClass]
    public class Reflection
    {
        class ToSerialize
        {
#pragma warning disable 169
            private string field_private = "field_private";
            protected string field_protected = "field_protected";
            public int field_public = 10;
            public DateTime field_public_date = new DateTime(2018, 01, 26);
            public string prop_public_get_set { get; set; } = "public_get_set";
            public string prop_public_get { get; } = "public_get";
            private string _prop_public_set = "public_set";
            public string prop_public_set
            {
                set => _prop_public_set = value;
            }
            private string prop_private_get_set { get; set; } = "private_get_set";
            protected string prop_protected_get_set { get; set; } = "protected_get_set";
            [StringSerializer.SkipPublicMember]
            public int prop_skipped { get; set; } = 11;
            [StringSerializer.SkipPublicMember]
            public int field_skipped = 12;
#pragma warning restore 169
        }

        [TestMethod]
        public void TestStringSerializer()
        {
            ToSerialize obj = new ToSerialize();
            StringBuilder sb = new StringBuilder();

            // property serialization
            StringSerializer.PublicPropertiesToString(obj, sb);
            Assert.AreEqual(sb.ToString(), "prop_public_get_set=[public_get_set],prop_public_get=[public_get]");

            int len = sb.Length;
            StringSerializer.PublicPropertiesToString(obj, sb);
            Assert.AreEqual(2 * len + 1, sb.Length);

            // field serialization
            sb.Clear();
            StringSerializer.PublicFieldsToString(obj, sb);
            Assert.AreEqual(sb.ToString(), "field_public=[10],field_public_date=[26.01.2018 00:00:00]");

            len = sb.Length;
            StringSerializer.PublicFieldsToString(obj, sb);
            Assert.AreEqual(2 * len + 1, sb.Length);

            // public member serialization
            sb.Clear();
            StringSerializer.PublicMembersToString(obj, sb);
            Assert.AreEqual(sb.ToString(), "field_public=[10],field_public_date=[26.01.2018 00:00:00],prop_public_get_set=[public_get_set],prop_public_get=[public_get]");

            len = sb.Length;
            StringSerializer.PublicMembersToString(obj, sb);
            Assert.AreEqual(2 * len + 1, sb.Length);
        }
    }
}
