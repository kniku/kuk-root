using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Text;
using System.Threading.Tasks;

namespace Knk.Base.Framework.Diagnostic
{
    public static class ManagementObject
    {
        static ManagementObjectSearcher moWin32;

        private static void init_objects()
        {
            if (moWin32 == null)
                moWin32 = new ManagementObjectSearcher("root\\CIMV2", "Select * from Win32_OPeratingSystem");
        }

        private static object GetValueFromManagementObjectWin32(string name)
        {
            init_objects();
            foreach (var o in moWin32.Get())
            {
                if (o is System.Management.ManagementObject obj)
                {
                    return obj[name];
                }
            }

            return null;
        }

        public static UInt64? GetFreePhysicalMemory
        {
            get
            {
                var xxx = GetValueFromManagementObjectWin32("FreePhysicalMemory");
                return xxx == null ? (UInt64?) null : Convert.ToUInt64(xxx);
            }
        }
        public static UInt64? GetTotalRamSize
        {
            get
            {
                var xxx = GetValueFromManagementObjectWin32("TotalVisibleMemorySize");
                return xxx == null ? (UInt64?) null : Convert.ToUInt64(xxx);
            }
        }

        //private void GetPhysicalMemory()
        //{
        //    if (moWin32 == null)
        //        moWin32 = new ManagementObjectSearcher("root\\CIMV2", "Select * from Win32_OPeratingSystem");

        //    foreach (var o in moWin32.Get())
        //    {
        //        if (o is System.Management.ManagementObject obj)
        //        {
        //            var _totalRamMemory = GetTotalRamUsage(obj);
        //            var _freePhysicalMemory = GetFreePhysicalMemory(obj);
        //        }
        //    }
        //}
    }
}
