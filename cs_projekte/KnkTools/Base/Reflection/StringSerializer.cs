using System;
using System.Linq;
using System.Reflection;
using System.Text;

namespace Knk.Base.Reflection
{
    public static class StringSerializer
    {
        public class SkipPublicMemberAttribute : Attribute {}

        public static StringBuilder PublicPropertiesToString(object iObject, StringBuilder iStringBuilder = null)
        {
            if (iStringBuilder == null)
                iStringBuilder = new StringBuilder();

            foreach (var prop in iObject.GetType().GetProperties().Where(pi => !Attribute.IsDefined(pi, typeof(SkipPublicMemberAttribute))))
            {
                PropertyInfo propertyInfo = iObject.GetType().GetProperty(prop.Name);
                if (propertyInfo != null && propertyInfo.CanRead)
                    iStringBuilder.AppendFormat("{0}{1}=[{2}]", (iStringBuilder.Length == 0 ? "" : ","), prop.Name, propertyInfo.GetValue(iObject));
            }
            return iStringBuilder;
        }

        public static StringBuilder PublicFieldsToString(object iObject, StringBuilder iStringBuilder)
        {
            if (iStringBuilder == null)
                iStringBuilder = new StringBuilder();

            foreach (var fieldInfo in iObject.GetType().GetFields().Where(fi => !Attribute.IsDefined(fi, typeof(SkipPublicMemberAttribute))))
            {
                iStringBuilder.AppendFormat("{0}{1}=[{2}]", (iStringBuilder.Length == 0 ? "" : ","), fieldInfo.Name, fieldInfo.GetValue(iObject));
            }
            return iStringBuilder;
        }

        public static StringBuilder PublicMembersToString(object iObject, StringBuilder iStringBuilder)
        {
            return PublicPropertiesToString(iObject, PublicFieldsToString(iObject, iStringBuilder));
        }
    }
}
