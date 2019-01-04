using System;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using AxisMediaViewerLib;
using Newtonsoft.Json.Linq;

namespace Viewer
{
    public partial class ViewerForm : Form
    {
        private static Thread renderThread;

        private string videoDirPath = "";
        private string videoPath = "";
        private string guiPath = "";
        private static string defaultGuiPath = "";

        private int WindowXPos = 0;
        private int WindowYPos = 0;
        private int WindowWidth = 0;
        private int WindowHeight = 0;

        private static int VideoXPos = 0;
        private static int VideoYPos = 0;
        private static int VideoWidth = 0;
        private static int VideoHeight = 0;

        public ViewerForm()
        {
            InitializeComponent();

            // set the border style, position and size  of the form
            FormBorderStyle = FormBorderStyle.None;

            // Create a thread for rendering the video content.
            renderThread = new Thread(new ParameterizedThreadStart(RenderThread));
            renderThread.SetApartmentState(ApartmentState.MTA);
            renderThread.Start(this.Handle);

            defaultGuiPath = "default/gui.json";
        }

        public void setWindowParametersFromCommandLineArguments(string[] args)
        {
            for (int i = 0; i < args.Length; i++)
            {
                Console.WriteLine(args[i]);
            }
            // Look at our arguments
            for (int i = 0; i < args.Length; i++)
            {
                // Check if we are recieving the name of an argument
                if (args[i].Substring(0, 2) == "--")
                {
                    // We got a real argument!
                    switch (args[i])
                    {
                        case "--data-path":
                            videoDirPath = args[++i];
                            break;
                        case "--window-x":
                            if (!int.TryParse(args[++i], out WindowXPos))
                            {
                                Console.WriteLine("Failed to parse: " + args[i] + " to integer, default set");
                                WindowXPos = 0;
                            }
                            break;
                        case "--window-y":
                            if(!int.TryParse(args[++i], out WindowYPos))
                            {
                                Console.WriteLine("Failed to parse: " + args[i] + " to integer, default set");
                                WindowYPos = 0;
                            }
                            break;
                        case "--window-width":
                            if (!int.TryParse(args[++i], out WindowWidth))
                            {
                                Console.WriteLine("Failed to parse: " + args[i] + " to integer, default set");
                                WindowWidth = 1920;
                            }
                            break;
                        case "--window-height":
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
            setDataParameters();
        }

        private void setWindowParameters()
        {
            Location = new System.Drawing.Point(WindowXPos, WindowYPos);
            ClientSize = new System.Drawing.Size(WindowWidth, WindowHeight);
        }

        private void setDataParameters()
        {
            videoPath = getVideoFilePathFromVideoDirPath(videoDirPath);
            guiPath = getGuiFilePathFromVideoDirPath(videoDirPath);
            // Load Settings From Gui
            loadSettingsFromGui(guiPath);
        }

        void loadSettingsFromGui(string guiPath)
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
                using (FileStream inFileStream = new FileStream(videoPath, FileMode.Open)) using (BinaryReader inFile = new BinaryReader(inFileStream, Encoding.UTF32))
                {
                    if(inFile.PeekChar() != -1)
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
                    } else
                    {
                        Console.WriteLine(string.Format("File at {0} is empty or malformed", videoPath));
                        Environment.Exit(0);
                    }
                }
            }
            catch (COMException e)
            {
                MessageBox.Show(string.Format("Exception {0}", e.Message));
            }
        }

        string getVideoFilePathFromVideoDirPath(string videoDirPath)
        {
            string[] files = Directory.GetFiles(videoDirPath, "*.bin");

            if(files.Length == 1)
            {
                return files[0];
            }
            // If we have no or multiple videos so we need to exit now.
            Console.WriteLine("There were " + files.Length + " videos in the specified video directory: " + videoDirPath + " Exiting.");
            Environment.Exit(0);
            return "";
        }

        string getGuiFilePathFromVideoDirPath(string videoDirPath)
        {
            string[] files = Directory.GetFiles(videoDirPath, "*.json");

            if (files.Length == 1)
            {
                return files[0];
            }
            //// If we have no or multiple gui files we need to default to the default one.
            Console.WriteLine("There we " + files.Length + " json files in the specified video directory: " + videoDirPath + " Setting default gui path to: " + defaultGuiPath);
            return "default/gui.json";
        }

    }
}
