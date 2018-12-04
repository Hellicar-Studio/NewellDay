using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.InteropServices;
using AxisMediaParserLib;

namespace Parser
{
    class Program
    {
        static BinaryWriter outFile;
        static object fileLock = new object ();

        static void Main(string[] args)
        {
            // Create the AXIS Media Parser object and set connection properties
            AxisMediaParser parser = new AxisMediaParser();
            parser.MediaURL = "axmphttp://192.168.1.51/mjpg/1/video.mjpg";
            parser.MediaUsername = "root";
            parser.MediaPassword = "pass";
            // Register for OnVideoSample events
            parser.OnVideoSample += OnVideoSample;

            Console.WriteLine("Press Enter To Start");
            Console.ReadLine();

            try
            {
                using (FileStream outFileStream = new FileStream("C:\\Axis\\video.bin", FileMode.Create)) using (outFile = new BinaryWriter(outFileStream))
                {
                    Console.WriteLine("Connecting to {0}", parser.MediaURL);
                    int cookieID;
                    int numberOfStreams;
                    object mediaBuffer;
                    parser.Connect(out cookieID, out numberOfStreams, out mediaBuffer);
                    // Write media type information to file (buffer is an array of bytes)
                    byte[] mediaTypeBuffer = (byte[])mediaBuffer;
                    outFile.Write(mediaTypeBuffer.Length);
                    outFile.Write(mediaTypeBuffer, 0, mediaTypeBuffer.Length);

                    // Start parser, OnVideoSample() will be called for each parsed frame
                    parser.Start();

                    // Sleep while OnVideoSample()
                    System.Threading.Thread.Sleep(5000);

                    // Stop the stream, the file C:\Axis\video.bin contains the 5 seconds video
                    parser.Stop();
                    Console.WriteLine("Stream stopped");
                    Console.ReadLine();
                }
            }
            catch (COMException e)
            {
                Console.WriteLine("Exception from URL: {0}, Error: {1}", parser.MediaURL, e.Message);
                Console.ReadLine();
            }
        }


        //Event hanler callbck for video samples buffers
        static void OnVideoSample(int cookieID, int sampleType, int sampleFlags, ulong startTime, ulong stopTime, object SampleArray)
        {
            byte[] bufferBytes = (byte[])SampleArray;
            Console.WriteLine("OnVideoSample - Received {0} bytes", bufferBytes.Length);
            lock (fileLock)
            {
                outFile.Write(sampleType);
                outFile.Write(sampleFlags);
                outFile.Write(startTime);
                outFile.Write(stopTime);
                outFile.Write(bufferBytes.Length);
                outFile.Write(bufferBytes, 0, bufferBytes.Length);
            }
        }
    }
}
