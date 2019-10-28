using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using MigraDoc.DocumentObjectModel;
using MigraDoc.Rendering;
using MigraDoc.RtfRendering;
using PdfSharp.Pdf;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Markup;
using System.Xml;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc
{
    struct ReportType
    {
        public string Subject;
        public string Value;
        public bool? IsAcceptable;
    }


    static class Reporting
    {
        public static void GenerateReport(string path, RecoverLog log, string graphPath)
        {
            var document = new MigraDoc.DocumentObjectModel.Document();
            var section = document.AddSection();
            section.PageSetup = document.DefaultPageSetup.Clone();
            var imageWidth = section.PageSetup.PageWidth - 125;
            var columnWidth = (imageWidth / 2) + 20;



            //Define values for report (Allows me to loop through easily without a bunch of repeated code)
            var reportValues = new ReportType[]
            {
                new ReportType() { Subject = Strings.BaseHeaterDeviation, Value = log.BaseString, IsAcceptable = log.BaseHeaterWithinTolerance },
                new ReportType() { Subject = Strings.PrecursorHeaterDeviation, Value = log.PrecursorString, IsAcceptable = log.PrecursorHeaterWithinTolerance },
                new ReportType() { Subject = Strings.PressureDeviation, Value = log.PressureString, IsAcceptable = log.PressureDeviationWithinTolerance },
                new ReportType() { Subject = Strings.LeakTest1Result, Value = log.LeakTest1 ? Strings.TestSuccess : Strings.TestFail, IsAcceptable = log.LeakTest1 },
                new ReportType() { Subject = Strings.LeakTest2Result, Value = log.LeakTest2 ? Strings.TestSuccess : Strings.TestFail, IsAcceptable = log.LeakTest2 },
                new ReportType() { Subject = Strings.PumpdownMinutes, Value = log.PumpDownRunTime.ToString(), IsAcceptable = null },
                new ReportType() { Subject = Strings.HeatMinutes, Value = log.TimeToReachVacuumSetpoint.ToString(), IsAcceptable = null },
                new ReportType() { Subject = Strings.FirmwareVersion, Value = log.FirmwareBuild.ToString(), IsAcceptable = null},
                new ReportType() { Subject = Strings.SerialNumber, Value = log.SerialNumber, IsAcceptable = null },
                new ReportType() { Subject = Strings.MetalType, Value = log.MetalTypeString, IsAcceptable = null },
                new ReportType() { Subject = Strings.ChamberType, Value = log.ChamberSizeString, IsAcceptable = null },
                new ReportType() { Subject = Strings.DevelopRType, Value = log.DevelopRType, IsAcceptable = null }
            };

            if (log.HasExtraInformation)
            {
                var extraValues = new List<ReportType>();

                if (log.HasCaseReference)
                    extraValues.Add(new ReportType() { Subject = Strings.CaseReference, Value = log.CaseReference, IsAcceptable = null });

                if (log.HasCaseOperator)
                    extraValues.Add(new ReportType() { Subject = Strings.CaseOperator, Value = log.CaseOperator, IsAcceptable = null });                                                        

                reportValues = extraValues.Concat(reportValues).ToArray();

                extraValues.Clear();
                if (log.HasNotes)
                {
                    extraValues.Add(new ReportType() { Subject = Strings.Notes, Value = log.Notes, IsAcceptable = null });
                    reportValues = reportValues.Concat(extraValues).ToArray();
                }
            }

            //Add Title
            var title = section.AddParagraph();
            title.Format = new ParagraphFormat()
            {
                Alignment = ParagraphAlignment.Center,
                Font = new Font("Segoe UI Light", 20),
            };

            title.AddText(log.StartTime.ToString());


            //Add graph
            if (graphPath != null)
            {
                var graph = section.AddImage(graphPath);
                graph.Width = imageWidth;
            }

            //Add Information            
            var table = section.AddTable();

            table.Format.Alignment = MigraDoc.DocumentObjectModel.ParagraphAlignment.Center;
            
            var subjectColumn = table.AddColumn(columnWidth);
            var valueColumn = table.AddColumn(columnWidth);

            foreach (var reportValue in reportValues)
            {
                //Add Row
                var row = table.AddRow();

                //Add Subject
                var subjectParagraph = row.Cells[0].AddParagraph();
                subjectParagraph.Format = new MigraDoc.DocumentObjectModel.ParagraphFormat()
                {
                    Alignment = MigraDoc.DocumentObjectModel.ParagraphAlignment.Right,
                    Font = new MigraDoc.DocumentObjectModel.Font("Segoe UI Light"),
                };
                subjectParagraph.Format.Alignment = MigraDoc.DocumentObjectModel.ParagraphAlignment.Right;
                subjectParagraph.Add(new MigraDoc.DocumentObjectModel.Text(reportValue.Subject));

                //Add Value
                var valueParagraph = row.Cells[1].AddParagraph();
                valueParagraph.Format = new MigraDoc.DocumentObjectModel.ParagraphFormat()
                {
                    Alignment = MigraDoc.DocumentObjectModel.ParagraphAlignment.Left,
                    Font = new MigraDoc.DocumentObjectModel.Font("Segoe UI Black")
                };

                if (reportValue.IsAcceptable == true)
                    valueParagraph.Format.Font.Color = MigraDoc.DocumentObjectModel.Color.FromRgb(0x00, 0xFF, 0x00);
                else if (reportValue.IsAcceptable == false)
                    valueParagraph.Format.Font.Color = MigraDoc.DocumentObjectModel.Color.FromRgb(0xFF, 0x00, 0x00);
                valueParagraph.Add(new MigraDoc.DocumentObjectModel.Text(reportValue.Value));

            }
            
            if (System.IO.Path.GetExtension(path).ToUpper() == ".PDF")
            {            
                var pdfRenderer = new PdfDocumentRenderer(true)
                {
                    Document = document
                };

                pdfRenderer.RenderDocument();
                pdfRenderer.Save(path);
            }
            else
            {
                var rtfRenderer = new RtfDocumentRenderer();
                rtfRenderer.Render(document, path, System.IO.Path.GetDirectoryName(path));                
            }


        }


        private static string MigraDocFilenameFromByteArray(byte[] image)
        {
            return "base64:" + Convert.ToBase64String(image);
        }
    }
}
