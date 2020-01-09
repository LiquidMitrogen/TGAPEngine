using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine;
using TGAPE_CS.Engine.Loader;

namespace TGAPE_CS
{
    public static class Program
    {
        public static void Main()
        {
            var engine = new MainEngine(800, 600);
            var scene = BamFileReader.LoadScene(@"C:\Users\szymo\Documents\TGAPEngine\TGAPEngine\res\cube.bams");
            Console.WriteLine("fffffffuuuuuuu");
        }
    }
}
