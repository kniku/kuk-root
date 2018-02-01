using System;
using System.Collections.Generic;
using System.Data;
using System.Reflection;
using System.Text;

namespace Knk.Base.Reflection
{
    public static class SimpleORMapper
    {
        public static T ObjectFromDataRow<T>(DataRow Row) where T : class, new()
        {
            T r = new T();
            foreach (var prop in r.GetType().GetProperties())
            {
                PropertyInfo propertyInfo = r.GetType().GetProperty(prop.Name);
                if (propertyInfo != null && Row.Table.Columns.Contains(prop.Name) && Row[prop.Name] != DBNull.Value)
                {
                    if (propertyInfo.PropertyType.IsEnum)
                        propertyInfo.SetValue(r, Enum.ToObject(propertyInfo.PropertyType, Convert.ToInt32(Row[prop.Name])), null);
                    else
                        propertyInfo.SetValue(r, Convert.ChangeType(Row[prop.Name], propertyInfo.PropertyType), null);
                }
            }
            return r;
        }
    }
}
