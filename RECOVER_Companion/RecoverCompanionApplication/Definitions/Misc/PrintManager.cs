using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing.Printing;
using System.Drawing;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc
{
    public struct PrintObject
    {
        public string Description;
        public string Value;
        public bool? IsPositive;
    }

    class PrintManager
    {
        private RecoverLog[] logs;
        private Font largeFont;
        private Font defaultFont;
        private Font boldFont;
        private Brush defaultBrush;
        private Brush successBrush;
        private Brush failBrush;
        private StringFormat centeredFormat;
        private ProgressWindow progressWindow;        
        private int logIndex;
        private int pageNumber;
        private int totalPages;
        private bool singlePagePerLog = false;


        public PrintManager()
        {
                        largeFont = new Font("Segoi UI Semilight", 16, FontStyle.Regular, GraphicsUnit.Pixel);
            defaultFont = new Font("Segoi UI Semilight", 12, FontStyle.Regular, GraphicsUnit.Pixel);
            boldFont = new Font(defaultFont, FontStyle.Bold);

            defaultBrush = Brushes.Black;
            successBrush = Brushes.Green;
            failBrush = Brushes.Red;

            centeredFormat = new StringFormat()
            {
                LineAlignment = StringAlignment.Center,
                Alignment = StringAlignment.Center
            };
        }


        public void PrintRecoverLog(RecoverLog recoverLog)
        {

            PrintRecoverLog(new [] { recoverLog });
        }

        public void PrintRecoverLog(IEnumerable<RecoverLog> recoverLogs, bool useSinglePagePerLog = false)
        {
            singlePagePerLog = useSinglePagePerLog;

            //Setup Progress window
            progressWindow = new ProgressWindow();
            progressWindow.SetText(Strings.PrintingInProgress);

            //Reset Values
            logIndex = 0;
            pageNumber = 1;
            totalPages = 0;

            //Set Logs
            logs = recoverLogs.ToArray();            

            //Setup printing
            var printDocument = new PrintDocument();
            printDocument.PrintPage += new PrintPageEventHandler(this.Pd_printPage);

            //Start Printing
            new System.Threading.Thread(() => {
                printDocument.Print();

                App.Current.Dispatcher.Invoke(() =>
                {
                    progressWindow.SetText(Strings.PrintingFinished);
                    progressWindow.SetAllowClose(true);
                    progressWindow.SetIsIndetermite(false);
                });


            }).Start();

            //Show progress window
            progressWindow.ShowDialog();
        }


        private void Pd_printPage(object sender, PrintPageEventArgs e)
        {
            //Calculate amount of pages
            if (pageNumber == 1)
            {                
                using (var graphics = Graphics.FromImage(new Bitmap(10, 10, System.Drawing.Imaging.PixelFormat.Format24bppRgb)))
                {
                    var dummyEventArgs = new PrintPageEventArgs(graphics, e.MarginBounds, e.PageBounds, e.PageSettings);
                    do
                    {
                        totalPages++;
                        PrintPage(dummyEventArgs);
                    }
                    while (dummyEventArgs.HasMorePages);
                }

                //Reset Values
                logIndex = 0;
                pageNumber = 1;                
            }

            PrintPage(e);                        
        }


        private void PrintPage(PrintPageEventArgs e)
        {
            int top = e.MarginBounds.Top;
            int left = e.MarginBounds.Left;
            int maxWidth = e.MarginBounds.Width;            
            var xCenter = maxWidth / 2.0f;
            int rowPadding = 40;
            int maximumValueWidth = 250;
            var currentYHeight = (float)top;
            int rowIndex = 0;
            var logBoundingBoxes = new List<RectangleF>();

            //Add Recover logo
            if (pageNumber == 1)
            {
                using (var stream = System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("FosterAndFreeman.RecoverCompanionApplication.Resources.Images.RECOVER-LFT-cutBW.png"))
                {
                    //Set Sizes
                    int imageWidth = 400, imageHeight = 96;

                    //Draw Image
                    var image = Image.FromStream(stream);
                    e.Graphics.DrawImage(image, left + xCenter - (imageWidth / 2), top - 20, imageWidth, imageHeight);

                    //Update Height
                    currentYHeight += imageHeight;
                }
            }



            float maximumHeightOffset = 0;
            for (int pageIndex = 0; logIndex < logs.Count(); pageIndex++, logIndex++)
            {

                float heightOffset = 0;
                var log = logs[logIndex];

                //If Single page per log then move to the next page
                if (singlePagePerLog && pageIndex > 0)
                    break;

                //Work out if there needs to be a second object                    
                bool singleRow = logIndex == logs.Count() - 1 && logs.Count() % 2 != 0;

                //Work out center of this instance
                float localisedXCenter = xCenter;
                if (!singleRow && !singlePagePerLog)
                    localisedXCenter = rowIndex % 2 == 0 ? maxWidth * 0.25f : maxWidth * 0.75f;

                //Work out title sizes
                var titleSize = e.Graphics.MeasureString(log.StartTime.ToString("G"), largeFont);
                var titleLeft = left + localisedXCenter - (titleSize.Width / 2);

                var printObjects = log.PrintObjects;

                //If the log has extra information add it to the front
                if (log.HasExtraInformation)
                {
                    //Add Reference and Operator to beginning
                    var extraObjects = new List<PrintObject>();
                    if (log.HasCaseReference)
                        extraObjects.Add(new PrintObject() { Description = Strings.CaseReference, Value = log.CaseReference, IsPositive = null });
                    if (log.HasCaseOperator)
                        extraObjects.Add(new PrintObject() { Description = Strings.CaseOperator, Value = log.CaseOperator, IsPositive = null });

                    if (extraObjects.Any())
                        printObjects = extraObjects.Concat(printObjects).ToArray();

                    //Add Notes to end
                    extraObjects.Clear();
                    if (log.HasNotes)
                    {
                        extraObjects.Add(new PrintObject() { Description = Strings.Notes, Value = log.Notes, IsPositive = null });
                        printObjects = printObjects.Concat(extraObjects).ToArray();
                    }
                }

                //Check if there is enough space first
                double calculatedHeight = currentYHeight + heightOffset + titleSize.Height;
                foreach (var printObject in printObjects)
                {
                    var valueSize = e.Graphics.MeasureString(printObject.Value, boldFont, maximumValueWidth);
                    calculatedHeight += valueSize.Height + 5;
                }

                //If theres no space go to the next page
                if (calculatedHeight > e.MarginBounds.Bottom)
                    break;

                //Define sizes for box
                var logBounding = new RectangleF()
                {
                    X = 99999,
                    Y = currentYHeight + heightOffset - 10,                    
                };
                



                //Title                            
                e.Graphics.DrawString(log.StartTime.ToString("G"), largeFont, defaultBrush, titleLeft, currentYHeight + heightOffset);

                heightOffset += titleSize.Height;


                foreach (var printObject in printObjects)
                {
                    //Work out sizes of text
                    var descriptionSize = e.Graphics.MeasureString(printObject.Description, defaultFont);
                    var valueSize = e.Graphics.MeasureString(printObject.Value, boldFont, maximumValueWidth);

                    //Work out xPositions
                    var totalWidth = descriptionSize.Width + valueSize.Width;
                    var startPosition = left + (localisedXCenter - (totalWidth / 2));
                    var valueStart = startPosition + descriptionSize.Width;                    

                    //Update log bounds
                    if (logBounding.X > startPosition - 10)
                        logBounding.X = startPosition - 10;
                    if (logBounding.X < totalWidth + 20)
                        logBounding.X = totalWidth + 20;


                    //Work out what colour the value text should be
                    Brush valueBrush;
                    if (printObject.IsPositive == true)
                        valueBrush = successBrush;
                    else if (printObject.IsPositive == false)
                        valueBrush = failBrush;
                    else
                        valueBrush = defaultBrush;                    


                    //Put on page
                    e.Graphics.DrawString(printObject.Description, defaultFont, defaultBrush, new RectangleF(startPosition, currentYHeight + heightOffset, descriptionSize.Width, descriptionSize.Height));
                    e.Graphics.DrawString(printObject.Value, boldFont, valueBrush, new RectangleF(valueStart, currentYHeight + heightOffset, valueSize.Width, valueSize.Height));


                    //Increase Height for next object
                    heightOffset += valueSize.Height + 5;

                    //Store the maximum height for each log (for when there are multiple side by side
                    if (maximumHeightOffset < heightOffset)
                        maximumHeightOffset = heightOffset;
                }

                logBounding.Height = ((currentYHeight + heightOffset) - logBounding.Y) + 20;                
                logBounding.Width = (maxWidth / 2) - 10;                
                logBounding.X = left + localisedXCenter - (logBounding.Width / 2);                

                logBoundingBoxes.Add(logBounding);                

                rowIndex++;

                //If new row, then update current height and pad
                if (rowIndex > 1 || singleRow)
                {
                    rowIndex = 0;
                    currentYHeight += maximumHeightOffset + rowPadding;
                    maximumHeightOffset = 0;

                    //Draw Boxes
                    var boundingBoxHeight = logBoundingBoxes.OrderByDescending(a => a.Height).First().Height;
                    foreach (var boundingBox in logBoundingBoxes)                                     
                        e.Graphics.DrawRectangle(Pens.Black, 
                            boundingBox.X, 
                            boundingBox.Y, 
                            boundingBox.Width,
                            boundingBoxHeight);

                    logBoundingBoxes.Clear();
                    

                }
            }

            //Add Page Number                        
            var pageNumberText = $"{Strings.Page} {pageNumber}/{totalPages}";            
            e.Graphics.DrawString(pageNumberText, defaultFont, defaultBrush, left, e.MarginBounds.Bottom + 20);


            //Increase page number for next page
            pageNumber++;

            //Work out if there are more logs
            e.HasMorePages = logIndex < logs.Count();
        }

    }
}
