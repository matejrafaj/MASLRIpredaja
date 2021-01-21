using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZynqCamApp.Helpers
{
    public static class Options
    { 
        public static bool testImage { get; set; }

        public static byte[] Serialize()
        {
            List<byte> options = new List<byte>();

            options.Add(Constants.OptionMessageStart);
            options.AddRange(BitConverter.GetBytes(testImage));

            return options.ToArray();
        }

    }
}
