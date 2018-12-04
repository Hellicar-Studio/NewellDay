using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using AxisMediaViewerLib;

namespace Viewer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            // Create the render thread that will read file and render frames
            renderThread = new Thread(new ParameterizedThreadStart(RenderThread));

            // Set apartment state to MTA to allow multiple threads access
            renderThread.SetApartmentState(ApartmentState.MTA);

            // Start the render thread and send in the Win32 handle to this window
            renderThread.Start(this.Handle);
        }

        Thread renderThread;

        // A thread started in the Form1() constructor which reads a binary
        // media file and renders the frames in the form window
        void RenderThread(object obj)
        {
            // Get the hWnd windows handle provided in Form1() method above
            IntPtr hWnd = (IntPtr)obj;

            // Create the Axis Media Viewer COM object and set properties
            AxisMediaViewer viewer = new AxisMediaViewer();
            viewer.VMR9 = true;

            try
            {
                // Open binary input file to read parsed video frames from
                using (FileStream inFileStream = new FileStream("C:\\Axis\\video.bin", FileMode.Open)) using (BinaryReader inFile = new BinaryReader(inFileStream, Encoding.UTF32))
                {
                    // Read media type information fro infile
                    int mediaTypeSize = inFile.ReadInt32();
                    byte[] mediaTypeBuffer = inFile.ReadBytes(mediaTypeSize);

                    // Init the Viewer with media type and the hWnd handle to the window.
                    viewer.Init(1, mediaTypeBuffer, hWnd.ToInt64());

                    // Get the video frame size and resize the form to fit the video stream.
                    int width;
                    int height;
                    viewer.GetVideoSize(out width, out height);
                    ResizeFormToFitVideoSize(width, height);

                    // Start the viewer before rendering the first frame
                    viewer.Start();

                    // While there are frames in the bin file, read each frame and let
                    // the viewer render it
                    while(inFile.PeekChar() != -1)
                    {
                        // Read frame data
                        int sampleType = inFile.ReadInt32();
                        int sampleFlags = inFile.ReadInt32();
                        ulong startTime = inFile.ReadUInt64();
                        ulong stopTime = inFile.ReadUInt64();
                        int bufferSize = inFile.ReadInt32();
                        byte[] bufferBytes = inFile.ReadBytes(bufferSize);
                        // Check that it's not an audio sample
                        if (sampleType != (int)AMV_VIDEO_SAMPLE_TYPE.AMV_VST_MPEG4_AUDIO)
                        {
                            // Let the viewer render the frame
                            viewer.RenderVideoSample(sampleFlags, startTime, stopTime, bufferBytes);
                        }
                    }

                    viewer.Stop();
                }
            }
            catch (COMException e)
            {
                MessageBox.Show(string.Format("Exception {0}", e.Message));
            }
        }

        // Function and delegate to resize the form
        private delegate void ResizeFormToFitVideoSizeDelegate(int width, int height);

        private void ResizeFormToFitVideoSize(int width, int height)
        {
            if(InvokeRequired)
            {
                // If called from a non UI thread, let the UI thread do the call
                Invoke(new ResizeFormToFitVideoSizeDelegate(ResizeFormToFitVideoSize), new object[] { width, height });
                return;
            }
            ClientSize = new System.Drawing.Size(width, height);
        }
    }
}
