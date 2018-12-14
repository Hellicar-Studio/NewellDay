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
        Thread renderThread;
        private static bool showGui = false;
        private static Form2Gui gui;

        public Form1()
        {
            InitializeComponent();

            //FormBorderStyle = FormBorderStyle.None;
            //WindowState = FormWindowState.Maximized;
            renderThread = new Thread(new ParameterizedThreadStart(RenderThread));

            renderThread.SetApartmentState(ApartmentState.MTA);

            renderThread.Start(this.Handle);

            gui = new Form2Gui(this);
            gui.Show();

            this.KeyPress += new KeyPressEventHandler(Form1_KeyPress);
        }

        void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == 'g')
            {
                showGui = !showGui;
                if (showGui)
                    gui.Show();
                else
                    gui.Hide();
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

                    viewer.Init(1, mediaTypeBuffer, hWnd.ToInt64());

                    int width, height;
                    viewer.GetVideoSize(out width, out height);
                    ResizeFormToFitVideoSize(width, height);

                    long startPosition = inFile.BaseStream.Position;

                    while(true)
                    {
                        Console.WriteLine("Video Started!");
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
                                viewer.SetVideoPosition(0, -1080 / 2, 1920, 1080 / 2);
                                viewer.RenderVideoSample(sampleFlags, startTime, stopTime, bufferBytes);
                            }
                        }

                        Console.WriteLine("Video Done!");

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

        private delegate void ResizeFormToFitVideoSizeDelegate(int width, int height);

        private void ResizeFormToFitVideoSize(int width, int height)
        {
            if(InvokeRequired)
            {
                Invoke(new ResizeFormToFitVideoSizeDelegate(ResizeFormToFitVideoSize), new object[] { width, height });
                return;
            }
            ClientSize = new System.Drawing.Size(width, height);
        }
    }
}
