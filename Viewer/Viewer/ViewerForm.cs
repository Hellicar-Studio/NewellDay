using System;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using AxisMediaViewerLib;

namespace Viewer
{
    public partial class ViewerForm : Form
    {
        Thread renderThread;
        private static bool showGui = false;
        private static GuiForm gui;
        //private static int scale = 1;

        private static int VideoXPos;
        private static int VideoYPos;
        private static int VideoWidth;
        private static int VideoHeight;

        public ViewerForm()
        {
            InitializeComponent();

            //FormBorderStyle = FormBorderStyle.None;
            //WindowState = FormWindowState.Maximized;
            renderThread = new Thread(new ParameterizedThreadStart(RenderThread));

            renderThread.SetApartmentState(ApartmentState.MTA);

            renderThread.Start(this.Handle);

            gui = new GuiForm(this);
            gui.Show();

            this.KeyPress += new KeyPressEventHandler(OnKeyPress);
        }

        void OnKeyPress(object sender, KeyPressEventArgs e)
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

        public void OnParameterChanged(object sender, System.EventArgs e)
        {
            string name = (sender as NumericUpDown).Name;
            int v = (int)(sender as NumericUpDown).Value;
            switch(name)
            {
                case "XPosition":
                    VideoXPos = v;
                    break;
                case "YPosition":
                    VideoYPos = v;
                    break;
                case "Width":
                    VideoWidth = v;
                    break;
                case "Height":
                    VideoHeight = v;
                    break;
                default:
                    break;
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
