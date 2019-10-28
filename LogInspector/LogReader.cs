using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;

namespace RecoverLogInspector
{
    public struct LogReaderEvent
    {
        public int Maximum { get; set; }
        public int Value { get; set; }
    }

    class LogReader
    {
        public EventHandler<LogReaderEvent> OnProgress;


        public List<RecoverLog> OpenLogs(string dir)
        {
            var testInformation = new List<RecoverLog>();

            int index = 0;
            var directories = Directory.GetFiles(dir, "*.XML");
            foreach (string filename in directories)
            {
                var log = RecoverLog.Deserialize(filename);
                log.Path = filename;

                testInformation.Add(log);

                index++;
                OnProgress?.Invoke(this, new LogReaderEvent { Value = index, Maximum = directories.Count() });
            }

            return testInformation;
        }
    }
}
