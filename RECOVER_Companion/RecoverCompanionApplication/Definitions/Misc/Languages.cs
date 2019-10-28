using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc
{
    static class Languages
    {
        public static CultureInfo[] GetAvailableLanguages()
        {            

            var executingDirectory = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location);
            var subDirectories = System.IO.Directory.EnumerateDirectories(executingDirectory);


            //Loop all directories looking for resource file
            var availableLanguages = new List<CultureInfo>();
            foreach (var directory in subDirectories)
            {
                var files = System.IO.Directory.EnumerateFiles(directory);
                files = files.Select(a => System.IO.Path.GetFileName(a));

                //If the directory has a resources.dll then the language is present to be used!
                if (files.Any(a => a.ToUpper() == "RECOVERCOMPANIONAPPLICATION.RESOURCES.DLL"))                
                    availableLanguages.Add(new CultureInfo(System.IO.Path.GetFileName(directory)));
                
            }

            //Add English as well!
            availableLanguages.Add(new CultureInfo("en-GB"));

            return availableLanguages.ToArray();
        }


    }
}
