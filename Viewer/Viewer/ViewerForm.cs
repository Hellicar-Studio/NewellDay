using System;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using AxisMediaViewerLib;
using Newtonsoft.Json.Linq;
using System.Net;

namespace Viewer
{
    public partial class ViewerForm : Form
    {
        private static Thread renderThread;
        public string videoDir;
        //Thread responseThread;
        public int WindowXPos;
        public int WindowYPos;
        public int WindowWidth;
        public int WindowHeight;

        private static int VideoXPos;
        private static int VideoYPos;
        private static int VideoWidth;
        private static int VideoHeight;

        //private static HttpListener httpListener;

        public ViewerForm()
        {
            InitializeComponent();

            // set the border style, position and size  of the form
            FormBorderStyle = FormBorderStyle.None;

            //WindowState = FormWindowState.Maximized;

            // Create a thread for rendering the video content.
            renderThread = new Thread(new ParameterizedThreadStart(RenderThread));

            renderThread.SetApartmentState(ApartmentState.MTA);

            renderThread.Start(this.Handle);

            // Load Settings From Gui
            LoadSettingsFromGui("gui.json");
        }

        public void setWindowParametersFromCommandLineArguments(string[] args)
        {
            // Look at our arguments
            for (int i = 0; i < args.Length; i++)
            {
                // Check if we are recieving the name of an argument
                if (args[i].Substring(0, 2) == "--")
                {
                    // We got a real argument!
                    switch (args[i])
                    {
                        case "--VideoFolder":
                            videoDir = args[++i];
                            break;
                        case "--X":
                            if (!int.TryParse(args[++i], out WindowXPos))
                            {
                                Console.WriteLine("Failed to parse: " + args[i] + " to integer, default set");
                                WindowXPos = 0;
                            }
                            break;
                        case "--Y":
                            if(!int.TryParse(args[++i], out WindowYPos))
                            {
                                Console.WriteLine("Failed to parse: " + args[i] + " to integer, default set");
                                WindowYPos = 0;
                            }
                            break;
                        case "--Width":
                            if (!int.TryParse(args[++i], out WindowWidth))
                            {
                                Console.WriteLine("Failed to parse: " + args[i] + " to integer, default set");
                                WindowWidth = 1920;
                            }
                            break;
                        case "--Height":
                            if (!int.TryParse(args[++i], out WindowHeight))
                            {
                                Console.WriteLine("Failed to parse: " + args[i] + " to integer, default set");
                                WindowHeight = 1080;
                            }
                            break;
                        default:
                            Console.WriteLine("Unknown Command: " + args[i] + " With argument: " + args[++i]);
                            break;
                    }
                }
                else
                {
                    // We did not get a real argument!
                    Console.WriteLine("Malformed Command: " + args[i] + ",\n Arguments must be preceded by -- and succeeded by their argument");
                }
            }

            setWindowParameters();
        }

        private void setWindowParameters()
        {
            Location = new System.Drawing.Point(WindowXPos, WindowYPos);
            ClientSize = new System.Drawing.Size(WindowWidth, WindowHeight);
        }

        void LoadSettingsFromGui(string guiPath)
        {
            using (StreamReader r = new StreamReader(guiPath))
            {
                string json = r.ReadToEnd();
                JObject settings = JObject.Parse(json);
                Console.Write(settings);
                VideoXPos = Convert.ToInt32(settings["Camera:0"]["X"]);
                VideoYPos = Convert.ToInt32(settings["Camera:0"]["Y"]);
                VideoWidth = Convert.ToInt32(settings["Camera:0"]["Width"]);
                VideoHeight = Convert.ToInt32(settings["Camera:0"]["Height"]);
            }
        }

        void RenderThread(object obj)
        {
            IntPtr hWnd = (IntPtr)obj;

            AxisMediaViewer viewer = new AxisMediaViewer();
            viewer.VMR9 = true;

            try
            {
                using (FileStream inFileStream = new FileStream("C:\\Axis\\video.bin", FileMode.Open)) using (BinaryReader inFile = new BinaryReader(inFileStream, Encoding.UTF32))
                {
                    int mediaTypeSize = inFile.ReadInt32();
                    byte[] mediaTypeBuffer = inFile.ReadBytes(mediaTypeSize);

                    long startPosition = inFile.BaseStream.Position;

                    while (true)
                    {
                        viewer.Init(1, mediaTypeBuffer, hWnd.ToInt64());

                        viewer.Start();

                        while (inFile.PeekChar() != -1)
                        {
                            // Read frame data
                            int sampleType = inFile.ReadInt32();
                            int sampleFlags = inFile.ReadInt32();
                            ulong startTime = inFile.ReadUInt64();
                            ulong stopTime = inFile.ReadUInt64();
                            int bufferSize = inFile.ReadInt32();
                            byte[] bufferBytes = inFile.ReadBytes(bufferSize);
                            // Check that it’s not an audio sample.
                            if (sampleType != (int)AMV_VIDEO_SAMPLE_TYPE.AMV_VST_MPEG4_AUDIO)
                            {
                                // Let the viewer render the frame
                                viewer.SetVideoPosition(VideoXPos, VideoYPos, VideoXPos + VideoWidth, VideoYPos + VideoHeight);
                                viewer.RenderVideoSample(sampleFlags, startTime, stopTime, bufferBytes);
                            }
                        }

                        inFile.BaseStream.Position = startPosition;

                        viewer.Stop();
                    }

                }
            }
            catch (COMException e)
            {
                MessageBox.Show(string.Format("Exception {0}", e.Message));
            }
        }

        //private delegate void ResizeFormToFitVideoSizeDelegate(int width, int height);

        //private void ResizeFormToFitVideoSize(int width, int height)
        //{
        //    if(InvokeRequired)
        //    {
        //        Invoke(new ResizeFormToFitVideoSizeDelegate(ResizeFormToFitVideoSize), new object[] { width, height });
        //        return;
        //    }
        //    ClientSize = new System.Drawing.Size(width, height);
        //}
    }
}
